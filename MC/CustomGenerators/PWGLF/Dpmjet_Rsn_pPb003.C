AliGenerator *GeneratorCustom(TString opt = "")
{
  // Custom generator to inject f0, f1 and f2
  // Options:
  // pPb:  flat pT distibution in a range 0-20 GeV/c; 4 particles per event;
  // rapidity range: -0.6 < y < 0.1 : Anchored to period LHC16q

  Int_t ninj = 4; // 4 injected particles per species

  // Set pT range (pT_L, pT_H)
  Double_t  pT_L = 0.0;
  Double_t  pT_H = 20.0;

  // Set Rapidity
  Double_t y_L = -0.6;
  Double_t y_H = 0.1;

  AliGenCocktail *ctl  = (AliGenCocktail*) GeneratorCocktail("DPMJET_RsnInject");
  AliGenDPMjet *dpmjet  = (AliGenDPMjet*) GeneratorPhojet();
  ctl->AddGenerator(dpmjet,"Dpmjet",           1.);

  Double_t randm = gRandom->Rndm();

  if ( randm < 0.2) {
    AliGenerator *injf0_1710_pPb     = GeneratorInjector(ninj, 10331, pT_L, pT_H,  y_L, y_H); // f0(1710)
    ctl->AddGenerator(injf0_1710_pPb,"Injector(Rsnf0_1710)", 1.);
  }
  else if ( randm >= 0.2 && randm < 0.4) {
    AliGenerator *injf2_1525_pPb     = GeneratorInjector(ninj,    335, pT_L, pT_H,  y_L, y_H); // f2(1525)
    ctl->AddGenerator(injf2_1525_pPb,"Injector(Rsnf2_1525)", 1.);
  }
  else if ( randm >= 0.4 && randm < 0.6) {
    AliGenerator *injf1_1285_pPb     = GeneratorInjector(ninj,    20223, pT_L, pT_H,  y_L, y_H); // f1(1285)
    ctl->AddGenerator(injf1_1285_pPb,"Injector(Rsnf1_1285)", 1.);
  }
  else if ( randm >= 0.6 && randm < 0.8) {
    AliGenerator *injf1_1420_pPb     = GeneratorInjector(ninj,    20333, pT_L, pT_H,  y_L, y_H); // f1(1420)
    ctl->AddGenerator(injf1_1420_pPb,"Injector(Rsnf1_1420)", 1.);
  }
  else {
    AliGenerator *injf0_1500_pPb     = GeneratorInjector(ninj,    90302, pT_L, pT_H,  y_L, y_H); // f0(1500)
    ctl->AddGenerator(injf0_1500_pPb,"Injector(Rsnf2)", 1.);
  }

  return ctl;

}
