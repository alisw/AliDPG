AliGenerator *
GeneratorCustom()
{
  AliGenCocktail *ctl  = GeneratorCocktail("Hijing_Gamma001");
  AliGenerator   *hij  = GeneratorHijing();
  ctl->AddGenerator(hij,  "Hijing",         1.);
  // PCM
  TFormula* neutralsF  = new TFormula("neutrals",  "30. + 30. * exp(- 0.5 * x * x / 5.12 / 5.12)");
  AliGenerator   *pi0  = GeneratorInjector(1, 111, 0., 50., -1.2, 1.2);
  AliGenerator   *eta  = GeneratorInjector(1, 221, 0., 50., -1.2, 1.2);
  ctl->AddGenerator(pi0,  "Injector (pi0)", 1., neutralsF);
  ctl->AddGenerator(eta,  "Injector (eta)", 1., neutralsF);
  // PHOS
  AliGenerator   *pi0a = GeneratorInjector(1, 111, 0., 50., -0.155, 0.155, 240., 260.);
  AliGenerator   *pi0b = GeneratorInjector(1, 111, 0., 50., -0.155, 0.155, 260., 280.);
  AliGenerator   *pi0c = GeneratorInjector(1, 111, 0., 50., -0.155, 0.155, 280., 300.);
  AliGenerator   *pi0d = GeneratorInjector(1, 111, 0., 50., -0.155, 0.155, 300., 320.);
  AliGenerator   *etaa = GeneratorInjector(1, 221, 0., 50., -0.155, 0.155, 240., 320.);
  ctl->AddGenerator(pi0a, "Injector (pi0a)", 1.);
  ctl->AddGenerator(pi0b, "Injector (pi0b)", 1.);
  ctl->AddGenerator(pi0c, "Injector (pi0c)", 1.);
  ctl->AddGenerator(pi0d, "Injector (pi0d)", 1.);
  ctl->AddGenerator(etaa, "Injector (etaa)", 1.);
  return ctl;
}
