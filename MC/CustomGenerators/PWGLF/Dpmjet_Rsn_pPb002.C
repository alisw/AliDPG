AliGenerator *GeneratorCustom(TString opt = "")
{
  // Custom generator to inject f0 and f2
  // Options:
  // pPb:  flat pT distibution in a range 0-15 GeV/c; 4 particles per event;
  // rapidity range: -0.6 < y < 0.1 : Anchored to period LHC16q

  Int_t ninj = 4; // 4 injected particles per species

  // Set pT range (pT_L, pT_H)
  Double_t  pT_L = 0.0;
  Double_t  pT_H = 15.0;

  // Set Rapidity
  Double_t y_L = -0.6;
  Double_t y_H = 0.1;

  AliGenCocktail *ctl  = (AliGenCocktail*) GeneratorCocktail("DPMJET_RsnInject");
  AliGenDPMjet *dpmjet  = (AliGenDPMjet*) GeneratorPhojet();
  ctl->AddGenerator(dpmjet,"Dpmjet",           1.);

  Double_t randm = gRandom->Rndm();

  if ( randm < 0.5) {
    AliGenerator *injf0_pPb     = GeneratorInjector(ninj,9010221, pT_L, pT_H,  y_L, y_H); // f0(980)
    ctl->AddGenerator(injf0_pPb,"Injector(Rsnf0)", 1.);
  }
  else {
    AliGenerator *injf2_pPb     = GeneratorInjector(ninj,    225, pT_L, pT_H,  y_L, y_H); // f2(1270)
    ctl->AddGenerator(injf2_pPb,"Injector(Rsnf2)", 1.);
  }

  return ctl;

}
