AliGenerator *
GeneratorCustom(TString opt = "")
{
  TString optList[3] = {"a", "b", "c"};
  Int_t ninjlist[3] = {25, 7, 3};
  Int_t ninj = 1;
  for (Int_t iopt = 0; iopt < 3; iopt++)
    if (opt.EqualTo(optList[iopt]))
      ninj = ninjlist[iopt];

  // randomly injected particles
  Int_t pdglist1[] = {3124, 225}; // Lambda(1520), f2(1270), 
  Int_t pdglist2[] = {-3124, 9010221}; // Lambda_bar(1520), f0(980)
  Int_t pdg1 = pdglist1[uidConfig % 2]; // select according to unique ID
  Int_t pdg2 = pdglist2[uidConfig % 2]; // select according to unique ID

  AliGenCocktail *ctl  = GeneratorCocktail("Hijing_Rsn002");
  AliGenerator   *hij  = GeneratorHijing();
  AliGenerator   *inj1 = GeneratorInjector(ninj, pdg1, 0., 10., -0.6, 0.6);
  AliGenerator   *inj2 = GeneratorInjector(ninj, pdg2, 0., 10., -0.6, 0.6);
  ctl->AddGenerator(hij,  "Hijing",            1.);
  ctl->AddGenerator(inj1, "Injector (Rsn001)", 1.);
  ctl->AddGenerator(inj2, "Injector (Rsn001)", 1.);
  return ctl;
}
