AliGenerator *
GeneratorCustom()
{
  AliGenCocktail *ctl  = GeneratorCocktail("Hijing_XeXe_Nuclei");
  AliGenerator   *hij  = GeneratorHijing();
  AliGenerator   *iom  = GeneratorInjector(1, 3334, 0.5, 7., -0.7, 0.7);
  AliGenerator   *iaom = GeneratorInjector(1, -3334, 0.5, 7., -0.7, 0.7);
  ctl->AddGenerator(hij,  "Hijing",           1.);
  ctl->AddGenerator(iom,  "Injector (Omega)", 1.);
  ctl->AddGenerator(iaom, "Injector (AntiOmega)", 1.);
  AliGenerator   *nu1a  = Generator_Nuclex(0x3, kFALSE, 2,6.f,0.82f);
  AliGenerator   *nu1b  = Generator_Nuclex(0x3, kTRUE,  2,6.f,0.82f);
  ctl->AddGenerator(nu1a,  "Nuclex1a", 1.);
  ctl->AddGenerator(nu1b,  "Nuclex1b", 1.);
  return ctl;
}
