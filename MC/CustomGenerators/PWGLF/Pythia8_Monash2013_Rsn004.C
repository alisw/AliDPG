struct particle_inj {
  int n;
  char name[32];
  int pdg;
  double maxpt;
  double maxy;
};

AliGenerator* GeneratorCustom()
{
  AliGenCocktail *ctl = (AliGenCocktail*) GeneratorCocktail("Monash2013_RsnNcl004");
  // pythia8
  AliGenerator   *py8 = GeneratorPythia8(kPythia8Tune_Monash2013);
  ctl->AddGenerator(py8, "Pythia8 (Monash2013)", 1.);

  // randomly injected particles
  const int nParticles = 16;
  particle_inj particleList[16] = { // {name,pdgcode,maxpt,maxy},
      {1,"Ps(1870)+",9322132,10.,0.55},
      {1,"Ps_Bar(1870)-",-9322132,10.,0.55},
      {1,"Ps(2065)+",9322312,10.,0.55},
      {1,"Ps_Bar(2065)-",-9322312,10.,0.55},
      {1,"Ps(2255)+",9323212,10.,0.55},
      {1,"Ps_Bar(2255)-",-9323212,10.,0.55},
      {1,"Ps(2455)+",9332212,10.,0.55},
      {1,"Ps_Bar(2455)-",-9332212,10.,0.55},
      {1,"Ps(1870)0",9322131,10.,0.55},
      {1,"Ps_Bar(1870)0",-9322131,10.,0.55},
      {1,"Ps(2065)0",9322311,10.,0.55},
      {1,"Ps_Bar(2065)0",-9322311,10.,0.55},
      {1,"Ps(2255)0",9323211,10.,0.55},
      {1,"Ps_Bar(2255)0",-9323211,10.,0.55},
      {1,"Ps(2455)0",9332211,10.,0.55},
      {1,"Ps_Bar(2455)0",-9332211,10.,0.55}
  };

    AliDecayerPythia *dec = new AliDecayerPythia;
    AliPDG::AddParticlesToPdgDataBase();

    //ctl->UseSingleInjectionPerEvent();
    for (int idx = 0; idx < nParticles; ++idx) {
      AliGenerator   *inj = GeneratorParam(particleList[idx].n, particleList[idx].pdg, 1., particleList[idx].maxpt,-particleList[idx].maxy, particleList[idx].maxy,dec);
      ctl->AddGenerator(inj, (TString(particleList[idx].name) + " injector").Data(), 1.);
    }
    return ctl;
  }

