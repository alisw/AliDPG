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
  kSimulationCustom,
  kNSimulations
};

const Char_t *SimulationName[kNSimulations] = {
  "Default",
  "Custom"
};


/*****************************************************************/

SimulationConfig(AliSimulation &sim, ESimulation_t tag, Int_t run)
{


  switch(tag) {

    // Default
  case kSimulationDefault:
    //
    // set OCDB snapshot mode
    //    AliCDBManager *man = AliCDBManager::Instance();
    //    man->SetDefaultStorage(Raw);
    //    man->SetRun(runNumber);
    //    man->SetSnapshotMode("OCDBsim.root");
    sim.SetCDBSnapshotMode("OCDBsim.root");
    //
    if (run < 222222)
      {
	sim.SetMakeSDigits("TRD TOF PHOS HMPID EMCAL MUON ZDC PMD T0 VZERO FMD");
      }
    else
      {
	sim.SetMakeSDigits("TRD TOF PHOS HMPID EMCAL MUON ZDC PMD T0 VZERO FMD AD");
      }
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

/*** ITS ****************************************************/

SimulationConfigITS(AliSimulation &sim, Int_t run)
{
  //  sim.SetSpecificStorage("ITS/Align/Data", Ideal);
  //  sim.SetSpecificStorage("ITS/Calib/SPDSparseDead", Full); // only for PbPb ?
}

/*** TPC ****************************************************/

SimulationConfigTPC(AliSimulation &sim, Int_t run)
{
  //  sim.SetSpecificStorage("TPC/Calib/Parameters",   Residual);
  //  sim.SetSpecificStorage("TPC/Calib/ClusterParam", Residual);
  //  sim.SetSpecificStorage("TPC/Calib/RecoParam",    Full);
  //  sim.SetSpecificStorage("TPC/Calib/TimeGain",     Ideal);
  //  sim.SetSpecificStorage("TPC/Calib/Correction",   Ideal);
  //  sim.SetSpecificStorage("TPC/Align/Data",         Ideal);
  //  sim.SetSpecificStorage("TPC/Calib/TimeDrift",    Ideal);
}

/*** MUON ****************************************************/

SimulationConfigMUON(AliSimulation &sim, Int_t run)
{
  //  sim.SetSpecificStorage("MUON/Align/Data", Ideal);
}

/*** ZDC ****************************************************/

SimulationConfigZDC(AliSimulation &sim, Int_t run)
{
  //  sim.SetSpecificStorage("ZDC/Align/Data", Ideal);
  //  sim.SetSpecificStorage("ZDC/Calib/Pedestals", Ideal); // only for PbPb ?
}

/*** PHOS ****************************************************/

SimulationConfigPHOS(AliSimulation &sim, Int_t run)
{
  AliPHOSSimParam *simParam = AliPHOSSimParam::GetInstance();
  simParam->SetCellNonLineairyA(0.001);
  simParam->SetCellNonLineairyB(0.2);
  simParam->SetCellNonLineairyC(1.02);
}
