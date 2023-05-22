AliGenerator *
GeneratorCustom( ) //(TString opt)
{
  TString optList[3] = {"a", "b", "c"};
  Int_t ninjk0[3] = {10, 5, 1};
  Int_t ninjla[3] = {10, 5, 1};
  Int_t ninjxi[3] = {10, 5, 1};
  Int_t ninjom[3] = {10, 5, 1};

  Int_t iinj = 0;
  //for (Int_t iopt = 0; iopt < 3; iopt++)
  //  if (opt.EqualTo(optList[iopt]))
  //    iinj = iopt;
  Int_t sign = uidConfig % 2 == 0 ? 1 : -1;
  AliGenCocktail *ctl  = (AliGenCocktail*) GeneratorCocktail("Hijing_Strangness_PbPb2018");
  AliGenerator   *hij  = GeneratorHijing();
  AliGenerator   *ik0  = GeneratorInjector(ninjk0[iinj],          310, 1., 15., -0.7, 0.7);
  AliGenerator   *ila  = GeneratorInjector(ninjla[iinj], sign *  3122, 1., 15., -0.7, 0.7);
  AliGenerator   *iala = GeneratorInjector(ninjla[iinj], sign * -3122, 1., 15., -0.7, 0.7);
  
  AliGenerator   *ixim = GeneratorInjector(ninjxi[iinj], sign *  3312, 1., 15., -0.7, 0.7);
  AliGenerator   *ixip = GeneratorInjector(ninjxi[iinj], sign * -3312, 1., 15., -0.7, 0.7);
  AliGenerator   *iomm = GeneratorInjector(ninjom[iinj], sign *  3334, 1., 15., -0.7, 0.7);
  AliGenerator   *iomp = GeneratorInjector(ninjom[iinj], sign * -3334, 1., 15., -0.7, 0.7);

  ctl->AddGenerator(hij,  "Hijing",             1.);
  ctl->AddGenerator(ik0,  "Injector (K0s)",     1.);
  ctl->AddGenerator(ila,  "Injector (Lambda)",  1.);
  ctl->AddGenerator(iala, "Injector (ALambda)", 1.);
  
  ctl->AddGenerator(ixim,  "Injector (Xi-)",  1.);
  ctl->AddGenerator(ixip, "Injector (Xi+)", 1.);
  ctl->AddGenerator(iomm,  "Injector (Omega-)",  1.);
  ctl->AddGenerator(iomp, "Injector (Omega+)", 1.);
  
  return ctl;
}
