AliGenerator *
GeneratorCustom(TString opt)
{
  TString optList[3] = {"a", "b", "c"};
  Int_t ninjlist[3] = {25, 7, 3};
  Int_t ninj = 1;
  for (Int_t iopt = 0; iopt < 3; iopt++)
    if (opt.EqualTo(optList[iopt]))
      ninj = ninjlist[iopt];
  AliGenCocktail *ctl  = GeneratorCocktail("Hijing_Rsn002");
  AliGenerator   *hij  = GeneratorHijing();
  AliGenerator   *inj1 = GeneratorInjector(ninj,  3124, 0., 10., -0.6, 0.6);
  AliGenerator   *inj2 = GeneratorInjector(ninj, -3124, 0., 10., -0.6, 0.6);
  ctl->AddGenerator(hij,  "Hijing",            1.);
  ctl->AddGenerator(inj1, "Injector (Rsn002)", 1.);
  ctl->AddGenerator(inj2, "Injector (Rsn002)", 1.);
  return ctl;
}
