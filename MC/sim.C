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
  AliSimulation sim("$ALIDPG_ROOT/MC/Config.C");

  /* configuration */
  SimulationConfig(sim, simulationConfig);

  /* run */
  sim.Run(nev);

  /* write cross-section */
  WriteXsection();

}

WriteXsection()
{

  TPythia6 *pythia = TPythia6::Instance();
  if (!pythia) return;
  pythia->Pystat(1);
  Double_t xsection = pythia->GetPARI(1);
  UInt_t ntrials = pythia->GetMSTI(5);
  TFile *file = new TFile("pyxsec.root","recreate");
  TTree *tree = new TTree("Xsection","Pythia cross section");
  TBranch *branch = tree->Branch("xsection", &xsection, "X/D");
  TBranch *branch = tree->Branch("ntrials" , &ntrials , "X/i");
  tree->Fill();
  tree->Write();
  file->Close();
  cout << "Pythia cross section: " << xsection
       << ", number of trials: " << ntrials << endl;

}
