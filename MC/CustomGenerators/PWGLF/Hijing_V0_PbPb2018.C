AliGenerator *
GeneratorCustom( ) //(TString opt)
{
  TString optList[3] = {"a", "b", "c"};
  Int_t ninjk0[3] = {10, 5, 1};
  Int_t ninjla[3] = {10, 5, 1};
  Int_t iinj = 0;
  //for (Int_t iopt = 0; iopt < 3; iopt++)
  //  if (opt.EqualTo(optList[iopt]))
  //    iinj = iopt;
  Int_t sign = uidConfig % 2 == 0 ? 1 : -1;
  AliGenCocktail *ctl  = (AliGenCocktail*) GeneratorCocktail("Hijing_V0_PbPb2018");
  AliGenerator   *hij  = GeneratorHijing();
  AliGenerator   *ik0  = GeneratorInjector(ninjk0[iinj],          310, 1., 15., -0.7, 0.7);
  AliGenerator   *ila = GeneratorInjector(ninjla[iinj], sign *  3122, 1., 15., -0.7, 0.7);
  AliGenerator   *iala  = GeneratorInjector(ninjla[iinj], sign * -3122, 1., 15., -0.7, 0.7);

  ctl->AddGenerator(hij,  "Hijing",             1.);
  ctl->AddGenerator(ik0,  "Injector (K0s)",     1.);
  ctl->AddGenerator(ila,  "Injector (Lambda)",  1.);
  ctl->AddGenerator(iala, "Injector (ALambda)", 1.);

  return ctl;
}
