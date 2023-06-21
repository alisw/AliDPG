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
      {1, "f1(1420)", 20333, 20., 0.9}
  };

  // Force decay mode of f0(1500) to K0s K0s - disabled since it's configured in the decay table directly
  // (AliPythia8::Instance())->ReadString("9030221:onMode = off");
  // (AliPythia8::Instance())->ReadString("9030221:oneChannel = 1 1.00 0 310 -310");
  // (AliPythia8::Instance())->ReadString("9030221:onIfMatch = 310 -310");
  // Force decay mode of f0(1710) to K0s K0s
  (AliPythia8::Instance())->ReadString("10331:onMode = off");
  (AliPythia8::Instance())->ReadString("10331:oneChannel = 1 1.00 0 310 -310");
  (AliPythia8::Instance())->ReadString("10331:onIfMatch = 310 -310");
  // force decay of f2(1525) to K0s K0s
  (AliPythia8::Instance())->ReadString("335:onMode = off");
  (AliPythia8::Instance())->ReadString("335:oneChannel = 1 1.00 0 310 -310");
  (AliPythia8::Instance())->ReadString("335:onIfMatch = 310 -310");
  // force decay of f2(1270) to K0s K0s
  (AliPythia8::Instance())->ReadString("225:onMode = off");
  (AliPythia8::Instance())->ReadString("225:oneChannel = 1 1.00 0 310 -310");
  (AliPythia8::Instance())->ReadString("225:onIfMatch = 310 -310");
  // force decay of f0(1370) to K0s K0s
  (AliPythia8::Instance())->ReadString("10221:onMode = off");
  (AliPythia8::Instance())->ReadString("10221:oneChannel = 1 1.00 0 310 -310");
  (AliPythia8::Instance())->ReadString("10221:onIfMatch = 310 -310");
  // force decay of f1(1285) to K0s, anti-K, pi+
  (AliPythia8::Instance())->ReadString("20223:onMode = off");
  (AliPythia8::Instance())->ReadString("20223:oneChannel = 1 1.00 0 310 -321 211");
  (AliPythia8::Instance())->ReadString("20223:onIfMatch = 310 -321 211");
  // force decay of f1(1420) to K0s, anti-K, pi+
  (AliPythia8::Instance())->ReadString("20333:onMode = off");
  (AliPythia8::Instance())->ReadString("20333:oneChannel = 1 1.00 0 310 -321 211");
  (AliPythia8::Instance())->ReadString("20333:onIfMatch = 310 -321 211");


  AliDecayerPythia *dec = new AliDecayerPythia;
  AliPDG::AddParticlesToPdgDataBase();

  ctl->UseSingleInjectionPerEvent();
  for (int idx = 0; idx < nParticles; ++idx)
  {
    AliGenerator *inj = GeneratorParam(particleList[idx].n, particleList[idx].pdg, 1., particleList[idx].maxpt, -particleList[idx].maxy, particleList[idx].maxy, dec);
    ctl->AddGenerator(inj, (TString(particleList[idx].name) + " injector").Data(), 1.);
  }
  return ctl;
}
