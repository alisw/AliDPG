struct particle_inj {
  int n;
  TString name;
  int pdg;
  double maxpt;
  double maxy;
};

AliGenerator *
GeneratorCustom()
{
  AliGenCocktail *ctl = GeneratorCocktail("Monash2013_Nucl001");

  // pythia8
  AliGenerator   *py8 = GeneratorPythia8(kPythia8Tune_Monash2013);
  ctl->AddGenerator(py8, "Pythia8 (Monash2013)", 1.);

  // randomly injected particles
  const int nParticles = 8;
  particle_inj particleList[8] = { // {name,pdgcode,maxpt,maxy},
    {1,"deuteron",1000010020,6.,0.82},
    {1,"anti-deuteron",-1000010020,6.,0.82},
    {1,"triton",1000010030,6.,0.82},
    {1,"anti-triton",-1000010030,6.,0.82},
    {1,"3He",1000020030,6.,0.82},
    {1,"anti-3He",-1000020030,6.,0.82},
    {1,"hypertriton",1010010030,10.,0.82},
    {1,"anti-hypertriton",-1010010030,10.,0.82}
  };

  ctl->UseSingleInjectionPerEvent();
  for (int idx = 0; idx < nParticles; ++idx) {
    AliGenerator   *inj = GeneratorInjector(particleList[idx].n, particleList[idx].pdg, 0., particleList[idx].maxpt,-particleList[idx].maxy, particleList[idx].maxy);
    ctl->AddGenerator(inj, (particleList[idx].name + " injector").Data(), 1.);
  }
  return ctl;
}

