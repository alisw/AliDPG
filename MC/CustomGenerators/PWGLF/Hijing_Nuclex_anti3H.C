// Requires AliRoot from v5-09-02
AliGenerator *
GeneratorCustom()
{
  AliGenCocktail *ctl   = (AliGenCocktail*) GeneratorCocktail("Hijing_Nuclex_anti3H");
  AliGenerator   *hij   = GeneratorHijing();
  ctl->AddGenerator(hij,  "Hijing", 1.);
  AliGenerator   *nu1a  = Generator_Nuclex(0x4, kTRUE, 160, 12., 0.8);
  ctl->AddGenerator(nu1a,  "Anti-3H", 1.);
  return ctl;
}
