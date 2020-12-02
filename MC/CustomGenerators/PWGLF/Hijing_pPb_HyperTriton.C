AliGenerator *
GeneratorCustom()
{
  AliGenCocktail *ctl   = (AliGenCocktail*) GeneratorCocktail("Hijing_pPb_HyperTriton");
 
  TString simulation = gSystem->Getenv("CONFIG_SIMULATION");
  if(!simulation.Contains("Embed")){
    AliGenerator *hij = GeneratorHijing();
    ctl->AddGenerator(hij, "Hijing", 1.);
  }

  AliGenerator   *nu2a  = Generator_Nuclex(0x10, kFALSE, 5);
  AliGenerator   *nu2b  = Generator_Nuclex(0x10, kTRUE, 5);
  ctl->AddGenerator(nu2a,  "HyperTritons", 1.);
  ctl->AddGenerator(nu2b,  "AntiHyperTritons", 1.);
  return ctl;
}
