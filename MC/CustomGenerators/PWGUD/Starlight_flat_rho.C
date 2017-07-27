AliGenerator* GeneratorCustom() {
  AliGenerator* gen = GeneratorStarlight();
  if (processConfig == TString("kCohRhoToPiFlat")) {
    AliGenStarLight *genStarLight = (AliGenStarLight*) gen;
    // set maximum invariant mass for pipi system to 4 GeV/c^2 (2 GeV/c^2 by default)
    genStarLight->SetParameter("W_MAX        =   4.0    #Max value of w");
  }
  return gen;
}
