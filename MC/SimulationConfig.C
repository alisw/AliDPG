/*
 * AliDPG - ALICE Experiment Data Preparation Group
 * Simulation configuration script
 *
 */

/*****************************************************************/
/*****************************************************************/
/*****************************************************************/

#if (!defined(__CLING__) && !defined(__CINT__)) || defined(__ROOTCLING__) || defined(__ROOTCINT__)
#include "AliSimulation.h"
#include "AliModule.h"
#include "AliMagF.h"
#include "AliPHOSSimParam.h"
#include "TGrid.h"
#include "TROOT.h"
#endif

enum ESimulation_t {
  kSimulationDefault,
  kSimulationMuon,
  kSimulationMuonOnly,
  kSimulationPhosOnly,
  kSimulationEmbedBkg,
  kSimulationEmbedSig,
  kSimulationGeneratorOnly,
  kSimulationNoDigitization,
  kSimulationRun3,
  kSimulationCustom,
  kSimulationDefaultIonTail,
  kSimulationNoTPC,
  kSimulationNoEMCal,
  kNSimulations
};

const Char_t *SimulationName[kNSimulations] = {
  "Default",
  "Muon",
  "MuonOnly",
  "PhosOnly",
  "EmbedBkg",
  "EmbedSig",
  "GeneratorOnly",
  "NoDigitization",
  "Run3",
  "Custom",
  "SimulationDefaultIonTail",
  "NoTPC",
  "NoEMCal"
};


/*****************************************************************/

void SimulationDefault(AliSimulation &sim);
void SimulationConfigPHOS(AliSimulation &sim);
void SimulationRun3(AliSimulation &sim);
void AddDetToGRPRun3(AliDAQ::DetectorBits detAdd, AliDAQ::DetectorBits detRem, int run);
void SetCDBRun3(int run);

AliSimulation* gg_tmp_sim;
void SimulationConfig(AliSimulation &sim, ESimulation_t tag)
{
  
  printf(">>>>> SimulationConfig: %s \n", SimulationName[tag]);
  
  /** fast magnetic field **/
  if (gSystem->Getenv("CONFIG_FASTB")) {  
    if (!AliMagF::Class()->GetMethodAny("SetFastFieldDefault")) {
      printf("ERROR: fast magnetic field is requested, but AliRoot version is not compatible\n");
      abort();
    }
    printf(">>>>> Using fast magnetic field \n");
    AliMagF::SetFastFieldDefault(kTRUE);
 }

  Int_t year;
  switch(tag) {
    
    // Default
  case kSimulationDefault:
    SimulationDefault(sim);
    return;
    
    // Muon
  case kSimulationMuon:
    SimulationDefault(sim);
    sim.SetMakeSDigits("MUON VZERO T0 AD");
    sim.SetMakeDigitsFromHits("ITS");
    sim.SetRunHLT("");
    return;
    
    // Muon only
  case kSimulationMuonOnly:
    SimulationDefault(sim);
    sim.SetMakeSDigits("MUON");
    sim.SetMakeDigits("MUON");
    sim.SetMakeDigitsFromHits("");
    sim.SetRunHLT("");
    sim.SetRunQA("MUON:ALL");
    return;

    // Phos only
  case kSimulationPhosOnly:
    SimulationDefault(sim);
    sim.SetMakeSDigits("PHOS");
    sim.SetMakeDigits("PHOS");
    sim.SetMakeDigitsFromHits("");
    sim.SetRunHLT("");
    sim.SetRunQA(":");
    return;
    
  // EmbedBkg
  case kSimulationEmbedBkg:
    SimulationDefault(sim);
    sim.SetMakeSDigits("ALL");
    sim.SetMakeDigitsFromHits("");
    return;
    
    // EmbedSig
  case kSimulationEmbedSig:
    SimulationDefault(sim);
    sim.SetMakeSDigits("ALL");
    sim.SetMakeDigitsFromHits("");
    {
      TString bgstr = gSystem->Getenv("CONFIG_BGEVDIR");
      if (!bgstr.IsNull()) { 
        if (bgstr.BeginsWith("alien://") && !gGrid && !TGrid::Connect("alien://")) {
	  printf("Failed to create a grid connection\n");
	  abort();
        }
        if (!bgstr.EndsWith("/")) bgstr += "/";
        bgstr += "galice.root";
        sim.EmbedInto(bgstr.Data());
      }
    }
    return;
    
    // GeneratorOnly
  case kSimulationGeneratorOnly:
    SimulationDefault(sim);
    sim.SetRunGeneratorOnly(kTRUE);
    return;

    // NoDigitization
  case kSimulationNoDigitization:
    SimulationDefault(sim);
    sim.SetMakeSDigits("");
    sim.SetMakeDigits("");
    sim.SetMakeDigitsFromHits("");
    sim.SetRunHLT("");
    return;

    // Default
  case kSimulationRun3:
    SimulationRun3(sim);
    return;


    // Custom
  case kSimulationCustom:
    if ((gROOT->LoadMacro("SimulationCustom.C")) != 0) {
      printf("ERROR: cannot find SimulationCustom.C\n");
      abort();
      return;
    }
    gg_tmp_sim = &sim;
    gROOT->ProcessLine("SimulationCustom(*gg_tmp_sim)");
    return;

   // Default simulation enabling IonTail/Crosstalk for TPC
  case kSimulationDefaultIonTail:
      SimulationDefault(sim);
      year = atoi(gSystem->Getenv("CONFIG_YEAR"));
      if (year < 2015) sim.SetMakeSDigits("TPC TRD TOF PHOS HMPID EMCAL MUON ZDC PMD T0 VZERO FMD");
      else             sim.SetMakeSDigits("TPC TRD TOF PHOS HMPID EMCAL MUON ZDC PMD T0 VZERO FMD AD");
      sim.SetMakeDigitsFromHits("ITS");
      return;

   // Default simulation without TPC 
  case kSimulationNoTPC:
    SimulationDefault(sim);
    sim.SetRunHLT("");
    sim.SetMakeDigitsFromHits("ITS");
    // sim.SetMakeDigits("ALL -TPC");  // this line does not work, the Digits tree will anyway be created also with this line. Without this line, an empty digits tree for the TPC will be created
    return;

    // Default simulation without EMCal 
  case kSimulationNoEMCal:
    SimulationDefault(sim);
    year = atoi(gSystem->Getenv("CONFIG_YEAR"));
    if (year < 2015) sim.SetMakeSDigits("TRD TOF PHOS HMPID MUON ZDC PMD T0 VZERO FMD");
    else             sim.SetMakeSDigits("TRD TOF PHOS HMPID MUON ZDC PMD T0 VZERO FMD AD");
    return;

  }

}

void SimulationDefault(AliSimulation &sim)
{

  Int_t year = atoi(gSystem->Getenv("CONFIG_YEAR"));
  
  //
  // set OCDB source
  TString ocdbConfig = "default,snapshot";
  if (gSystem->Getenv("CONFIG_OCDB"))
    ocdbConfig = gSystem->Getenv("CONFIG_OCDB");
  if (ocdbConfig.Contains("alien") || ocdbConfig.Contains("cvmfs")) {
    // set OCDB 
    gROOT->LoadMacro("$ALIDPG_ROOT/MC/OCDBConfig.C");
    gROOT->ProcessLine("OCDBDefault(0);");
  }
  else {
    // set OCDB snapshot mode
    sim.SetCDBSnapshotMode("OCDBsim.root");
    AliCDBManager *cdbm = AliCDBManager::Instance();
    cdbm->SetDefaultStorage("local://");
    //    cdbm->SetSnapshotMode("OCDBsim.root");
  }

  //
  //
  if (year < 2015) sim.SetMakeSDigits("TRD TOF PHOS HMPID EMCAL MUON ZDC PMD T0 VZERO FMD");
  else             sim.SetMakeSDigits("TRD TOF PHOS HMPID EMCAL MUON ZDC PMD T0 VZERO FMD AD");
  sim.SetMakeDigitsFromHits("ITS TPC");

  //
  // HLT settings
  TString hltConfig = "auto";
  if (gSystem->Getenv("CONFIG_HLT"))
    hltConfig = gSystem->Getenv("CONFIG_HLT");
  sim.SetRunHLT(hltConfig.Data());

  //
  // material budget settings
  if (gSystem->Getenv("CONFIG_MATERIAL")) {
    Float_t material = atof(gSystem->Getenv("CONFIG_MATERIAL"));
    AliModule::SetDensityFactor(material);
  }

  //
  //
  SimulationConfigPHOS(sim);

  //
  //
  sim.UseVertexFromCDB();
  sim.UseMagFieldFromGRP();

  //
  //
  sim.SetRunQA(":");
  //
}

/*** PHOS ****************************************************/

void SimulationConfigPHOS(AliSimulation &sim)
{
  Int_t year = atoi(gSystem->Getenv("CONFIG_YEAR"));
  AliPHOSSimParam *simParam = AliPHOSSimParam::GetInstance();
  if (year < 2015) {
    simParam->SetCellNonLineairyA(0.001);
    simParam->SetCellNonLineairyB(0.2);
    simParam->SetCellNonLineairyC(1.02);
  }
  else {
    simParam->SetCellNonLinearity(kFALSE);
  }
}


/*** RUN3 ****************************************************/

void SimulationRun3(AliSimulation &sim)
{
  
  sim.SetRunHLT(""); // no HLT with Run3 sim, since it uses old ITS

  Int_t year = atoi(gSystem->Getenv("CONFIG_YEAR"));
  //
  int runNumber = atoi(gSystem->Getenv("DC_RUN"));
  SetCDBRun3(runNumber);
  //
  sim.SetMakeSDigits("TRD TOF PHOS HMPID EMCAL MUON ZDC");
  sim.SetMakeDigits("TRD TOF PHOS HMPID EMCAL MUON ZDC FIT");
  //  sim.SetMakeDigits("ALL");
  sim.SetMakeDigitsFromHits("ITS TPC");
   // material budget settings
  if (gSystem->Getenv("CONFIG_MATERIAL")) {
    Float_t material = atof(gSystem->Getenv("CONFIG_MATERIAL"));
    AliModule::SetDensityFactor(material);
  }
  //
  SimulationConfigPHOS(sim);
  //
  sim.UseVertexFromCDB();
  sim.UseMagFieldFromGRP();
  sim.SetUseDetectorsFromGRP(kTRUE);
  //
  sim.SetRunQA(":");
  //
  printf("Adding new detectors to GRP and suppressing HLT\n");
  AddDetToGRPRun3((AliDAQ::DetectorBits)(AliDAQ::kFIT), AliDAQ::kHLT, runNumber); // recreate GRP
  //AddDetToGRPRun3((AliDAQ::DetectorBits)(AliDAQ::kMFT | AliDAQ::kFIT), AliDAQ::kHLT, runNumber); // recreate GRP, removed MFT for the time being
  AliCDBManager* man = AliCDBManager::Instance();
  man->ClearCache();
  man->SetSpecificStorage("GRP/GRP/Data", "local://./");
}

/*******************************************************/

void AddDetToGRPRun3(AliDAQ::DetectorBits detAdd, AliDAQ::DetectorBits detRem, int run)
{
  // take raw GRP and the detector
  AliCDBManager* man = AliCDBManager::Instance();
  AliCDBEntry* grpe = man->Get("GRP/GRP/Data");
  AliGRPObject* grp = (AliGRPObject*)grpe->GetObject();
  UInt_t msk = grp->GetDetectorMask();
  if (detAdd) msk |= detAdd;
  if (detRem) msk &= ~detRem; 
  grp->SetDetectorMask(msk);
  int ndet = 0;
  for (int i=0;i<8*sizeof(msk);i++){ if( (msk>>i)&0x1 ) ndet++;}  
  grp->SetNumberOfDetectors(ndet);
  if (detRem|AliDAQ::kHLT) grp->SetHLTMode(AliGRPObject::kModeA);
  grpe->SetObject(0);
  AliCDBStorage* storage = man->GetStorage("local://./");
  AliCDBMetaData* md = new AliCDBMetaData();
  AliCDBId id("GRP/GRP/Data",run,run);
  man->Put(grp,id,md);
  //
}

/*******************************************************/

void SetCDBRun3(int run)
{
  // set OCDB source
  TString ocdbConfig = "default,snapshot";
  if (!gSystem->AccessPathName("OCDBsim.root")) {
    // set OCDB snapshot mode
    AliCDBManager *cdbm = AliCDBManager::Instance();
    cdbm->SetSnapshotMode("OCDBsim.root");
    cdbm->SetDefaultStorage("local://");
    //    cdbm->SetSnapshotMode("OCDBsim.root");
  }
  else if (gSystem->Getenv("CONFIG_OCDBCUSTOM")) {
    gROOT->LoadMacro("OCDBCustom.C");
    gROOT->ProcessLine("OCDBDefault(0);");
  }
  else {
    if (gSystem->Getenv("CONFIG_OCDB")) ocdbConfig = gSystem->Getenv("CONFIG_OCDB");
    if (ocdbConfig.Contains("alien") || ocdbConfig.Contains("cvmfs")) {
      // set OCDB 
      gROOT->LoadMacro("$ALIDPG_ROOT/MC/OCDBRun3.C");
      gROOT->ProcessLine("OCDBRun3(0);");
    }
  }
  AliCDBManager::Instance()->SetRun(run);
}
