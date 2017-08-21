struct particle_inj {
  int pdg;
  double maxpt;
  double maxy;
};

AliGenerator *
GeneratorCustom()
{
  AliGenCocktail *ctl = GeneratorCocktail("Monash2013_RsnNcl001");
  // pythia8
  AliGenerator   *py8 = GeneratorPythia8(kPythia8Tune_Monash2013);
  ctl->AddGenerator(py8, "Pythia8 (Monash2013)", 1.);

  // randomly injected particles
  const int nParticles = 12;
  particle_inj particleList[12] = { // {pdgcode,maxpt,maxy},
    {1000010020,6.,0.82},   // Deuteron
    {-1000010020,6.,0.82},  // Anti-Deuteron
    {1000010030,6.,0.82},   // Triton
    {-1000010030,6.,0.82},  // Anti-Triton
    {1000020030,6.,0.82},   // 3He
    {-1000020030,6.,0.82},  // Anti-3He
    {1010010030,10.,0.82},  // Hypertriton
    {-1010010030,10.,0.82}, // Anti-Hypertriton
    {225,15.,0.6},          // f2(1270)
    {3124,15.,0.6},         // Lambda(1520)
    {-3124,15.,0.6},        // Lambda_bar(1520)
    {9010221,15.,0.6}       // f0(980)
  };

  const int idx = uidConfig % nParticles;

  Int_t pdg = particleList[idx].pdg; // select according to unique ID
  AliGenerator   *inj = GeneratorInjector(1, particleList[idx].pdg, 0., particleList[idx].maxpt,
     -particleList[idx].maxy, particleList[idx].maxy);
  ctl->AddGenerator(inj, "Injector (RsnNcl001)", 1.);
  //
  return ctl;
}

