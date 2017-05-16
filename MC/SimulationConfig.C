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
  kSimulationEmbedBkg,
  kSimulationEmbedSig,
  kSimulationCustom,
  kNSimulations
};

const Char_t *SimulationName[kNSimulations] = {
  "Default",
  "Muon",
  "EmbedBkg",
  "EmbedSig",
  "Custom"
};


/*****************************************************************/

SimulationConfig(AliSimulation &sim, ESimulation_t tag)
{
  
  printf(">>>>> SimulationConfig: %s \n", SimulationName[tag]);
  
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
    
    // Custom
  case kSimulationCustom:
    if ((gROOT->LoadMacro("SimulationCustom.C")) != 0) {
      printf("ERROR: cannot find SimulationCustom.C\n");
      abort();
      return;
    }
    SimulationCustom(sim);
    return;

  }
  
}

SimulationDefault(AliSimulation &sim)
{

  Int_t year = atoi(gSystem->Getenv("CONFIG_YEAR"));
  
  //
  // set OCDB source
  TString ocdbConfig = "default,snapshot";
  if (gSystem->Getenv("CONFIG_OCDB"))
    ocdbConfig = gSystem->Getenv("CONFIG_OCDB");
  if (ocdbConfig.Contains("alien")) {
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

SimulationConfigPHOS(AliSimulation &sim)
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
