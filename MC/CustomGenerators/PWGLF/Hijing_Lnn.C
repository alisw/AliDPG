AliGenerator * GeneratorCustom()
{
  AliGenCocktail *ctl   = (AliGenCocktail*) GeneratorCocktail("Hijing_Nuclex001");
  AliGenerator   *hij   = GeneratorHijing();
  ctl->AddGenerator(hij,  "Hijing", 1.);
  AliGenerator   *nu1a  = Generator_Nuclex(0x2000, kFALSE, 5,10,1);
  ctl->AddGenerator(nu1a,"Nuclex_Lnn", 1);
  AliGenerator   *nu1b  = Generator_Nuclex(0x2000, kTRUE, 5,10,1);
  ctl->AddGenerator(nu1b,"Nuclex_AntiLnn", 1);
  return ctl;

}

