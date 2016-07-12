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
  kSimulationCustom,
  kNSimulations
};

const Char_t *SimulationName[kNSimulations] = {
  "Default",
  "Muon",
  "Custom"
};


/*****************************************************************/

SimulationConfig(AliSimulation &sim, ESimulation_t tag, Int_t run)
{
  
  
  switch(tag) {
    
    // Default
  case kSimulationDefault:
    SimulationDefault(sim, run);
    return;
    
    // Muon
  case kSimulationMuon:
    SimulationDefault(sim, run);
    sim.SetMakeSDigits("MUON VZERO");
    sim.SetMakeDigitsFromHits("ITS");
    return;
    
    // Custom
  case kSimulationCustom:
    if ((gROOT->LoadMacro("SimulationCustom.C")) != 0) {
      printf("ERROR: cannot find SimulationCustom.C\n");
      abort();
      return;
    }
    SimulationCustom(sim, run);
    return;

  }
  
}

SimulationDefault(AliSimulation &sim, Int_t run)
{

  gROOT->LoadMacro("$ALIDPG_ROOT/MC/Utils.C");
  Int_t year = RunToYear(run);
  
  //
  // set OCDB source
  TString ocdbConfig = "default,snapshot";
  if (gSystem->Getenv("CONFIG_OCDB"))
    ocdbConfig = gSystem->Getenv("CONFIG_OCDB");
  if (ocdbConfig.Contains("alien")) {
    // set OCDB 
    gROOT->LoadMacro("$ALIDPG_ROOT/MC/OCDBConfig.C");
    OCDBDefault(run, 0);
  }
  else {
    // set OCDB snapshot mode
    sim.SetCDBSnapshotMode("OCDBsim.root");
    //    AliCDBManager *cdbm = AliCDBManager::Instance();
    //    cdbm->SetSnapshotMode("OCDBsim.root");
  }

  //
  if (year < 2015) sim.SetMakeSDigits("TRD TOF PHOS HMPID EMCAL MUON ZDC PMD T0 VZERO FMD");
  else             sim.SetMakeSDigits("TRD TOF PHOS HMPID EMCAL MUON ZDC PMD T0 VZERO FMD AD");
  sim.SetMakeDigitsFromHits("ITS TPC");
  //  sim.SetRunHLT(""); // can't detect from GRP if HLT was running, off for safety now 
  //
  SimulationConfigPHOS(sim, run);
  //
  sim.UseVertexFromCDB();
  sim.UseMagFieldFromGRP();
  //
  sim.SetRunQA(":");
  //
}

/*** PHOS ****************************************************/

SimulationConfigPHOS(AliSimulation &sim, Int_t run)
{
  AliPHOSSimParam *simParam = AliPHOSSimParam::GetInstance();
  simParam->SetCellNonLineairyA(0.001);
  simParam->SetCellNonLineairyB(0.2);
  simParam->SetCellNonLineairyC(1.02);
}
