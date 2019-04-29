AliGenerator *GeneratorCustom(){

  Int_t sign = uidConfig % 2 == 0 ? 1 : -1;
  
  AliGenCocktail *ctl = GeneratorCocktail("Pythia8_StrInj_pp5_2017");
  
  AliGenerator   *py8  = GeneratorPythia8(kPythia8Tune_Monash2013);
  AliGenerator   *ila  = GeneratorInjector(1, sign * 3122, 0.0, 8.0, -0.7, 0.7);
  AliGenerator   *ixi  = GeneratorInjector(1, sign * 3312, 0.4, 8.0, -0.7, 0.7);
  AliGenerator   *iomp = GeneratorInjector(1, -3334, 0.5, 6.5, -0.7, 0.7);
  AliGenerator   *iomm = GeneratorInjector(1,  3334, 0.5, 6.5, -0.7, 0.7);
  
  ctl->AddGenerator(py8 , "Pythia8 (Monash2013)", 1.);
  ctl->AddGenerator(ila , "Injector (Lambda)", 1.);
  ctl->AddGenerator(ixi , "Injector (Xi)", 1.);
  ctl->AddGenerator(iomp, "Injector (Omega+)", 1.);
  ctl->AddGenerator(iomm, "Injector (Omega-)", 1.);

  return ctl;

}
