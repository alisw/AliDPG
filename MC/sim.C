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
  if (WriteXsection_Pythia6()) return;
  if (WriteXsection_Pythia8()) return;
}

Bool_t
WriteXsection_Pythia6()
{

  TPythia6 *pythia = TPythia6::Instance();
  if (!pythia) return kFALSE;
  pythia->Pystat(1);
  Double_t xsection = pythia->GetPARI(1);
  if (xsection < 1.e-30) return kFALSE;
  UInt_t ntrials = pythia->GetMSTI(5);
  TFile *file = new TFile("pyxsec.root","recreate");
  TTree *tree = new TTree("Xsection","Pythia cross section");
  TBranch *branch = tree->Branch("xsection", &xsection, "X/D");
  TBranch *branch = tree->Branch("ntrials" , &ntrials , "X/i");
  tree->Fill();
  tree->Write();
  file->Close();
  cout << "Pythia6 cross section: " << xsection
       << ", number of trials: " << ntrials << endl;
  return kTRUE;
}

Bool_t
WriteXsection_Pythia8()
{

  AliPythia8 *pythia = AliPythia8::Instance();
  if (!pythia) return kFALSE;
  pythia->Pystat(1);
  Double_t xsection = pythia->GetXSection();
  if (xsection < 1.e-30) return kFALSE;
  UInt_t ntrials = 0; // fix me please!
  TFile *file = new TFile("pyxsec.root","recreate");
  TTree *tree = new TTree("Xsection","Pythia cross section");
  TBranch *branch = tree->Branch("xsection", &xsection, "X/D");
  TBranch *branch = tree->Branch("ntrials" , &ntrials , "X/i");
  tree->Fill();
  tree->Write();
  file->Close();
  cout << "Pythia8 cross section: " << xsection
       << ", number of trials: " << ntrials << endl;
  return kTRUE;
}
