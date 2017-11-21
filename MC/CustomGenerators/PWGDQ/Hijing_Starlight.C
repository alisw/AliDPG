AliGenerator *
GeneratorCustom()
{
  AliGenCocktail* cocktail = GeneratorCocktail("Hijing_Starlight");
  AliGenerator* hijing = GeneratorHijing();
  cocktail->AddGenerator(hijing, "Hijing", 1.);    
  
  AliGenerator* starlight = GeneratorStarlight();
  cocktail->AddGenerator(starlight, "Starlight", 1.);
  
  return cocktail;
}
