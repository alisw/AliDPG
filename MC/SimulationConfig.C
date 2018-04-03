/*
 * AliDPG - ALICE Experiment Data Preparation Group
 * Simulation configuration script
 *
 */

/*****************************************************************/
/*****************************************************************/
/*****************************************************************/

enum ESimulation_t {
  kSimulationDefault,
  kSimulationMuon,
  kSimulationMuonOnly,
  kSimulationPhosOnly,
  kSimulationEmbedBkg,
  kSimulationEmbedSig,
  kSimulationGeneratorOnly,
  kSimulationNoDigitization,
  kSimulationCustom,
  kSimulationDefaultIonTail,
  kSimulationNoTPC,
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
  "Custom",
  "SimulationDefaultIonTail",
  "NoTPC"
};


/*****************************************************************/

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

    // Custom
  case kSimulationCustom:
    if ((gROOT->LoadMacro("SimulationCustom.C")) != 0) {
      printf("ERROR: cannot find SimulationCustom.C\n");
      abort();
      return;
    }
    SimulationCustom(sim);
    return;

   // Default simulation enabling IonTail/Crosstalk for TPC
  case kSimulationDefaultIonTail:
      SimulationDefault(sim);
      Int_t year = atoi(gSystem->Getenv("CONFIG_YEAR"));
      if (year < 2015) sim.SetMakeSDigits("TPC TRD TOF PHOS HMPID EMCAL MUON ZDC PMD T0 VZERO FMD");
      else             sim.SetMakeSDigits("TPC TRD TOF PHOS HMPID EMCAL MUON ZDC PMD T0 VZERO FMD AD");
      sim.SetMakeDigitsFromHits("ITS");
      return;

   // Default simulation without TPC TPC
  case kSimulationNoTPC:
    SimulationDefault(sim);
    sim.SetRunHLT("");
    sim.SetMakeDigitsFromHits("ITS");
    // sim.SetMakeDigits("ALL -TPC");  // this line does not work, the Digits tree will anyway be created also with this line. Without this line, an empty digits tree for the TPC will be created
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
    OCDBDefault(0);
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
    sim.AliModule::SetDensityFactor(material);
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
