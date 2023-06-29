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
  AliGenCocktail *ctl = (AliGenCocktail *)GeneratorCocktail("Monash2013_Rsn_f0f1");
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

  AliDecayerPythia *dec = new AliDecayerPythia;
  dec->SetForceDecay(kF0F1K0s);
  AliDecayerPythia *dec2 = new AliDecayerPythia; // for f1 to K0s K anti-pi
  dec2->SetForceDecay(kF1K0sK);
  AliPDG::AddParticlesToPdgDataBase();  

  ctl->UseSingleInjectionPerEvent();
  for (int idx = 0; idx < nParticles; ++idx)
  {
    AliGenerator *inj = GeneratorParam(particleList[idx].n, particleList[idx].pdg, 1., particleList[idx].maxpt, -particleList[idx].maxy, particleList[idx].maxy, dec);
    ctl->AddGenerator(inj, (TString(particleList[idx].name) + " injector").Data(), 1.);
  }
  // f1(1285) to K0s K anti-pi
  AliGenerator *inj_20223 = GeneratorParam(1, 20223, 1., 20., -0.9, 0.9, dec2);
  ctl->AddGenerator(inj_20223, "f1(1285) to K0,K,pi- injector", 1.);
  // f1(1420) to K0s K anti-pi
  AliGenerator *inj_20333 = GeneratorParam(1, 20333, 1., 20., -0.9, 0.9, dec2);
  ctl->AddGenerator(inj_20333, "f1(1420) to K0,K,pi- injector", 1.);
  return ctl;
}
