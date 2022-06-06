// Requires AliRoot from v5-09-59b --> fix for A=4 hypernuclei masses
AliGenerator *
GeneratorCustom()
{
  AliGenCocktail *ctl   = (AliGenCocktail*) GeneratorCocktail("Hijing_Nuclex006");
  AliGenerator   *hij   = GeneratorHijing();
  ctl->AddGenerator(hij,  "Hijing", 1.);
  //	hypertriton, hyperhydrogen-4, hyperhelium-4, double-hyperhydrogen-4
  AliGenerator   *nu1a  = Generator_Nuclex(0x10C010, kFALSE, 40);
  AliGenerator   *nu1b  = Generator_Nuclex(0x10C010, kTRUE, 40);
  ctl->AddGenerator(nu1a,  "Nuclex1a", 1.);
  ctl->AddGenerator(nu1b,  "Nuclex1b", 1.);
  return ctl;
}
