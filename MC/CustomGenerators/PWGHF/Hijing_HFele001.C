AliGenerator *
GeneratorCustom()
{
  AliGenCocktail *ctl  = GeneratorCocktail("Hijing_HF");
  AliGenerator   *hij  = GeneratorHijing();
  ctl->AddGenerator(hij, "Hijing", 1.);
  //
  Int_t process[2] = {kPythia6HeavyProcess_Charm, kPythia6HeavyProcess_Beauty};
  const Char_t *label[2] = {"cele PYTHIA", "bele PYTHIA"};
  TFormula *formula = new TFormula("Signals", "max(1.,120.*(x<5.)+80.*(1.-x/20.)*(x>5.)*(x<11.)+240.*(1.-x/13.)*(x>11.))");
  Int_t iprocess = uidConfig % 2;
  AliGenerator *phf  = GeneratorPythia6Heavy(process[iprocess], kPythia6HeavyDecay_Electron, kPythia6Tune_Perugia2011);
  //
  Float_t pth[4] = {2.76, 20., 50., 1000.};
  Int_t ipt;
  if      ((uidConfig / 2) % 10 < 7) ipt = 0;
  else if ((uidConfig / 2) % 10 < 9) ipt = 1;
  else                               ipt = 2;
  ((AliGenPythia *)phf)->SetPtHard(pth[ipt], pth[ipt + 1]);
  ctl->AddGenerator(phf, label[iprocess], 1., formula);
  printf(">>>>> added HF generator %s \n", label[iprocess]);
  // add pi0 and eta enhancement
  TFormula* neutralsF = new TFormula("neutrals", "20.+ 80. * exp(- 0.5 * x * x / 5.12 / 5.12)");
  AliGenPHOSlib *plib = new AliGenPHOSlib();
  AliGenParam *pi0 = new AliGenParam(1, plib, AliGenPHOSlib::kPi0Flat);
  pi0->SetPhiRange(0., 360.) ;
  pi0->SetYRange(-1.2, 1.2) ;
  pi0->SetPtRange(0., 50.) ;
  ctl->AddGenerator(pi0,  "pi0", 1., neutralsF);
  AliGenParam *eta = new AliGenParam(1, plib, AliGenPHOSlib::kEtaFlat);
  eta->SetPhiRange(0., 360.) ;
  eta->SetYRange(-1.2, 1.2) ;
  eta->SetPtRange(0., 50.) ;
  ctl->AddGenerator(eta,  "eta", 1., neutralsF);
  return ctl;
}
