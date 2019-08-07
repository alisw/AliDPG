AliGenerator *GeneratorCustom(TString opt = "")
{
  //
  // Custom generator to inject resonances for PbPb@5-2018
  // Central     : a - inject 40+40 particles per event
  // Semi-Central: b - inject 10+10 particles per event
  // Peripheral  : c - inject 5+5 particles per event
  // 
  //   |y| < 0.6 for all species               
  //   flat pT: K*0/pm, phi, rho, f0, f2 up 25 GeV/c; 
  //            Sigma(1385) up 10 GeV/c; 
  //            Xi(1530), K*(1430), L(1520), Xi(1820) up 7 GeV/c
  //
  TString optList[3] = {"a", "b", "c"};
  Int_t ninjlist[3]  = {40, 10, 5};
  Int_t ninj = 1;

  for (Int_t iopt = 0; iopt < 3; iopt++)
    if (opt.EqualTo(optList[iopt]))
      ninj = ninjlist[iopt];

  // randomly injected particles
  //                 K*\pm  Sig*+  Sig*- Xi*(1530) K2*(1430) Xi*(1820) K*0  rho/phi  f0/f2  L*(1520)
  Int_t pdglist1[] = { 323,  3224,  3114,  3324,    315,    123314,    313,  113,   9010221,  3124}; 
  Int_t pdglist2[] = {-323, -3224, -3114, -3324,   -315,   -123314,   -313,  333,       225, -3124}; 
  Double_t pTp[]   = { 25.,   10.,   10.,    7.,     7.,        7.,    25.,  25.,       25.,   7.0};
  Int_t pdg1 = pdglist1[uidConfig % 10]; // select according to unique ID
  Int_t pdg2 = pdglist2[uidConfig % 10]; // select according to unique ID
  Double_t pTmax =  pTp[uidConfig % 10]; // select according to unique ID

  AliDecayerPythia *dec = new AliDecayerPythia;

  AliGenCocktail *ctl  = GeneratorCocktail("Hijing_Rsn005");
  AliGenerator   *hij  = GeneratorHijing();
  AliGenerator   *inj1 = GeneratorParam(ninj,  pdg1, 0., pTmax, -0.6, 0.6, dec);
  AliGenerator   *inj2 = GeneratorParam(ninj,  pdg2, 0., pTmax, -0.6, 0.6, dec);

  ctl->AddGenerator(hij,  "Hijing",              1.);
  ctl->AddGenerator(inj2, "Injector (Rsn005_1)", 1.);
  ctl->AddGenerator(inj1, "Injector (Rsn005_2)", 1.);
  return ctl;
}
