// Requires latest AliRoot --> fix for A=4 hypernuclei masses
AliGenerator *
GeneratorCustom()
{
  AliGenCocktail *ctl   = (AliGenCocktail*) GeneratorCocktail("Hijing_Nuclex007");
  AliGenerator   *hij   = GeneratorHijing();
  ctl->AddGenerator(hij,  "Hijing", 1.);
  
  //	Exotica + hypertriton, hyperhydrogen-4, hyperhelium-4, double-hyperhydrogen-4, 4(Xi-)He
  AliGenerator   *nu1a  = Generator_Nuclex((0x10 | 0x20 | 0x40 | 0x80 | 0x100 | 0x200 | 0x400 | 0x800 | 0x1000 | 0x2000 | 0x4000 | 0x8000 | 0x100000 | 0x200000), kFALSE, 40, 10., 1.);
  AliGenerator   *nu1b  = Generator_Nuclex((0x10 | 0x20 | 0x40 | 0x80 | 0x100 | 0x200 | 0x400 | 0x800 | 0x1000 | 0x2000 | 0x4000 | 0x8000 | 0x100000 | 0x200000), kTRUE,  40, 10., 1.);
  ctl->AddGenerator(nu1a,  "Nuclex1a", 1.);
  ctl->AddGenerator(nu1b,  "Nuclex1b", 1.);
  //	helium3, triton, alpha
  AliGenerator   *nu2a  = Generator_Nuclex((0x2 | 0x4 | 0x8), kFALSE, 10, 10., 1.);
  AliGenerator   *nu2b  = Generator_Nuclex((0x2 | 0x4 | 0x8), kTRUE,  10, 10., 1.);
  ctl->AddGenerator(nu2a,  "Nuclex2a", 1.);
  ctl->AddGenerator(nu2b,  "Nuclex2b", 1.);
  
  AliGenerator   *ilam  = GeneratorInjector(20, 3122,  0., 10., -1., 1.);
  AliGenerator   *ixi   = GeneratorInjector(20, 3312,  0., 10., -1., 1.);
  AliGenerator   *iom   = GeneratorInjector(20, 3334,  0., 10., -1., 1.);
  AliGenerator   *ialam = GeneratorInjector(20, -3122, 0., 10., -1., 1.);
  AliGenerator   *iaxi  = GeneratorInjector(20, -3312, 0., 10., -1., 1.);
  AliGenerator   *iaom  = GeneratorInjector(20, -3334, 0., 10., -1., 1.);
  ctl->AddGenerator(ilam,  "Injector (Lambda)", 1.);
  ctl->AddGenerator(ixi,   "Injector (Xi)",     1.);
  ctl->AddGenerator(iom,   "Injector (Omega)",  1.);
  ctl->AddGenerator(ialam, "Injector (Lambda)", 1.);
  ctl->AddGenerator(iaxi,  "Injector (Xi)",     1.);
  ctl->AddGenerator(iaom,  "Injector (Omega)",  1.);
  
  return ctl;
}
