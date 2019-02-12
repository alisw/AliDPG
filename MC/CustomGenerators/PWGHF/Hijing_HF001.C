AliGenerator *
GeneratorCustom(TString opt = "")
{
  AliGenCocktail *ctl  = GeneratorCocktail("Hijing_HF");
  AliGenerator   *hij  = GeneratorHijing();
  ctl->AddGenerator(hij, "Hijing", 1.);

  const Char_t *label[2][4] = {
    "chadr PYTHIA", "chadr PYTHIA", "cele PYTHIA", "chadr PYTHIA", 
    "bchadr PYTHIA", "bchadr PYTHIA", "bele PYTHIA", "bchadr PYTHIA"
  };
  //
  Int_t process[2] = {kPythia6HeavyProcess_Charm, kPythia6HeavyProcess_Beauty};
  Int_t iprocess = uidConfig % 2;
  //
  Int_t decay[6]   = {kPythia6HeavyDecay_Hadrons, kPythia6HeavyDecay_HadronsWithV0, kPythia6HeavyDecay_Electron, kPythia6HeavyDecay_HadronsWithV0, kPythia6HeavyDecay_HadronsWithV0, kPythia6HeavyDecay_Hadrons};
  TString optList[6] = {"had", "hv0", "ele", "LcK0Sp", "LcK0SpBDTsig", "LcpKpi"};
  Int_t idecay = 0;
  for (Int_t iopt = 0; iopt < 6; iopt++)
    if (opt.EqualTo(optList[iopt]))
      idecay = iopt;
  //
  AliGenerator *phf  = GeneratorPythia6Heavy(process[iprocess], decay[idecay], kPythia6Tune_Perugia2011);
  //
  TFormula *formula = new TFormula("Signals","max(1.,120.*(x<5.)+80.*(1.-x/20.)*(x>5.)*(x<11.)+240.*(1.-x/13.)*(x>11.))");
  //
  Float_t pth[4] = {2.76, 20., 50., 1000.};
  Int_t ipt;
  if      ((uidConfig / 2) % 10 < 7) ipt = 0;
  else if ((uidConfig / 2) % 10 < 9) ipt = 1;
  else                               ipt = 2;
  ((AliGenPythia *)phf)->SetPtHard(pth[ipt], pth[ipt + 1]);
  if(opt.EqualTo(optList[3])) {// Lc --> K0sp
    ((AliGenPythia *)phf)->SetTriggerParticle(4122, 999, 999, -1, 1000);
    ((AliGenPythia *)phf)->SetTriggerY(1.6);
    ((AliGenPythia *)phf)->SetHeavyQuarkYRange(-1.5,1.5);
    ((AliGenPythia *)phf)->SetForceDecay(AliDecayer::kLcpK0S);  //Force Lc decay mode in PYTHIA to Lc --> K0s+p
  }
  if(opt.EqualTo(optList[4])) {// Lc --> K0sp (modified for BDT signal)
    ((AliGenPythia *)phf)->SetTriggerParticle(4122, 999, 999, -1, 1000);
    ((AliGenPythia *)phf)->SetTriggerY(1.6);
    ((AliGenPythia *)phf)->SetHeavyQuarkYRange(-1.5,1.5);
    ((AliGenPythia *)phf)->SetForceDecay(AliDecayer::kLcpK0SBDTsig);  //Force Lc decay mode in PYTHIA to Lc --> K0s+p with forced K0->K0S->pi+pi-
  }
  if(opt.EqualTo(optList[5])) {// Lc --> pKpi
    ((AliGenPythia *)phf)->SetTriggerParticle(4122, 999, 999, -1, 1000);
    ((AliGenPythia *)phf)->SetTriggerY(1.6);
    ((AliGenPythia *)phf)->SetHeavyQuarkYRange(-1.5,1.5);
    ((AliGenPythia *)phf)->SetForceDecay(AliDecayer::kLcpKpi);  //Force Lc decay mode in PYTHIA to Lc->pKpi
  }
  ctl->AddGenerator(phf, label[iprocess][idecay], 1., formula);
  printf(">>>>> added HF generator %s \n", label[iprocess][idecay]);
  // add pi0 and eta enhancement
  if (decay[idecay] == kPythia6HeavyDecay_Electron) {
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
  }
  return ctl;
}
