AliGenerator* GeneratorCustom() {
  AliGenerator* gen = GeneratorStarlight();
  
  if (processConfig.Contains("Incoh")) {
    Float_t bslope = 3.75;
    if      (systemConfig.EqualTo("p-Pb")) bslope = 6.4;   // Pb towards muon arm
    else if (systemConfig.EqualTo("Pb-p")) bslope = 3.75;  // p  towards muon arm
    printf("GeneratorCustom: b-slope = %.2f\n",bslope);
    
    AliGenStarLight *genStarLight = 0;
    if (processConfig.Contains("Psi2sToElPi") || processConfig.Contains("Psi2sToMuPi") || processConfig.Contains("RhoPrime")) {
      // printf("Extract starlight pointer from the cocktail\n");
      AliGenCocktail* genCocktail = (AliGenCocktail *) gen;
      genStarLight = (AliGenStarLight*) genCocktail->FirstGenerator()->Generator();
    } else {
      // printf("Take Starlight pointer directly\n");
      genStarLight = (AliGenStarLight *) gen;
    }
    genStarLight->SetParameter("BSLOPE_DEFINITION = 1");
    genStarLight->SetParameter(Form("BSLOPE_VALUE      = %.2f",bslope));
  }
  
  return gen;
}
