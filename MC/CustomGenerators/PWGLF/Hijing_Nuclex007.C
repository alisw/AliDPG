// Requires AliRoot from v5-09-59b --> fix for A=4 hypernuclei masses
AliGenerator *
GeneratorCustom()
{
  AliGenCocktail *ctl   = (AliGenCocktail*) GeneratorCocktail("Hijing_Nuclex007");
  AliGenerator   *hij   = GeneratorHijing();
  ctl->AddGenerator(hij,  "Hijing", 1.);
  
  //	hypertriton, hyperhydrogen-4, hyperhelium-4, double-hyperhydrogen-4
  AliGenerator   *nu1a  = Generator_Nuclex((0x10 | 0x4000 | 0x8000 | 0x100000 | 0x200000), kFALSE, 40, 10., 1.);
  AliGenerator   *nu1b  = Generator_Nuclex((0x10 | 0x4000 | 0x8000 | 0x100000 | 0x200000), kTRUE,  40, 10., 1.);
  ctl->AddGenerator(nu1a,  "Nuclex1a", 1.);
  ctl->AddGenerator(nu1b,  "Nuclex1b", 1.);
  //	helium3, triton, alpha
  AliGenerator   *nu2a  = Generator_Nuclex((0x2 | 0x4 | 0x8), kFALSE, 10, 10., 1.);
  AliGenerator   *nu2b  = Generator_Nuclex((0x2 | 0x4 | 0x8), kTRUE,  10, 10., 1.);
  ctl->AddGenerator(nu2a,  "Nuclex2a", 1.);
  ctl->AddGenerator(nu2b,  "Nuclex2b", 1.);
  return ctl;
}
