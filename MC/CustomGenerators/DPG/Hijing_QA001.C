AliGenerator *
GeneratorCustom()
{
  AliGenCocktail *ctl    = GeneratorCocktail("Hijing_QA001");
  AliGenerator   *hij    = GeneratorHijing();
  AliGenerator   *injpip = GeneratorInjector(10,  211, 10., 110., -1.0, 1.0);
  AliGenerator   *injpim = GeneratorInjector(10, -211, 10., 110., -1.0, 1.0);
  AliGenerator   *injkap = GeneratorInjector(5,   321, 10., 110., -1.0, 1.0);
  AliGenerator   *injkam = GeneratorInjector(5,  -321, 10., 110., -1.0, 1.0);
  AliGenerator   *injprp = GeneratorInjector(1,  2212, 10., 110., -1.0, 1.0);
  AliGenerator   *injprm = GeneratorInjector(1, -2212, 10., 110., -1.0, 1.0);
  ctl->AddGenerator(hij,    "Hijing",         1.);
  ctl->AddGenerator(injpip, "Injector (pip)", 1.);
  ctl->AddGenerator(injpim, "Injector (pim)", 1.);
  ctl->AddGenerator(injkap, "Injector (kap)", 1.);
  ctl->AddGenerator(injkam, "Injector (kam)", 1.);
  ctl->AddGenerator(injprp, "Injector (prp)", 1.);
  ctl->AddGenerator(injprm, "Injector (prm)", 1.);
  return ctl;
}
