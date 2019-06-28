struct particle_inj {
  int n;
  char name[32];
  int pdg;
  double maxpt;
  double maxy;
};

AliGenerator* GeneratorCustom()
{
  AliGenCocktail *ctl = GeneratorCocktail("Monash2013_RsnNcl002");
  // pythia8
  AliGenerator   *py8 = GeneratorPythia8(kPythia8Tune_Monash2013);
  ctl->AddGenerator(py8, "Pythia8 (Monash2013)", 1.);

  // randomly injected particles
  const int nParticles = 14;
  particle_inj particleList[14] = { // {name,pdgcode,maxpt,maxy},
    {1,"f2(1270)",225,15.,0.6},
    {1,"Lambda(1520)",3124,15.,0.6},
    {1,"Lambda_bar(1520)",-3124,15.,0.6},
    {1,"f0(980)",9010221,15.,0.6},
    {1,"Xi*0(1530)",3324,15.,0.6},
    {1,"Xi*0_bar(1530)",-3324,15.,0.6},
    {1,"Xi0(1820)",123324,15.,0.6},
    {1,"Xi0_bar(1820)",-123324,15.,0.6},
    {1,"Xi-(1820)",123314,15.,0.6},
    {1,"Xi+(1820)",-123314,15.,0.6},
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
