//          ReshuffleRawTree.C
//          ==================
//
// macro for reshuffling ALICE RAW data in ROOT format
// used for EMCAL data from LHC18r
// 
// modifications needed for using for a different detector
//  1. const Int_t emcID = AliDAQ::DetectorID("EMCAL"); <-  set the desired detector name 
//  2. rawReader1[iF]->Select("EMCAL", 0, 40);  <- detector name + DDL range
//  3. AliReshuffleRawTreeHelper object contianing the map
//
// Revision history:
// 24.01.2019 - Martin Poghosyan - created
// 25.01.2019 - Ruben Shahoyan   - memory issue fix
// 26.01.2019 - MP               - correction map added, (minor) changes + cosmetics 
// 29.01.2019 - MP               - changes for adapting with the "final" JDL 
// 30.01.2019 - MP               - use AliReshuffleRawTreeHelper (instead of AliEMCALLHC18rHelper)
// 09.02.2019 - MP               - replace if(emcInReadout) with if(emcInReadout && igld>=0)
// verison 1.8


//#define MODE_LOCAL

#include <TROOT.h>
#include <TError.h>
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TGrid.h>
#include <Riostream.h>
#include <TRefTable.h>
#include <TSystem.h>
#include <TStopwatch.h>
#include <TAlienCollection.h>

#include "AliRawReader.h"
#include "AliRawVEvent.h"
#include "AliRawEventHeaderBase.h"
#include "AliRawVEquipment.h"
#include "AliRawEquipmentHeader.h"
#include "AliRawDataHeader.h"
#include "AliRawData.h"
#include "AliDAQ.h"
#include "AliRawDataArrayV2.h"
#include "AliRawEventV2.h"
#include "ARVersion.h"
#include "RVersion.h"
#include "AliRawReaderRoot.h"
#include "AliRawDataHeaderV3.h"

#ifndef MODE_LOCAL
#include "AliReshuffleRawTreeHelper.h"
#else
// all 3 in different files
const TString rootFileNameIn1("../18000297363039.317.root"); 
const Int_t EvSel1 = 231;
const TString rootFileNameIn2("../18000297363026.329.root"); 
const Int_t EvSel2 = 229;
// both in the input file 
//const TString rootFileNameIn1("../18000297363019.327.root"); 
//const Int_t EvSel1 = 200;
//const TString rootFileNameIn2("../18000297363019.327.root"); 
//const Int_t EvSel2 = 228;
// both in the same file but different from input
//const TString rootFileNameIn1("../18000297363039.317.root"); 
//const Int_t EvSel1 = 231;
//const TString rootFileNameIn2("../18000297363039.317.root"); 
//const Int_t EvSel2 = 226;
#endif

#define DBG_BASE     if ( debug > 0 )
#define DBG_DETAILED if ( debug > 1 )
#define DBG_VERBOSE  if ( debug > 2 )

const Int_t debug=0;
const Int_t emcID = AliDAQ::DetectorID("EMCAL");

TChain* CreateChainXML(const char *xmlfile, const char* chName = "RAW");
TChain* CreateChainTXT(const char *txtfile, const char* chName = "RAW");
const char *GetRootAliRootInfo();
static void BranchResetBit(TBranch *b); 
const string GetFileName(const char *str); 

Bool_t ReshuffleRawTree(const char* list = "wn.xml")
{
 
  TString data = list;
  TChain *rawTree = data.EndsWith(".xml") ? CreateChainXML(data,"RAW") : CreateChainTXT(data,"RAW");
  if (!rawTree) {printf("No chain\n"); return -1;}
  rawTree->SetName("RAWa");
  TRefTable* tabSav = TRefTable::GetRefTable();

 if (!gGrid) TGrid::Connect("alien://");

#ifndef MODE_LOCAL
 AliRawReaderRoot* rd = new AliRawReaderRoot(rawTree->GetFile()->GetName());
 rd->NextEvent();
 Int_t runNb = rd->GetRunNumber();
 delete rd;
 TFile *fileHelper = TFile::Open(Form("AliReshuffleRawTreeHelper_000%d.root",runNb));
 if (!fileHelper || !fileHelper->IsOpen())
    {
    ::Fatal( "ReshuffleRawTree", Form("AliReshuffleRawTreeHelper_000%d.root file not found",runNb));
    }
 
 AliReshuffleRawTreeHelper *gLHC18rH = (AliReshuffleRawTreeHelper*) fileHelper->Get("aliReshuffleRawTreeHelper");
 if(!gLHC18rH)
    {
    ::Fatal( "ReshuffleRawTree", Form("mapping object not found in AliReshuffleRawTreeHelper_000%d.root",runNb));
    }
 gLHC18rH->PrintInfo();
#endif


//const string rootFileNameOut = Form("emcfixed_%s",GetFileName(rawTree->GetFile()->GetName()).data());
const string rootFileNameOut = GetFileName(rawTree->GetFile()->GetName());
  // Create the output tree (see AliRawDB)
  TFile newfile( rootFileNameOut.data(),"recreate");
  TTree *newtree = new TTree("RAW", Form("ALICE [EMCAL fixed] raw-data tree (%s)", GetRootAliRootInfo()));
  Int_t basketsize = 32000;
  Long64_t autoflush=-5000000LL;
  Int_t split   = 99;
  newtree->SetAutoSave(21000000000LL);  // autosave when 21 Gbyte written
  newtree->SetAutoFlush(autoflush);
  newtree->BranchRef();
  TRefTable* tabSavNew = TRefTable::GetRefTable();
  AliRawEventV2   *rawEvent=NULL; 
  TObjArray brNames; // names of branches to read
  brNames.SetOwner(kTRUE); 

  rawTree->SetBranchAddress( "rawevent" , &rawEvent);
  TBranch *b = newtree->Branch( "rawevent" , "AliRawEventV2", &rawEvent, basketsize, split);
  BranchResetBit(b);
  brNames.Add( new TNamed("rawevent","") ); // this must be the 1st branch to read

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
    kTRUE , //SDD
    kTRUE , //SSD
    kTRUE , //TPC
    kTRUE , //TRD
    kTRUE , //TOF
    kTRUE , //HMPID
    kTRUE , //PHOS
    kTRUE , //CPV
    kTRUE , //PMD
    kTRUE , //MUONTRK
    kTRUE , //MUONTRG
    kTRUE , //FMD
    kTRUE , //T0
    kTRUE , //VZERO
    kTRUE , //ZDC
    kTRUE , //ACORDE
    kTRUE , //TRG
    kTRUE , //EMCAL
    kTRUE , //DAQ_TEST
    kTRUE , //EMPTY
    kTRUE , //AD
    kTRUE , //MFT
    kTRUE , //FIT
    kTRUE , //HLT
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
      //      bname = new TNamed(Form("%s%d",AliDAQ::DetectorName(iDet),iBranch),"");
      rawTree->SetBranchAddress(Form("%s%d",AliDAQ::DetectorName(iDet),iBranch), &fDetRawData[detIndex[iDet]][iBranch]);
      b = newtree->Branch(Form("%s%d",AliDAQ::DetectorName(iDet),iBranch),"AliRawDataArrayV2", &fDetRawData[detIndex[iDet]][iBranch],basketsize,split);
      brNames.Add(new TNamed(Form("%s%d",AliDAQ::DetectorName(iDet),iBranch),""));
      BranchResetBit(b);
    }
  }
  // Make special branch for unrecognized raw-data payloads
  for (Int_t iBranch = 0; iBranch < detBranches[AliDAQ::kNDetectors]; iBranch++) {
    //    bname = new TNamed(Form("%s%d","Common",iBranch),"");
    rawTree->SetBranchAddress( Form("%s%d","Common",iBranch), &fDetRawData[detCount][iBranch]);
    b = newtree->Branch(  Form("%s%d","Common",iBranch), "AliRawDataArrayV2", &fDetRawData[detCount][iBranch],basketsize,split);
    brNames.Add(new TNamed(Form("%s%d","Common",iBranch),""));
    BranchResetBit(b);
  }

  long bread = 0, bwrite = 0;

  DBG_VERBOSE
    {
    printf("Branches to read: \n");
    for (int ib=0;ib<brNames.GetEntriesFast();ib++) printf("%2d %s\n",ib, brNames[ib]->GetName());
    }

  Int_t nEvents= (Int_t)rawTree->GetEntries();
  DBG_BASE printf("Will process %d events\n",nEvents);

  void *headSize = nullptr;
  AliRawReaderRoot *rawReader1[2]={nullptr};
  
  //loop over events
  for (Int_t iEvent=0; iEvent < nEvents; iEvent++) 
    {
    DBG_BASE printf("Event #%d     ================================================\n", iEvent);
	int iloc = rawTree->LoadTree(iEvent);
    TRefTable::SetRefTable(tabSav);    
    bread += rawTree->GetBranch( brNames[0]->GetName() )->GetEntry(iloc);
    AliRawEventHeaderBase *rawEventHeader = rawEvent->GetHeader();

    // if a selection is needed
    const UInt_t detPattern =  rawEventHeader->Get("DetectorPattern");
    UInt_t emcInReadout = ( ((1 << emcID) & detPattern) >> emcID);
    DBG_BASE printf("DetectorPattern = %u   emcInReadout=%u\n", detPattern, emcInReadout);

//if(!emcInReadout) continue;

    for (int ib=1;ib<brNames.GetEntriesFast();ib++) {
      bread += rawTree->GetBranch( brNames[ib]->GetName() )->GetEntry(iloc);
      }
      DBG_VERBOSE
       for (int ib=0;ib<brNames.GetEntriesFast();ib++) { printf("Ev%4d br %2d %s | %ld\n",iEvent,ib,brNames[ib]->GetName(),bread);}

      	const UInt_t *id = rawEventHeader->GetP("Id");
	    const UInt_t period =  (((id)[0]>>4)&0x0fffffff);
    	const UInt_t orbit  =  ((((id)[0]<<20)&0xf00000)|(((id)[1]>>12)&0xfffff));
	    const UInt_t bc     =  ((id)[1]&0x00000fff);
		Int_t igld=gLHC18rH->GetGlobIDIndex(period, orbit, bc);
		if(igld<0)
			printf("igld=%d <0!!! %s, event=%d",igld, rawTree->GetFile()->GetName(), iEvent);


	if(emcInReadout && igld>=0)
	{
        TRefTable* tabSav1[2];
#ifdef MODE_LOCAL
        rawReader1[0] = new AliRawReaderRoot(rootFileNameIn1.Data(), EvSel1);
		rawReader1[0]->Select("EMCAL", 0, 40);
        tabSav1[0] = TRefTable::GetRefTable();
        rawReader1[1] = new AliRawReaderRoot(rootFileNameIn2.Data(), EvSel2);
		rawReader1[0]->Select("EMCAL", 0, 40);
        tabSav1[1] = TRefTable::GetRefTable();
#else

     	for(Int_t iF=0; iF<gLHC18rH->GetNoffsets()-1; iF++)
     		{
        	rawReader1[iF] = new AliRawReaderRoot(gLHC18rH->GetChunkFullName(igld+gLHC18rH->GetOffset(iF+1) ).Data(), gLHC18rH->GetEventNumber(igld+gLHC18rH->GetOffset(iF+1)) );
			if(!rawReader1[iF])
			 ::Fatal( "ReshuffleRawTree", Form("could not access the file %s",gLHC18rH->GetChunkFullName(igld+gLHC18rH->GetOffset(iF+1) ).Data()));

			rawReader1[iF]->Select("EMCAL", 0, 40);
        	tabSav1[iF]    =  TRefTable::GetRefTable();
			DBG_DETAILED printf("helperIndex #%d: Using Event=%d from %s \n", iF, gLHC18rH->GetEventNumber(igld+gLHC18rH->GetOffset(iF+1)), gLHC18rH->GetChunkFullName(igld+gLHC18rH->GetOffset(iF+1) ).Data());
     		}
#endif


	    Int_t  deltasizeEvent=0;

	    for (Int_t ldcCounter=0; ldcCounter < rawEvent->GetNSubEvents(); ldcCounter++) 
	    {
	    TRefTable::SetRefTable(tabSav);
	    AliRawVEvent *ldcRootEvent = rawEvent->GetSubEvent(ldcCounter);
        if(!ldcRootEvent) continue;
	    AliRawEventHeaderBase *ldcEventHeader = ldcRootEvent->GetHeader();
        if(!ldcEventHeader) continue;

	    Int_t  deltasizeLDC=0;

	    for (Int_t eqCounter=0; eqCounter < ldcRootEvent->GetNEquipments(); eqCounter++) 
	    {
		    AliRawVEquipment *equipment=ldcRootEvent->GetEquipment(eqCounter);
		    if(!equipment) continue;
		    AliRawEquipmentHeader *equipmentHeader = equipment->GetEquipmentHeader();
		    if(!equipmentHeader) continue;

           	UInt_t ddl_current = equipmentHeader->GetId();
            Int_t ddlIndex;
            Int_t detId = AliDAQ::DetectorIDFromDdlID(ddl_current,ddlIndex);
            if( detId != emcID ) continue;

            Int_t deltasizeEq=0;
            Int_t helperIndex=-1;


#ifdef MODE_LOCAL
		    const UInt_t EqSel = 4608;
        	if(!( (ddl_current == EqSel  || ddl_current == (EqSel+4)) ))
            continue;
        	helperIndex = ddl_current - EqSel;
		    if(helperIndex!=0) helperIndex=1;
#else
            if( ddlIndex>=0 && ddlIndex <=40 )
            	helperIndex = gLHC18rH->GetSMGroupIdex(ddlIndex/2)-1;
#endif
            if(helperIndex<0) continue;

             	DBG_BASE printf("* Replacing the data of DDL=%u with data from helperIndex=%d\n", equipmentHeader->GetId(), helperIndex);

	       	    AliRawData *rawData = equipment->GetRawData();
		        Int_t rawDataSize = equipment->GetRawData()->GetSize();
		        AliRawDataHeaderV3 *cdh = (AliRawDataHeaderV3*)rawData->GetBuffer();
              	DBG_BASE printf("== Initial rawDataSize=%d\n", rawDataSize);

              	// Store the CDH of current event
		        UInt_t Word2  =cdh->fWord2;             // bunch crossing, L1 trigger message and fomrat version
		        UInt_t EventID2 =cdh->fEventID2;          // orbit number
		        UInt_t AttributesSubDetectors =cdh->fAttributesSubDetectors; // block attributes (bits 24-31) and participating sub detectors
		        UInt_t StatusMiniEventID =cdh->fStatusMiniEventID; // status & error bits (bits 12-27) and mini event ID (bits 0-11)
		        UInt_t TriggerClassLow =cdh->fTriggerClassLow;   // low bits of trigger class
		        UInt_t TriggerClassesMiddleLow =cdh->fTriggerClassesMiddleLow; // 18 bits go into eventTriggerPattern[1] (low), 14 bits are zeroes (cdhMBZ2)
		        UInt_t TriggerClassesMiddleHigh =cdh->fTriggerClassesMiddleHigh; // Goes into eventTriggerPattern[1] (high) and [2] (low)
		        UInt_t ROILowTriggerClassHigh =cdh->fROILowTriggerClassHigh; // low bits of ROI data (bits 28-31) and high bits of trigger class (bits 0-17)
		        UInt_t ROIHigh =cdh->fROIHigh; // high bits of ROI data

		        TRefTable::SetRefTable(tabSav1[helperIndex]);
		        AliRawEventV2* rootEvent1 = (AliRawEventV2*)(rawReader1[helperIndex]->GetEvent());
		        AliRawEventHeaderBase *rawEventHeader1 = rootEvent1->GetHeader();

		        const UInt_t detPattern1 =  rawEventHeader1->Get("DetectorPattern");
		        UInt_t emcInReadout1 = ( ((1 << emcID) & detPattern1) >> emcID);
		        DBG_VERBOSE printf("---DetectorPattern1 = %u   emcInReadout1=%u\n", detPattern1, emcInReadout1);

              	if(!emcInReadout1) // add a fatal
                	printf("ERROR: No EMCAL data found form helperIndex=%d !!!\n",helperIndex);

		        for(Int_t ldcCounter1=0; ldcCounter1 < rootEvent1->GetNSubEvents(); ldcCounter1++) 
		         	{
		          	AliRawVEvent *ldcRootEvent1 = rootEvent1->GetSubEvent(ldcCounter1);
                 	if(!ldcRootEvent1) continue;
                 	AliRawEventHeaderBase *ldcEventHeader1 = ldcRootEvent1->GetHeader();
                 	if(!ldcEventHeader1) continue;
                 	if(ldcEventHeader1->Get("LdcId") != ldcEventHeader->Get("LdcId")) continue;

			        for(Int_t eqCounter1=0; eqCounter1 < ldcRootEvent1->GetNEquipments(); eqCounter1++) 
			        	{
			        	AliRawVEquipment *equipment1=ldcRootEvent1->GetEquipment(eqCounter1);
			            if(!equipment1) continue;
			            AliRawEquipmentHeader *equipmentHeader1 = equipment1->GetEquipmentHeader();
			            if(!equipmentHeader1) continue;
			            if( equipmentHeader1->GetId() != equipmentHeader->GetId()) continue;
                      	DBG_VERBOSE
                        	{
                        	printf("Replacing data of Eq%u\n", equipmentHeader1->GetId());
                        	equipmentHeader1->Print();
                        	}
				        AliRawData *rawData1 = equipment1->GetRawData();
				        headSize= equipmentHeader1->HeaderBegin();
                      	DBG_BASE printf("== rawDataSize before replacing =%d\n== rawDataSize        found     =%d\n", rawData->GetSize(),rawData1->GetSize());
                    	int rwsz = rawData1->GetSize();
                    	char* cpdt = new char[rwsz];
                    	memcpy(cpdt,rawData1->GetBuffer(),rwsz);
                    	// copy the data correct event
                    	rawData->SetBuffer(cpdt, rwsz);
                    	DBG_BASE printf("== rawDataSize after  replacing =%d\n", rawData->GetSize());
              			deltasizeEq=(rawData1->GetSize()-rawDataSize);
                    	DBG_BASE printf("== Eq%u data replaced\n", equipmentHeader->GetId());
			            } //eq1 loop 
		            } // ldc1 loop

		            TRefTable::SetRefTable(tabSav);

		            if(deltasizeEq!=0)
		              	{
                		DBG_BASE printf("=== Resizing EquipmenHeader by %d (%p)\n=== EquipmentHeader initial size = %d \n", deltasizeEq, equipmentHeader, equipmentHeader->GetEquipmentSize());
            			void *p = equipmentHeader->HeaderBegin();
            			*(UInt_t*)p = UInt_t(equipmentHeader->GetEquipmentSize()+deltasizeEq);
                		DBG_BASE printf("=== EquipmentHeader final   size = %d\n", equipmentHeader->GetEquipmentSize());
        		      	}
                	// use the CDH of current event  
        		    void *headSize0= equipmentHeader->HeaderBegin();
		            headSize0=headSize;
						    AliRawDataHeaderV3 *cdh1 = (AliRawDataHeaderV3*)equipment->GetRawData()->GetBuffer();
		            cdh1->fWord2  =Word2;             // bunch crossing, L1 trigger message and fomrat version
        		    cdh1->fEventID2 =EventID2;          // orbit number
		            cdh1->fAttributesSubDetectors =AttributesSubDetectors; // block attributes (bits 24-31) and participating sub detectors
        		    cdh1->fStatusMiniEventID =StatusMiniEventID; // status & error bits (bits 12-27) and mini event ID (bits 0-11)
        		    cdh1->fTriggerClassLow =TriggerClassLow;   // low bits of trigger class
        		    cdh1->fTriggerClassesMiddleLow =TriggerClassesMiddleLow; // 18 bits go into eventTriggerPattern[1] (low), 14 bits are zeroes (cdhMBZ2)
        		    cdh1->fTriggerClassesMiddleHigh =TriggerClassesMiddleHigh; // Goes into eventTriggerPattern[1] (high) and [2] (low)
        		    cdh1->fROILowTriggerClassHigh =ROILowTriggerClassHigh; // low bits of ROI data (bits 28-31) and high bits of trigger class (bits 0-17)
        		    cdh1->fROIHigh =ROIHigh; // high bits of ROI data

		            deltasizeLDC+=deltasizeEq;
		            deltasizeEvent+=deltasizeEq;
	        	}// Eq loop
	           	if(deltasizeLDC!=0)
	             	{
                	DBG_BASE printf("==== Resizing LDC[ID%d] Event by %d\n==== LDC Event iniail size = %d\n", ldcEventHeader->Get("LdcId"), deltasizeLDC, ldcEventHeader->GetEventSize());
                	void *p = ldcEventHeader->HeaderBaseBegin();
                	*(UInt_t*)p = UInt_t(ldcEventHeader->GetEventSize()+deltasizeLDC);
                	DBG_BASE printf("==== LDC Event final  size = %d\n", ldcEventHeader->GetEventSize());
	            	}
	         } // ldc loop
          	if(deltasizeEvent!=0)
            	{
            	DBG_BASE printf("===== Resizing GDC[ID%d] Event by %d\n===== GDC Event iniail size = %d\n", rawEventHeader->Get("GdcId"), deltasizeEvent, rawEventHeader->GetEventSize());
      	    	void *p = rawEventHeader->HeaderBaseBegin();
            	*(UInt_t*)p = UInt_t(rawEventHeader->GetEventSize()+deltasizeEvent);
            	DBG_BASE printf("===== GDC Event final  size = %d\n", rawEventHeader->GetEventSize());
            	}

          	delete rawReader1[0];
          	delete rawReader1[1];
    	}// emc in readout

        TRefTable::SetRefTable(tabSavNew);
        newtree->Fill();   
        TRefTable::SetRefTable(tabSav);
        rawEvent->Clear();
        DBG_BASE printf("event filled\n");
    }// event loop
  
  Int_t nEventsNew = (Int_t) newtree->GetEntriesFast();
 
  if( nEventsNew!= nEvents)
	::Fatal( "ReshuffleRawTree", Form("NeventOrig(%lld) != NeventNew(%d) ", nEventsNew , nEvents ));

  printf("%lld events selected out of %d \n",  nEventsNew, nEvents); 

  /*
   * Put the end job descriptors for ML counters
   */
  TString stats = Form("Run%d.Event0_%d.ESD.tag.root", runNb, nEvents);
  TFile* fp1 = new TFile(stats.Data(), "RECREATE");
  fp1->Close();
  stats = Form("%d_%d_0_%d.stat", nEvents, nEvents, nEventsNew);
  fp1 = new TFile(stats.Data(), "RECREATE");
  fp1->Close();
  
  printf("Listing local files...\n");
  gSystem->Exec("ls ./");
  printf("Done\n");


  newfile.cd();
  newtree->Write();
  if (newtree) delete newtree;

  newfile.Close();
  printf("output file %s closed\n", rootFileNameOut.data());

#ifndef MODE_LOCAL
  fileHelper->Close();
#endif
 
 //  return kTRUE;
   
  for (Int_t iDet = 0; iDet < AliDAQ::kNDetectors; iDet++) {
      if (detIndex[iDet]<0) continue;
      Int_t nDDLsPerBranch = AliDAQ::NumberOfDdls(iDet)/detBranches[iDet];
      for (Int_t iBranch = 0; iBranch < detBranches[iDet]; iBranch++) {
	     if(fDetRawData[detIndex[iDet]][iBranch])  delete fDetRawData[detIndex[iDet]][iBranch];
        }
    }
  for (Int_t iBranch = 0; iBranch < detBranches[AliDAQ::kNDetectors]; iBranch++) {
    if(fDetRawData[detCount][iBranch]) delete fDetRawData[detCount][iBranch];
    }
 

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
  printf("Created chain with  %d trees from %s\n",chain->GetNtrees(),inpData);
  return chain;
}

//________________________________________________________________________________
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

//________________________________________________________________________________
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

//________________________________________________________________________________
const string GetFileName(const char *ss) 
{ 
string str=ss;
std::size_t found = str.find_last_of("/\\");
string str1 = str.substr(found+1);
return str1.data();
} 

