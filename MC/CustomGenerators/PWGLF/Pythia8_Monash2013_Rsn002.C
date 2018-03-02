struct particle_inj {
  int n;
  TString name;
  int pdg;
  double maxpt;
  double maxy;
};

AliGenParam* GeneratorParam(int n, int pdg, double ptmin, double ptmax, double ymin, double ymax, AliDecayer* dec = 0x0) {
  AliGenParam* gen = new AliGenParam(Form("%i",pdg), n, pdg);
  gen->SetYRange(ymin,ymax);
  gen->SetPtRange(ptmin,ptmax);
  if (dec) gen->SetDecayer(dec);
  return gen;
}

AliGenerator *
GeneratorCustom()
{
  AliGenCocktail *ctl = GeneratorCocktail("Monash2013_RsnNcl002");
  // pythia8
  AliGenerator   *py8 = GeneratorPythia8(kPythia8Tune_Monash2013);
  ctl->AddGenerator(py8, "Pythia8 (Monash2013)", 1.);

  // randomly injected particles
  const int nParticles = 10;
  particle_inj particleList[10] = { // {name,pdgcode,maxpt,maxy},
    {1,"f2(1270)",225,15.,0.6},
    {1,"Lambda(1520)",3124,15.,0.6},
    {1,"Lambda_bar(1520)",-3124,15.,0.6},
    {1,"f0(980)",9010221,15.,0.6},
    {1,"Xi*0(1530)",3324,15.,0.6},
    {1,"Xi*0_bar(1530)",-3324,15.,0.6},
    {1,"Xi0(1820)",123324,15.,0.6},
    {1,"Xi0_bar(1820)",-123324,15.,0.6},
    {1,"Xi-(1820)",123314,15.,0.6},
    {1,"Xi+(1820)",-123314,15.,0.6}
  };

  AliDecayerPythia *dec = new AliDecayerPythia;

  ctl->UseSingleInjectionPerEvent();
  for (int idx = 0; idx < nParticles; ++idx) {
    AliGenerator   *inj = GeneratorParam(particleList[idx].n, particleList[idx].pdg, 0., particleList[idx].maxpt,-particleList[idx].maxy, particleList[idx].maxy,dec);
    ctl->AddGenerator(inj, (particleList[idx].name + " injector").Data(), 1.);
  }
  return ctl;
}
