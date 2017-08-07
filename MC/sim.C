/*
 * AliDPG - ALICE Experiment Data Preparation Group
 * Simulation steering script
 *
 */

/*****************************************************************/
/*****************************************************************/
/*****************************************************************/

void sim() 
{

  // number of events configuration
  Int_t nev = 200;
  if (gSystem->Getenv("CONFIG_NEVENTS"))
    nev = atoi(gSystem->Getenv("CONFIG_NEVENTS"));

  // simulation configuration
  gROOT->LoadMacro("$ALIDPG_ROOT/MC/SimulationConfig.C");
  Int_t simulationConfig = kSimulationDefault;
  if (gSystem->Getenv("CONFIG_SIMULATION")) {
    Bool_t valid = kFALSE;
    for (Int_t isim = 0; isim < kNSimulations; isim++)
      if (strcmp(gSystem->Getenv("CONFIG_SIMULATION"), SimulationName[isim]) == 0) {
        simulationConfig = isim;
        valid = kTRUE;
        break;
      }
    if (!valid) {
      printf(">>>>> Unknown simulation configuration: %s \n", gSystem->Getenv("CONFIG_SIMULATION"));
      abort();
    }
  }

  /* initialisation */
  Int_t error;
  TString config_macro = "$ALIDPG_ROOT/MC/Config.C";
  if (gROOT->LoadMacro("Config.C", &error, kTRUE) == 0) {
    printf(">>>>> Config.C macro detected in CWD, using that one \n");
    config_macro = "Config.C";
  }
  AliSimulation sim(config_macro.Data());

  /* configuration */
  SimulationConfig(sim, simulationConfig);

  /* run */
  sim.Run(nev);

}

