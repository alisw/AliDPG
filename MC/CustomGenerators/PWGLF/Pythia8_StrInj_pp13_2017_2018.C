AliGenerator *GeneratorCustom(){

  Int_t sign = uidConfig % 2 == 0 ? 1 : -1;
  
  AliGenCocktail *ctl = (AliGenCocktail*) GeneratorCocktail("Pythia8_StrInj_pp13_2017_2018");
  
  AliGenerator   *py8  = GeneratorPythia8(kPythia8Tune_Monash2013);
  AliGenerator   *ixi  = GeneratorInjector(1, sign * 3312, 0.4, 8.0, -0.7, 0.7);
  AliGenerator   *iom = GeneratorInjector(1, sign * 3334, 0.5, 6.5, -0.7, 0.7);
  
  ctl->AddGenerator(py8 , "Pythia8 (Monash2013)", 1.);
  ctl->AddGenerator(ixi , "Injector (Xi)", 1.);
  ctl->AddGenerator(iom, "Injector (Omega)", 1.);
  
  return ctl;

}
