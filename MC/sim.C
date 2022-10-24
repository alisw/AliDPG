/*
 * AliDPG - ALICE Experiment Data Preparation Group
 * Simulation steering script
 *
 */

/*****************************************************************/
/*****************************************************************/
/*****************************************************************/

#if (!defined(__CLING__) && !defined(__CINT__)) || defined(__ROOTCLING__) || defined(__ROOTCINT__)
#include "TSystem.h"
#include "TROOT.h"
#include "AliSimulation.h"
#endif

#if ROOT_VERSION_CODE >= ROOT_VERSION(6,0,0)
#include "SimulationConfig.C"
#endif

void sim() 
{

  // number of events configuration
  Int_t nev = 200;
  if (gSystem->Getenv("CONFIG_NEVENTS"))
    nev = atoi(gSystem->Getenv("CONFIG_NEVENTS"));
  
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,0,0)
  // in root5 the ROOT_VERSION_CODE is defined only in ACLic mode
#else
  gROOT->LoadMacro("$ALIDPG_ROOT/MC/SimulationConfig.C");
#endif
  
  // simulation configuration
  ESimulation_t simulationConfig = kSimulationDefault;
  if (gSystem->Getenv("CONFIG_SIMULATION")) {
    Bool_t valid = kFALSE;
    for (Int_t isim = 0; isim < kNSimulations; isim++)
      if (strcmp(gSystem->Getenv("CONFIG_SIMULATION"), SimulationName[isim]) == 0) {
        simulationConfig = (ESimulation_t) isim;
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
  if (gROOT->LoadMacro(Form("%s/Config.C", gSystem->pwd()), &error, kTRUE) == 0) {
    printf(">>>>> Config.C macro detected in CWD, using that one \n");
    config_macro = Form("%s/Config.C", gSystem->pwd());
  }
  gROOT->LoadMacro("$ALIDPG_ROOT/MC/Config_LoadLibraries.C");
  gROOT->ProcessLine("Config_LoadLibraries();");
  AliSimulation sim(config_macro.Data());
  
  if (gSystem->Getenv("CONFIG_DETECTOR")) {
    if ((strcmp(gSystem->Getenv("CONFIG_DETECTOR"), "FOCAL") == 0) ||
        (strcmp(gSystem->Getenv("CONFIG_DETECTOR"), "FOCALnoFIT") == 0)) {
      printf(">>>>> sim.C: No align data from CDB when running FOCAL !!!!!!!!!!!!!!! \n");  
      sim.SetLoadAlignFromCDB(kFALSE);
      sim.SetUseDetectorsFromGRP(kFALSE);
    }
  }

  /* configuration */
  SimulationConfig(sim, simulationConfig);

  /* run */
  sim.Run(nev);

}

