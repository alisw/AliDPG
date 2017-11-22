AliGenerator* GeneratorCustom() {
  AliGenerator* gen = GeneratorStarlight();
  if (processConfig == TString("kTwoGammaToElLow")) {
    AliGenStarLight *genStarLight = (AliGenStarLight*) gen;
    // set minimum invariant mass for ee continuum to 0.3 GeV/c^2 (0.4 GeV/c^2 by default)
    genStarLight->SetParameter("W_MIN        =   0.3    #Min value of w");
  }
  return gen;
}
