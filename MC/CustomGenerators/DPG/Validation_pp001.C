AliGenerator *
GeneratorCustom()
{
  //
  AliGenCocktail *ctl = (AliGenCocktail*) GeneratorCocktail("GEANT4_validation");
  //
  // Pythia8 Monash 2013
  AliGenerator   *py8 = GeneratorPythia8(kPythia8Tune_Monash2013);
  ctl->AddGenerator(py8, "Pythia8 (Monash2013)", 1.);
  //
  // J/psi -> ee
  AliGenerator  *jpsi = Generator_Jpsiee("pp 8", 1.0, 0.0, 0.0, 0.0);
  ctl->AddGenerator(jpsi, "Jpsi2ee", 1., NULL, 1);
  //
  // HF
  AddGeneratorHF(ctl);

  return ctl;
}

AddGeneratorHF(AliGenCocktail *ctl)
{  
  Int_t process[2] = {kPythia6HeavyProcess_Charm, kPythia6HeavyProcess_Beauty};
  Int_t decay[2]   = {kPythia6HeavyDecay_Hadrons, kPythia6HeavyDecay_Electron};
  const Char_t *label[2][2] = {
    "chadr PYTHIA", "cele PYTHIA",
    "bchadr PYTHIA", "bele PYTHIA"
  };
  Int_t iprocess = (uidConfig % 4) / 2;
  Int_t idecay   = (uidConfig % 4) % 2;
  AliGenerator *phf  = GeneratorPythia6Heavy(process[iprocess], decay[idecay], kPythia6Tune_Perugia2011, kFALSE);
  ctl->AddGenerator(phf, label[iprocess][idecay], 1.);
  printf(">>>>> added HF generator %s \n", label[iprocess][idecay]);
  // add pi0 and eta enhancement
  if (idecay == kPythia6HeavyDecay_Electron) {
    AliGenPHOSlib *plib = new AliGenPHOSlib();
    AliGenParam *pi0 = new AliGenParam(2, plib, AliGenPHOSlib::kPi0Flat);
    pi0->SetPhiRange(0., 360.) ;
    pi0->SetYRange(-1.2, 1.2) ;
    pi0->SetPtRange(0., 50.) ;
    ctl->AddGenerator(pi0,  "pi0", 1.);
    AliGenParam *eta = new AliGenParam(1, plib, AliGenPHOSlib::kEtaFlat);
    eta->SetPhiRange(0., 360.) ;
    eta->SetYRange(-1.2, 1.2) ;
    eta->SetPtRange(0., 50.) ;
    ctl->AddGenerator(eta,  "eta", 1.);
  }
}
