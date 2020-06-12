AliGenerator * GeneratorCustom()
{
  AliGenerator   *gen = (AliGenCocktail*) GeneratorCocktail("LambdaN");

  AliGenCocktail *ctl = (AliGenCocktail*) gen; 
  
  AliGenerator   *hij = GeneratorHijing();
  ctl->AddGenerator(hij,  "Hijing", 1.);

  AliGenerator   *LN  = Generator_Nuclex(0x2020, kFALSE, 20);
  ctl->AddGenerator(LN,"Nuclex_LambdaN", 1.);

  AliGenerator   *aLN  = Generator_Nuclex(0x2020, kTRUE, 20);
  ctl->AddGenerator(aLN,"Nuclex_AntiLambdaN", 1.);

  return gen;

}

