AliGenerator *
GeneratorCustom()
{
  AliGenCocktail* cocktail = GeneratorCocktail("Hijing_Starlight");
  AliGenerator* starlight = GeneratorStarlight();
  cocktail->AddGenerator(starlight, "Starlight", 1.);
  
  AliGenerator* hijing = GeneratorHijing();
  cocktail->AddGenerator(hijing, "Hijing", 1.);    
  
  return cocktail;
}
