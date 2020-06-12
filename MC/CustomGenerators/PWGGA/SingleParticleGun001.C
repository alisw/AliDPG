AliGenerator *GeneratorCustom(TString opt = "")
{
  AliGenCocktail *ctl = (AliGenCocktail*) GeneratorCocktail("SingleParticleGun001");

  printf("INFO: Particle with PDG = %d will be injected in %2.1f < pT < %2.1f (GeV/c), %2.1f < y < %2.1f, %2.1f < phi < %2.1f \n",pdgConfig,ptminConfig,ptmaxConfig,yminConfig,ymaxConfig,phiminConfig,phimaxConfig);

  AliGenerator   *particle = GeneratorInjector(1, pdgConfig,ptminConfig,ptmaxConfig,yminConfig,ymaxConfig,phiminConfig,phimaxConfig);
  ctl->AddGenerator(particle, Form("Injector (PDG %d)",pdgConfig), 1.);

  return ctl;
}
