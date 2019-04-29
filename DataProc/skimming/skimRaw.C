/////////////////////////////////////////////
// macro for skimming RAW data
// Used for CALOPLUS from PbPb 2018 data
// 21.11.2018
// Martin Poghosyan 
/////////////////////////////////////////////

#include <bitset>
#include <TROOT.h>
#include <TError.h>
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TGrid.h>
#include <Riostream.h>
#include <TEnv.h>
#include <TAlienCollection.h>
#include "AliRawReader.h"

#include "event.h"
#include "AliRawVEvent.h"
#include "AliRawEventHeaderBase.h"
#include "AliRawVEquipment.h"
#include "AliRawEquipmentHeader.h"
#include "AliRawDataHeader.h"
#include "AliRawData.h"
#include "AliDAQ.h"
#include "AliCDBManager.h"
#include "AliCDBEntry.h"
#include "AliRawDataArrayV2.h"
#include "AliRawEventV2.h"
#include "AliTriggerClass.h"
#include "AliTriggerConfiguration.h"
#include "AliTriggerCluster.h"
#include "ARVersion.h"
#include "RVersion.h"
#include "TStopwatch.h"

using std::cout;
using std::endl;

Bool_t Process(TChain *rawTree, const ULong64_t mask_1, const ULong64_t maskNext50_1);
TChain* CreateChainXML(const char *xmlfile, const char* chName = "RAW");
TChain* CreateChainTXT(const char *txtfile, const char* chName = "RAW");

long bread = 0, bwrite = 0;

Bool_t skimRaw(const char* list = "wn.xml")
{

  TStopwatch wc;
  wc.Start();
  
  if (!gGrid) TGrid::Connect("alien://");
  TString data = list;
  TChain *chain = data.EndsWith(".xml") ? CreateChainXML(data,"RAW") : CreateChainTXT(data,"RAW");
  if (!chain) {printf("No chain\n"); return -1;}
  AliRawReader* rd = AliRawReader::Create(chain->GetFile()->GetName());
  rd->NextEvent();
  int runNb = rd->GetRunNumber();
  delete rd;
  ////////////////////
  // get CALOPLUS trigger info from OCDB 
  AliCDBManager *man = AliCDBManager::Instance();
  if (gSystem->AccessPathName("OCDB.root", kFileExists)==0) {        
    man->SetDefaultStorage("local://");
    man->SetRaw(kFALSE);
    man->SetSnapshotMode("OCDB.root");
  }
  else {
    man->SetDefaultStorage("raw://");
  }
  man->SetRun(runNb);

  AliCDBEntry *entry2 = man->Get("GRP/CTP/Config");
  AliTriggerConfiguration* cfg = (AliTriggerConfiguration*) entry2->GetObject();
  ULong64_t mask = 0;
  ULong64_t maskNext50 = 0;
  TString trclasses;
  const TObjArray& classesArray =cfg->GetClasses();
  Int_t nclasses = classesArray.GetEntriesFast();
  for( Int_t iclass=0; iclass < nclasses; iclass++ ) {
    AliTriggerClass* trclass = (AliTriggerClass*)classesArray.At(iclass);
    TString clname = trclass->GetName();
    if(!clname.Contains("CALOPLUS") || clname.Contains("CPHI7")  || clname.Contains("CDMC7PER") )
      continue;
    Int_t trindex = trclass->GetIndex()-1;
    if(trindex <50)
      mask|=1ull <<trindex;
    else
      maskNext50|=1ull <<(trindex-50);

    trclasses += trclass->GetName();
    trclasses += "  ";
    
  }
  
  printf("---------- Selected trigger classses and masks ----------------------------\n");
  printf("%s\n", trclasses.Data());
  std::cout << "mask   " << std::bitset<64>(mask) << "  ("<<mask<<")"<<endl;
  std::cout << "mask50 " << std::bitset<64>(maskNext50) << "  ("<<maskNext50<<")"<< endl;
  printf("--------------------------------------------------------------------------\n");
  //////////////////
  
  //  return Process(Form("%s/000%d/raw/", period.Data(),runNb), chunkName, mask, maskNext50)) {
  bool res = Process(chain,  mask, maskNext50);
  wc.Stop();
  wc.Print();
  printf("Bytes: read: %ld written %ld\n",bread,bwrite);
  return res;
}


const char *GetRootAliRootInfo()
{
  // Return Root/AliRoot version and revisiosn 

  static TString version;
  version = "AliRoot version=";
  version+=TString(ALIROOT_VERSION);
  version+=", reversion=";
  version+=TString(ALIROOT_REVISION);
  version+= "; Root version=";
  version+=gROOT->GetVersion();
  return version.Data();
}


static void BranchResetBit(TBranch *b) 
{
  // Reset MapObject on this branch and all the sub-branches

  b->ResetBit( kBranchObject | kBranchAny ); // Or in newer ROOT: b->ResetBit( kMapObject )
  TIter next( b->GetListOfBranches() );
  TBranch *sub = 0;
  while ( (sub = (TBranch*)next() ) ) {
    BranchResetBit(sub);
  }
}


Bool_t Process(TChain *rawTree, const ULong64_t mask_1, const ULong64_t maskNext50_1)
{
  int run = AliCDBManager::Instance()->GetRun();
  TFile newfile(Form("skim%09d.root",run),"recreate");
    
  TTree *newtree = new TTree("RAW", Form("ALICE skimmed raw-data tree (%s)", GetRootAliRootInfo()));
  Int_t basketsize = 32000;
  Long64_t autoflush=-5000000LL;
  Int_t split   = 99;
  newtree->SetAutoSave(21000000000LL);  // autosave when 21 Gbyte written
  newtree->SetAutoFlush(autoflush);
  newtree->BranchRef();
  
  AliRawEventV2   *rawEvent=NULL; 
  TObjArray brNames; // names of branches to read
  brNames.SetOwner(kTRUE); 

  TNamed* bname = new TNamed("rawevent","");
  brNames.Add( bname ); // this must be the 1st branch to read

  rawTree->SetBranchAddress( bname->GetName() , &rawEvent);
  TBranch *b = newtree->Branch( bname->GetName() , "AliRawEventV2", &rawEvent, basketsize, split);
  BranchResetBit(b);

  Int_t detBranches[AliDAQ::kNDetectors+1] = {
    1, /*  1 ITSSPD */
    1, /*  2 ITSSDD */
    1, /*  3 ITSSSD */
    18, /*  4 TPC */
    1, /*  5 TRD */
    1, /*  6 TOF */
    1, /*  7 HMPID */
    1, /*  8 PHOS */
    1, /*  9 CPV */
    1, /* 10 PMD */
    1, /* 11 MUONTRK */
    1, /* 12 MUONTRG */
    1, /* 13 FMD */
    1, /* 14 T0 */
    1, /* 15 VZERO */
    1, /* 16 ZDC */
    1, /* 17 ACORDE */
    1, /* 18 TRG */
    1, /* 19 EMCAL */
    1, /* 20 DAQ_TEST */
    1, /* 21 EMPTY */
    1, /* 22 AD */
    1, /* 23 MFT */
    1, /* 24 FIT */
    10,/* 25 HLT */
    1  /* 26 --- */
  };


  // select branches	
  Bool_t kDetAccept[AliDAQ::kNDetectors+1] = {
    kTRUE , //SPD
    kFALSE, //SDD
    kTRUE , //SSD
    kFALSE, //TPC
    kTRUE , //TRD
    kTRUE , //TOF
    kFALSE, //HMPID
    kFALSE, //PHOS
    kFALSE, //CPV
    kFALSE, //PMD
    kFALSE, //MUONTRK
    kFALSE, //MUONTRG
    kFALSE, //FMD
    kTRUE , //T0
    kTRUE , //VZERO
    kTRUE , //ZDC
    kFALSE, //ACORDE
    kTRUE , //TRG
    kTRUE , //EMCAL
    kFALSE, //DAQ_TEST
    kFALSE, //EMPTY
    kFALSE, //AD
    kFALSE, //MFT
    kFALSE, //FIT
    kFALSE, //HLT
    kTRUE   //----
  };


  Int_t detCount=0;
  Int_t detIndex[AliDAQ::kNDetectors+1]={-1};

  for(Int_t i=0; i<= AliDAQ::kNDetectors; i++) {
    if(kDetAccept[i]) {
      detIndex[i]=detCount;    
      detCount++;
    }
  }

  AliRawDataArrayV2  **fDetRawData[detCount];
  
  for (Int_t iDet = 0; iDet < AliDAQ::kNDetectors; iDet++) {
    if (detIndex[iDet]<0) continue;
    
    fDetRawData[detIndex[iDet]] = new AliRawDataArrayV2*[detBranches[iDet]];
    Int_t nDDLsPerBranch = AliDAQ::NumberOfDdls(iDet)/detBranches[iDet];
    for (Int_t iBranch = 0; iBranch < detBranches[iDet]; iBranch++) {
      fDetRawData[detIndex[iDet]][iBranch] = new AliRawDataArrayV2(nDDLsPerBranch);
    }
  }
  
  fDetRawData[ detCount ] = new AliRawDataArrayV2*[detBranches[AliDAQ::kNDetectors]];
  for (Int_t iBranch = 0; iBranch < detBranches[AliDAQ::kNDetectors]; iBranch++) {
    fDetRawData[detCount][iBranch] = new AliRawDataArrayV2(100);
  }
  // Make brach for each sub-detector
  for (Int_t iDet = 0; iDet < AliDAQ::kNDetectors; iDet++) {
    if(!kDetAccept[iDet]) continue;
    for (Int_t iBranch = 0; iBranch < detBranches[iDet]; iBranch++) {
      bname = new TNamed(Form("%s%d",AliDAQ::DetectorName(iDet),iBranch),"");
      rawTree->SetBranchAddress(bname->GetName(), &fDetRawData[detIndex[iDet]][iBranch]);
      b = newtree->Branch(bname->GetName(),"AliRawDataArrayV2", &fDetRawData[detIndex[iDet]][iBranch],basketsize,split);
      brNames.Add(bname);
      BranchResetBit(b);
    }
  }
  // Make special branch for unrecognized raw-data payloads
  for (Int_t iBranch = 0; iBranch < detBranches[AliDAQ::kNDetectors]; iBranch++) {
    bname = new TNamed(Form("%s%d","Common",iBranch),"");
    rawTree->SetBranchAddress( bname->GetName(), &fDetRawData[detCount][iBranch]);
    b = newtree->Branch(  bname->GetName(), "AliRawDataArrayV2", &fDetRawData[detCount][iBranch],basketsize,split);
    brNames.Add(bname);
    BranchResetBit(b);
  }

  printf("Branches to read: \n");
  for (int ib=0;ib<brNames.GetEntriesFast();ib++) printf("%2d %s\n",ib, brNames[ib]->GetName());
  
  Int_t nEvents = rawTree->GetEntries();
  printf("Will process %d events\n",nEvents);
  //loop over events
  int currTree = rawTree->GetTreeNumber();
  for(Int_t iEvent=0; iEvent < nEvents; iEvent++) {

    int iloc = rawTree->LoadTree(iEvent);
    bread += rawTree->GetBranch( brNames[0]->GetName() )->GetEntry(iloc);
    
    AliRawEventHeaderBase *rawEventHeader = rawEvent->GetHeader();
    UInt_t  DAQAttributes = rawEventHeader->GetP("TypeAttribute")[2];
    
    if ((DAQAttributes&ATTR_2_B(ATTR_INCOMPLETE_EVENT))==0  && (DAQAttributes&ATTR_2_B(ATTR_FLUSHED_EVENT))==0 ) {
      const UInt_t *pattern = rawEventHeader->GetP("TriggerPattern");
      ULong64_t rawMask =  pattern ? (((ULong64_t)pattern[1] & 0x3ffff) << 32)|(pattern[0]) : 0;
      ULong64_t rawMask50= pattern ? ((ULong64_t)(pattern[1]>>18) & 0x3fff)|((ULong64_t)(pattern[2]&0xffffffff)<<14)|((ULong64_t)(pattern[3]&0xf)<<46):0;
      UInt_t type = rawEventHeader->Get("Type");
      
      if( ((rawMask & mask_1) || (rawMask50 & maskNext50_1)) && type==7) { // event is to be stored, read the rest      
	for (int ib=1;ib<brNames.GetEntriesFast();ib++) {
	  bread += rawTree->GetBranch( brNames[ib]->GetName() )->GetEntry(iloc);
	  //      printf("Ev%4d br %2d %s | %d\n",iEvent,ib,brNames[ib]->GetName(),bread);
	}
	//	printf("Ev%4d (%4d) | %ld\n",iEvent,iloc, bread);
	bwrite += newtree->Fill();
      }
    }
    //    rawEvent->Clear();
  }
  
  printf("%lld events selected out of %d \n",  newtree->GetEntriesFast(), nEvents); 
  
  newtree->Write();
  
  delete newtree;

  newfile.Close();
  
  return kTRUE;
}



//________________________________________________________________________________
TChain* CreateChainXML(const char *xmlfile, const char* chName)
{
  // Create a chain using url's from xml file
  printf("***************************************\n");
  printf("    Getting chain of trees %s\n", chName);
  printf("***************************************\n");
  TAlienCollection *coll = (TAlienCollection*)TAlienCollection::Open(xmlfile);
  if (!coll) {
    ::Error("CreateChain", "Cannot create an AliEn collection from %s", xmlfile);
    return NULL;
  }
  TChain *chain = new TChain(chName);
  coll->Reset();
  while (coll->Next()) {
    chain->Add(coll->GetTURL());
  }
  if (!chain->GetNtrees()) {
    ::Error("CreateChain", "No tree found from collection %s", xmlfile);
    return NULL;
  }
  printf("Created chain with %d trees from %s\n",chain->GetNtrees(),xmlfile);
  return chain;
}

//________________________________________________________________________________
TChain* CreateChainTXT(const char* inpData, const char* chName)
{
  TChain* chain = new TChain(chName);
  //
  TString inpDtStr = inpData;
  if (inpDtStr.EndsWith(".root")) {
    chain->AddFile(inpData);
  }
  else {
    //
    ifstream inpf(inpData);
    if (!inpf.good()) {
      printf("Failed on input filename %s\n",inpData);
      return 0;
    }
    //
    TString flName;
    flName.ReadLine(inpf);
    while ( !flName.IsNull() ) {
      flName = flName.Strip(TString::kBoth,' ');
      if (flName.BeginsWith("//") || flName.BeginsWith("#")) {flName.ReadLine(inpf); continue;}
      flName = flName.Strip(TString::kBoth,',');
      flName = flName.Strip(TString::kBoth,'"');
      printf("Adding %s\n",flName.Data());
      chain->AddFile(flName.Data());
      flName.ReadLine(inpf);
    }
  }
  //
  printf("Created chain with  %d trees from %s\n",chain->GetNtrees(),inpData);
  return chain;
}
