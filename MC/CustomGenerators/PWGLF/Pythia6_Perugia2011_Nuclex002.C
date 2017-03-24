AliGenerator *
GeneratorCustom() 
{
  AliGenCocktail *ctl   = GeneratorCocktail("Perugia2011_Nuclex002");
  AliGenerator   *pyt   = GeneratorPythia6(kPythia6Tune_Perugia2011);
  ctl->AddGenerator(pyt,  "Pythia6", 1.);
  AliGenerator   *nu1a  = Generator_Nuclex(0x1F, kFALSE, 10);
  AliGenerator   *nu1b  = Generator_Nuclex(0x1F, kTRUE, 10);
  ctl->AddGenerator(nu1a,  "Nuclex1a", 1.);
  ctl->AddGenerator(nu1b,  "Nuclex1b", 1.);
  return ctl;
}
