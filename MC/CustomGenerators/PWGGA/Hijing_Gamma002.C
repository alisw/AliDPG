AliGenerator *
GeneratorCustom()
{
  AliGenCocktail *ctl  = (AliGenCocktail*) GeneratorCocktail("Hijing_Gamma002");

  TString  simulation   = gSystem->Getenv("CONFIG_SIMULATION");  
  Double_t bEvalFormula = -1;
  Bool_t   isEmbedding  = kFALSE;
  if ( !simulation.Contains("Embed") )
  {
    AliGenerator *hij = GeneratorHijing();
    ctl->AddGenerator(hij, "Hijing", 1.);
  }
  else
  {
    isEmbedding=kTRUE;
    
    if ( bminConfig > bmaxConfig )
      Fatal("Hijing_Gamma002_MCtoMCembedding","max impact parameter smaller than min impact parameter");
    if ( bminConfig < 0. )
      Fatal("Hijing_Gamma002_MCtoMCembedding","min impact parameter <0");
    if ( bmaxConfig > 20 )
      Fatal("Hijing_Gamma002_MCtoMCembedding","unphysical impact parameter");
    
    bEvalFormula=(bmaxConfig+bminConfig)/2.;
  }
  
  // PCM
  TFormula* neutralsF  = new TFormula("neutrals",  "max(1.,470.*(x<5.)+62.*(x>7.5)*(x<12.5))");
  Int_t ntimes = 1;
  if ( isEmbedding )
  {
    ntimes=neutralsF->Eval(bEvalFormula);
    delete neutralsF; neutralsF=0x0;
  }
  AliGenerator   *pi0  = GeneratorInjector(1, 111, 0., 10., -1.2, 1.2);
  AliGenerator   *eta  = GeneratorInjector(1, 221, 0., 10., -1.2, 1.2);
  ctl->AddGenerator(pi0,  "Injector (pi0)", 1., neutralsF,ntimes);
  ctl->AddGenerator(eta,  "Injector (eta)", 1., neutralsF,ntimes);
  
  // PHOS
  AliGenerator   *pi0a = GeneratorInjector(1, 111, 0., 50., -0.155, 0.155, 240., 260.);
  AliGenerator   *pi0b = GeneratorInjector(1, 111, 0., 50., -0.155, 0.155, 260., 280.);
  AliGenerator   *pi0c = GeneratorInjector(1, 111, 0., 50., -0.155, 0.155, 280., 300.);
  AliGenerator   *pi0d = GeneratorInjector(1, 111, 0., 50., -0.155, 0.155, 300., 320.);
  AliGenerator   *etaa = GeneratorInjector(1, 221, 0., 50., -0.155, 0.155, 240., 320.);
  ctl->AddGenerator(pi0a, "Injector (pi0a)", 1.);
  ctl->AddGenerator(pi0b, "Injector (pi0b)", 1.);
  ctl->AddGenerator(pi0c, "Injector (pi0c)", 1.);
  ctl->AddGenerator(pi0d, "Injector (pi0d)", 1.);
  ctl->AddGenerator(etaa, "Injector (etaa)", 1.);

  // EMCAL
  AliGenerator   *pi0e = GeneratorInjector(1, 111, 0., 25., -0.7, 0.7, 80., 120.); // EMCal 1
  AliGenerator   *pi0f = GeneratorInjector(1, 111, 0., 25., -0.7, 0.7, 120., 160.); // EMCal 2
  AliGenerator   *pi0g = GeneratorInjector(1, 111, 0., 25., -0.7, 0.7, 160., 187.); // EMCal 3
  AliGenerator   *pi0h = GeneratorInjector(1, 111, 0., 25., 0.22, 0.7, 260., 327.); // DCal one side PHOS
  AliGenerator   *pi0i = GeneratorInjector(1, 111, 0., 25., -0.7, -0.22, 260., 327.); // DCal other side PHOS
  AliGenerator   *etab = GeneratorInjector(1, 221, 0., 25.,  -0.7, 0.7, 80., 120.); // EMCal 1
  AliGenerator   *etac = GeneratorInjector(1, 221, 0., 25.,  -0.7, 0.7, 120., 160); // EMCal 2
  AliGenerator   *etad = GeneratorInjector(1, 221, 0., 25.,  -0.7, 0.7, 160., 187.); // EMCal 3
  AliGenerator   *etae = GeneratorInjector(1, 221, 0., 25.,  0.22, 0.7, 260., 327.); // DCal one side PHOS
  AliGenerator   *etaf = GeneratorInjector(1, 221, 0., 25., -0.7, -0.22, 260., 327.); // DCal other side PHOS
  ctl->AddGenerator(pi0e, "Injector (pi0e)", 1.);
  ctl->AddGenerator(pi0f, "Injector (pi0f)", 1.);
  ctl->AddGenerator(pi0g, "Injector (pi0g)", 1.);
  ctl->AddGenerator(pi0h, "Injector (pi0h)", 1.);
  ctl->AddGenerator(pi0i, "Injector (pi0i)", 1.);
  ctl->AddGenerator(etab, "Injector (etab)", 1.);
  ctl->AddGenerator(etac, "Injector (etac)", 1.);
  ctl->AddGenerator(etad, "Injector (etad)", 1.);
  ctl->AddGenerator(etae, "Injector (etae)", 1.);
  ctl->AddGenerator(etaf, "Injector (etaf)", 1.);

  return ctl;
}
