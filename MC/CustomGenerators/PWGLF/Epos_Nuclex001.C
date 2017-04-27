AliGenerator *
GeneratorCustom()
{
  AliGenCocktail *ctl   = GeneratorCocktail("Epos_Nuclex001");
  AliGenerator   *epos  = GeneratorEPOSLHC();
  ctl->AddGenerator(epos, "EPOSLHC", 1.);
  AliGenerator   *nu1a  = Generator_Nuclex(0xF, kFALSE, 10);
  AliGenerator   *nu1b  = Generator_Nuclex(0xF, kTRUE, 10);
  AliGenerator   *nu2a  = Generator_Nuclex(0x10, kFALSE, 20);
  AliGenerator   *nu2b  = Generator_Nuclex(0x10, kTRUE, 20);
  ctl->AddGenerator(nu1a,  "Nuclex1a", 1.);
  ctl->AddGenerator(nu1b,  "Nuclex1b", 1.);
  ctl->AddGenerator(nu2a,  "Nuclex2a", 1.);
  ctl->AddGenerator(nu2b,  "Nuclex2b", 1.);
  return ctl;
}
