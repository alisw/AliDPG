// Dedicated script for the Monash2013 with resonance generation
// Only inject rho and Sigma1385 up to 15 GeV/c
struct particle_inj {
  int n;
  char name[32];
  int pdg;
  double maxpt;
  double maxy;
};

AliGenerator* GeneratorCustom()
{
  AliGenCocktail *ctl = (AliGenCocktail*) GeneratorCocktail("Monash2013_RsnNcl005");
  // pythia8
  AliGenerator   *py8 = GeneratorPythia8(kPythia8Tune_Monash2013);
  ctl->AddGenerator(py8, "Pythia8 (Monash2013)", 1.);

  // randomly injected particles
  const int nParticles = 5;
  particle_inj particleList[5] = { // {name,pdgcode,maxpt,maxy},
    {1,"rho", 113, 15., 0.6},
    {1,"Sigma*+(1385)",3224,15.,0.6},
    {1,"Sigma*+_bar(1385)",-3224,15.,0.6},
    {1,"Sigma*-(1385)",3114,15.,0.6},
    {1,"Sigma*-_bar(1385)",-3114,15.,0.6}
  };

  AliDecayerPythia *dec = new AliDecayerPythia;
  AliPDG::AddParticlesToPdgDataBase();

  ctl->UseSingleInjectionPerEvent();
  for (int idx = 0; idx < nParticles; ++idx) {
    AliGenerator   *inj = GeneratorParam(particleList[idx].n, particleList[idx].pdg, 0., particleList[idx].maxpt,-particleList[idx].maxy, particleList[idx].maxy,dec);
    ctl->AddGenerator(inj, (TString(particleList[idx].name) + " injector").Data(), 1.);
  }
  return ctl;
}
