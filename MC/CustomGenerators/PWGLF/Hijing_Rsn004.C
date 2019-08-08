AliGenerator *GeneratorCustom(TString opt = "")
{
  //
  // Custom generator to inject resonances for PbPb@5-2015
  // Central     : a - inject 40+40 particles per event
  // Semi-Central: b - inject 10+10 particles per event
  // Peripheral  : c - inject 5+5 particles per event
  // 
  //   |y| < 0.6 for all species               
  //   flat pT: K* up 25 GeV/c; Sigma(1385) up 10 GeV/c; Xi(1530), K*(1430), Xi(1820) up 7 GeV/c
  //
  TString optList[3] = {"a", "b", "c"};
  Int_t ninjlist[3]  = {40, 10, 5};
  Int_t ninj = 1;

  for (Int_t iopt = 0; iopt < 3; iopt++)
    if (opt.EqualTo(optList[iopt]))
      ninj = ninjlist[iopt];

  // randomly injected particles
  //                 K*\pm  Sig*+  Sig*- Xi*(1530) K2*(1430) Xi*(1820)
  Int_t pdglist1[] = { 323,  3224,  3114,  3324,    315,    123314}; 
  Int_t pdglist2[] = {-323, -3224, -3114, -3324,   -315,   -123314}; 
  Double_t pTp[]   = { 25.,   10.,   10.,    7.,     7.,        7.};
  Int_t pdg1 = pdglist1[uidConfig % 6]; // select according to unique ID
  Int_t pdg2 = pdglist2[uidConfig % 6]; // select according to unique ID
  Double_t pTmax =  pTp[uidConfig % 6]; // select according to unique ID

  AliDecayerPythia *dec = new AliDecayerPythia;

  AliGenCocktail *ctl  = GeneratorCocktail("Hijing_Rsn004");
  AliGenerator   *hij  = GeneratorHijing();
  AliGenerator   *inj1 = GeneratorParam(ninj,  pdg1, 0., pTmax, -0.6, 0.6, dec);
  AliGenerator   *inj2 = GeneratorParam(ninj,  pdg2, 0., pTmax, -0.6, 0.6, dec);

  ctl->AddGenerator(hij,  "Hijing",              1.);
  ctl->AddGenerator(inj2, "Injector (Rsn004_1)", 1.);
  ctl->AddGenerator(inj1, "Injector (Rsn004_2)", 1.);
  return ctl;
}
