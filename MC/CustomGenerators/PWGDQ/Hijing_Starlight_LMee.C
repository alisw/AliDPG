AliGenerator *
GeneratorCustom()
{
  AliGenCocktail *cocktail = (AliGenCocktail*) GeneratorCocktail("Hijing_Starlight_LMee");

  AliGenerator* starlight = GeneratorStarlight();
  AliGenStarLight *genStarLight = (AliGenStarLight*) starlight;  
  if (processConfig == TString("kTwoGammaToElLow")) {
     genStarLight->SetParameter("W_MIN = 0.38 #Min value of w");
  }
  else if (processConfig == TString("kTwoGammaToElMedium")) {
     genStarLight->SetParameter("W_MIN = 1.1 #Min value of w");
  }
  else if (processConfig == TString("kTwoGammaToElHigh")) {
     genStarLight->SetParameter("W_MIN = 1.8 #Min value of w");
  }
  genStarLight->SetParameter("W_MAX = 15.0 #Min value of w");
  genStarLight->SetParameter("RAP_MAX = 1.00 #max y");
  genStarLight->SetParameter("CUT_ETA = 1 #Cut in pseudorapidity? (0 = no, 1 = yes)");
  genStarLight->SetParameter("ETA_MIN = -1 #Minimum pseudorapidity");
  genStarLight->SetParameter("ETA_MAX = 1 #Maximum pseudorapidity");
  cocktail->AddGenerator(starlight, "Starlight", 1.);   

  // Background events: HIJING (only if non-embedded)
  TString simulation = gSystem->Getenv("CONFIG_SIMULATION");
  if(!simulation.Contains("Embed")){
    AliGenerator* hijing = GeneratorHijing();
    cocktail->AddGenerator(hijing, "Hijing", 1.);
  }
  
  return cocktail;
}
