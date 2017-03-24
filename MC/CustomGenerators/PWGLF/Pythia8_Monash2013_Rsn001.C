AliGenerator *
GeneratorCustom()
{
  AliGenCocktail *ctl = GeneratorCocktail("Monash2013_Rsn001");
  // pythia8
  AliGenerator   *py8 = GeneratorPythia8(kPythia8Tune_Monash2013);
  ctl->AddGenerator(py8, "Pythia8 (Monash2013)", 1.);
  // randomly injected particles
  Int_t pdglist[] = {225, 3124, -3124, 9010221}; // f2(1270), Lambda(1520), Lambda_bar(1520), f0(980)
  Int_t pdg = pdglist[uidConfig % (sizeof(pdglist) / 4)]; // select according to unique ID
  AliGenerator   *inj = GeneratorInjector(1, pdg, 0., 15., -0.6, 0.6);
  ctl->AddGenerator(inj, "Injector (Rsn001)", 1.);
  //
  return ctl;
}
