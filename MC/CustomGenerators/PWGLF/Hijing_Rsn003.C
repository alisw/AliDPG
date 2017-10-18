AliGenerator *GeneratorCustom(TString opt = "")
{
  //
  // Custom generator to inject phi,K*0,antiK*0 resonances flat in pT. Options:
  // Central     : a - injection flat in pT up 25 GeV/c, 10 phi, 10 K*0 and 10 antiK*0
  // Semi-Central: b - injection flat in pT up 25 GeV/c,  6 phi,  6 K*0 and  6 antiK*0
  // Peripheral  : c - injection flat in pT up 20 GeV/c,  3 phi,  3 K*0 and  3 antiK*0
  //
  TString optList[3] = {"a", "b", "c"};
  Int_t ninjlist[3]  = {10, 6, 3};
  Int_t ninj = 1;
  
  Double_t pTmax = 25.0;
  if(opt.EqualTo(optList[0]) || opt.EqualTo(optList[1])) pTmax = 25.0;
  else if (opt.EqualTo(optList[2])) pTmax = 20.0;
  
  for (Int_t iopt = 0; iopt < 3; iopt++)
    if (opt.EqualTo(optList[iopt]))
      ninj = ninjlist[iopt];
  
  AliGenCocktail *ctl  = GeneratorCocktail("Hijing_Rsn003");
  AliGenerator   *hij  = GeneratorHijing();
  AliGenerator   *inj1 = GeneratorInjector(ninj,  333, 0., pTmax, -0.6, 0.6);
  AliGenerator   *inj2 = GeneratorInjector(ninj,  313, 0., pTmax, -0.6, 0.6);
  AliGenerator   *inj3 = GeneratorInjector(ninj, -313, 0., pTmax, -0.6, 0.6);
  
  ctl->AddGenerator(hij,  "Hijing",            1.);
  ctl->AddGenerator(inj1, "Injector (RsnPhi)", 1.);
  ctl->AddGenerator(inj2, "Injector (RsnKSt0)", 1.);
  ctl->AddGenerator(inj3, "Injector (RsnAKst0)", 1.);
  return ctl;
}
