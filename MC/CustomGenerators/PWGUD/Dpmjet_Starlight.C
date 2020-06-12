AliGenerator *
GeneratorCustom()
{
  // Init cocktail
  AliGenCocktail* cocktail = (AliGenCocktail*) GeneratorCocktail("Dpmjet_Starlight");

  // DPMjet
  AliGenerator   *dpm   = GeneratorPhojet();
  cocktail->AddGenerator(dpm,  "Dpmjet", 1.);

  // STARLIGHT
  AliGenerator* starlight = GeneratorStarlight();
  cocktail->AddGenerator(starlight, "Starlight", 1.);
  
  return cocktail;
}
