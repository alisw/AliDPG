AliGenerator *
GeneratorCustom()
{
  AliGenCocktail *ctl  = (AliGenCocktail*) GeneratorCocktail("Hijing_XeXe_Omega");
  AliGenerator   *hij  = GeneratorHijing();
  AliGenerator   *iom  = GeneratorInjector(1, 3334, 0.5, 7., -0.7, 0.7);
  AliGenerator   *iaom = GeneratorInjector(1, -3334, 0.5, 7., -0.7, 0.7);
  ctl->AddGenerator(hij,  "Hijing",           1.);
  ctl->AddGenerator(iom,  "Injector (Omega)", 1.);
  ctl->AddGenerator(iaom, "Injector (AntiOmega)", 1.);
  return ctl;
}
