AliGenerator *
GeneratorCustom()
{
  AliGenCocktail *ctl = GeneratorCocktail("Monash2013_Nuclex001");
  // pythia8
  AliGenerator   *py8 = GeneratorPythia8(kPythia8Tune_Monash2013);
  ctl->AddGenerator(py8, "Pythia8 (Monash2013)", 1.);
  //
  AliGenerator   *nu1a  = Generator_Nuclex(0x7, kFALSE, 7,6.f,1.f);
  AliGenerator   *nu1b  = Generator_Nuclex(0x7, kTRUE,  7,6.f,1.f);
  ctl->AddGenerator(nu1a,  "Nuclex1a", 1.);
  ctl->AddGenerator(nu1b,  "Nuclex1b", 1.);
  return ctl;
}
