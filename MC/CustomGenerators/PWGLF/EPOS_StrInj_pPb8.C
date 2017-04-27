AliGenerator *GeneratorCustom(){

  int ninjxi = 3;
  int ninjom = 3;
  
  Int_t sign = uidConfig % 2 == 0 ? 1 : -1;
  
  AliGenCocktail *ctl  = GeneratorCocktail("EPOS_StrInj_pPb8");
  AliGenerator   *epos  = GeneratorEPOSLHC();
  
  AliGenerator   *ixi = GeneratorInjector(ninjxi, sign * 3312, 2., 20., -1.2, 1.2);
  AliGenerator   *iom = GeneratorInjector(ninjom, (-1) * sign * 3334, 0., 10., -1.2, 1.2);
  
  ctl->AddGenerator(epos,  "Epos", 1.);
  ctl->AddGenerator(ixi, "Injector (Xi)", 1.);
  ctl->AddGenerator(iom, "Injector (Omega)", 1.);

  return ctl;

}
