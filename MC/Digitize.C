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

void Digitize() 
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

  AliGeomManager::LoadGeometry("geometry.root");
  AliRunLoader* rl = AliRunLoader::Open("galice.root");
  rl->LoadgAlice();
  AliRunLoader::Instance()->CdGAFile();
  gAlice->InitLoaders();
  AliRunLoader::Instance()->LoadKinematics();
  AliRunLoader::Instance()->LoadTrackRefs();
  AliRunLoader::Instance()->LoadHits();

   
  AliSimulation sim(config_macro.Data());
  /* configuration */
  SimulationConfig(sim, simulationConfig);

  AliCDBManager::Instance()->SetSnapshotMode("OCDBsim.root");
  AliCDBManager::Instance()->SetRun(atoi(gSystem->Getenv("CONFIG_RUN")));
  AliGRPManager grpM;
  grpM.ReadGRPEntry();
  grpM.SetMagField();
  
  sim.SetRunGeneration(false);
  sim.SetRunSimulation(false);
  sim.Run(nev);

  
  // TString sdig = "TRD TOF PHOS HMPID EMCAL MUON ZDC PMD T0 VZERO FMD AD";
  // TString h2dig = "ITS TPC";
  // sim.RunSDigitization(sdig.Data());
  // sim.RunDigitization("ALL", h2dig.Data());
  // sim.RunHitsDigitization(h2dig.Data());
  // sim.RunTrigger();
  // sim.RunHLT();
}

