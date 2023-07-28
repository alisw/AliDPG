struct particle_inj
{
  int n;
  char name[32];
  int pdg;
  double maxpt;
  double maxy;
};

AliGenerator *GeneratorCustom()
{
  AliGenCocktail *ctl = (AliGenCocktail *)GeneratorCocktail("Monash2013_Rsn_f0f1_nodecayer");
  // pythia8
  AliGenerator *py8 = GeneratorPythia8(kPythia8Tune_Monash2013);
  ctl->AddGenerator(py8, "Pythia8 (Monash2013)", 1.);
  // randomly injected particles
  const int nParticles = 7;
  particle_inj particleList[nParticles] = {
      // {name,pdgcode,maxpt,maxy},
      {1, "f0(1710)", 10331, 20., 0.9},
      {1, "f2(1525)", 335, 20., 0.9},
      {1, "f0(1500)", 9030221, 20., 0.9},
      {1, "f2(1270)", 225, 20., 0.9},
      {1, "f0(1370)", 10221, 20., 0.9},
      {1, "f1(1285)", 20223, 20., 0.9},
      {1, "f1(1420)", 20333, 20., 0.9}};

  
  ctl->UseSingleInjectionPerEvent();
  for (int idx = 0; idx < nParticles; ++idx) {
    AliGenerator   *inj = GeneratorInjector(particleList[idx].n, particleList[idx].pdg, 0., particleList[idx].maxpt,-particleList[idx].maxy, particleList[idx].maxy);
    ctl->AddGenerator(inj, (TString(particleList[idx].name) + " injector").Data(), 1.);
  }
  return ctl;
}
