// Requires AliRoot from v5-09-02
AliGenerator *
GeneratorCustom()
{
  AliGenCocktail *ctl   = GeneratorCocktail("Hijing_Nuclex005");
  AliGenerator   *hij   = GeneratorHijing();
  ctl->AddGenerator(hij,  "Hijing", 1.);
  AliGenerator   *nu1a  = Generator_Nuclex(0x70000, kFALSE, 10);
  AliGenerator   *nu1b  = Generator_Nuclex(0x70000, kTRUE, 10);
  ctl->AddGenerator(nu1a,  "Nuclex1a", 1.);
  ctl->AddGenerator(nu1b,  "Nuclex1b", 1.);
  return ctl;
}
