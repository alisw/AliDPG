struct particle_inj {
  int n;
  char name[32];
  int pdg;
  double maxpt;
  double maxy;
};

AliGenerator* GeneratorCustom()
{
  AliGenCocktail *ctl = (AliGenCocktail*) GeneratorCocktail("Monash2013_Rsn_f2f0");
  // pythia8
  AliGenerator   *py8 = GeneratorPythia8(kPythia8Tune_Monash2013);
  ctl->AddGenerator(py8, "Pythia8 (Monash2013)", 1.);

  // randomly injected particles
  const int nParticles = 5;
  particle_inj particleList[5] = { // {name,pdgcode,maxpt,maxy},
    {1,"f0(1710)",10331,20.,0.9},
    {1,"f2(1525)",335,20.,0.9},
    {1,"f1(1285)",20223,20.,0.9},
    {1,"f1(1420)",20333,20.,0.9},
    {1,"f0(1500)",90302,20.,0.9}
  };

  AliDecayerPythia *dec = new AliDecayerPythia;
  AliPDG::AddParticlesToPdgDataBase();

  ctl->UseSingleInjectionPerEvent();
  for (int idx = 0; idx < nParticles; ++idx) {
    AliGenerator   *inj = GeneratorParam(particleList[idx].n, particleList[idx].pdg, 1., particleList[idx].maxpt,-particleList[idx].maxy, particleList[idx].maxy,dec);
    ctl->AddGenerator(inj, (TString(particleList[idx].name) + " injector").Data(), 1.);
  }
  return ctl;
}
