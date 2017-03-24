AliGenerator *
GeneratorCustom()
{
  AliGenCocktail *ctl  = GeneratorCocktail("Perugia2011_HF");
  //
  Int_t process[2] = {kPythia6HeavyProcess_Charm, kPythia6HeavyProcess_Beauty};
  const Char_t *label[2] = {"cele PYTHIA", "bele PYTHIA"};
  Int_t iprocess = uidConfig % 2;
  AliGenerator *phf  = GeneratorPythia6Heavy(process[iprocess], kPythia6HeavyDecay_Electron, kPythia6Tune_Perugia2011, kFALSE);
  ctl->AddGenerator(phf, label[iprocess], 1.);
  printf(">>>>> added HF generator %s \n", label[iprocess]);
  // add pi0 and eta enhancement
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
  //
  return ctl;
}
