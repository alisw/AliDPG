AliGenerator *
GeneratorCustom()
{
  // triggered particles
  Int_t pdglist[] = {310, 3122, 3312, 3334, 310, -3122, -3312, -3334};
  Int_t pdg = pdglist[uidConfig % (sizeof(pdglist) / 4)]; // select according to unique ID
  AliGenerator   *py8 = GeneratorPythia8(kPythia8Tune_Monash2013, pdg, 1.2);
  //
  return py8;
}
