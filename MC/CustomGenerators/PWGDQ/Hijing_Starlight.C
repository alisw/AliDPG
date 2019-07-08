AliGenerator *
GeneratorCustom()
{
  AliGenCocktail* cocktail = GeneratorCocktail("Hijing_Starlight");
  
  AliGenerator* starlight = GeneratorStarlight();
  if (processConfig == TString("kTwoGammaToElMedium")) {
     AliGenStarLight *genStarLight = (AliGenStarLight*) starlight;
     genStarLight->SetParameter("W_MIN        =   1.5    #Min value of w");
     genStarLight->SetParameter("RAP_MAX      =   1.00    #max y");
  }
  cocktail->AddGenerator(starlight, "Starlight", 1.);   

  // Background events: HIJING (only if non-embedded)
  TString simulation = gSystem->Getenv("CONFIG_SIMULATION");
  if(!simulation.Contains("Embed")){
    AliGenerator* hijing = GeneratorHijing();
    cocktail->AddGenerator(hijing, "Hijing", 1.);
  }
  
  return cocktail;
}
