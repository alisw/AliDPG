// Requires AliRoot from v5-09-59b --> fix for A=4 hypernuclei masses
AliGenerator *
GeneratorCustom()
{
  AliGenCocktail *ctl   = (AliGenCocktail*) GeneratorCocktail("Hijing_Nuclex007");
  AliGenerator   *hij   = GeneratorHijing();
  ctl->AddGenerator(hij,  "Hijing", 1.);
  
  /*//	hypertriton, hyperhydrogen-4, hyperhelium-4, double-hyperhydrogen-4
  AliGenerator   *nu1a  = Generator_Nuclex((0x10 | 0x4000 | 0x8000 | 0x100000), kFALSE, 40);
  AliGenerator   *nu1b  = Generator_Nuclex((0x10 | 0x4000 | 0x8000 | 0x100000), kTRUE,  40);
  ctl->AddGenerator(nu1a,  "Nuclex1a", 1.);
  ctl->AddGenerator(nu1b,  "Nuclex1b", 1.);
  //	helium3, triton, alpha
  AliGenerator   *nu2a  = Generator_Nuclex((0x2 | 0x4 | 0x8), kFALSE, 10);
  AliGenerator   *nu2b  = Generator_Nuclex((0x2 | 0x4 | 0x8), kTRUE,  10);
  ctl->AddGenerator(nu2a,  "Nuclex2a", 1.);
  ctl->AddGenerator(nu2b,  "Nuclex2b", 1.);*/
  
  TVirtualMCDecayer* decayer = new AliDecayerPythia();
  
  //Parametric injector -> Particles injected according to the pt distribution 
  //
  //Hyperhydrogen-4
  int ninj = 20;
  int pdgnumber = 1010010040;
  AliGenParam* gener1 = new AliGenParam((const char*) 0, (Int_t) ninj, (int) pdgnumber,
  (double (*)(const double*, const double*)) [](const double* px, const double*) -> double { return (double)(((*px)*0.546512*(9.21286-1)*(9.21286-2))/(9.21286*0.395236*(9.21286*0.395236+1.18937*(9.21286-2)))*pow((1+(pow((1.18937*1.18937+(*px)*(*px)),0.5)-1.18937)/(9.21286*0.395236)),(-9.21286))); },
  (double (*)(const double*, const double*)) [](const double* px, const double*) -> double { return (double)(*px>-0.9&&*px<0.9); },
  (double (*)(const double*, const double*)) [](const double* px, const double*) -> double { return 0.; });
  gener1->SetPtRange(0.,10.);
  gener1->SetYRange(-0.9,0.9);
  gener1->SetDecayer(decayer); //Decayer must be set explicitly for AliGenParam
  gener1->SetForceDecay(kAll);
  ctl->AddGenerator(gener1,"Hyperhydrogen-4",1.);

  AliGenParam* gener2 = new AliGenParam((const char*) 0, (Int_t) ninj, (int) -pdgnumber,
  (double (*)(const double*, const double*)) [](const double* px, const double*) -> double { return (double)(((*px)*0.546512*(9.21286-1)*(9.21286-2))/(9.21286*0.395236*(9.21286*0.395236+1.18937*(9.21286-2)))*pow((1+(pow((1.18937*1.18937+(*px)*(*px)),0.5)-1.18937)/(9.21286*0.395236)),(-9.21286))); },
  (double (*)(const double*, const double*)) [](const double* px, const double*) -> double { return (double)(*px>-0.9&&*px<0.9); },
  (double (*)(const double*, const double*)) [](const double* px, const double*) -> double { return 0.; });
  gener2->SetPtRange(0.,10.);
  gener2->SetYRange(-0.9,0.9);
  gener2->SetDecayer(decayer);
  gener2->SetForceDecay(kAll);
  ctl->AddGenerator(gener2,"Anti-Hyperhydrogen-4"",1.);
    
  //Hyperhelium-4
  pdgnumber = 1010020040;
  AliGenParam* gener3 = new AliGenParam((const char*) 0, (Int_t) ninj, (int) pdgnumber,
  (double (*)(const double*, const double*)) [](const double* px, const double*) -> double { return (double)(((*px)*0.546512*(9.21286-1)*(9.21286-2))/(9.21286*0.395236*(9.21286*0.395236+1.18937*(9.21286-2)))*pow((1+(pow((1.18937*1.18937+(*px)*(*px)),0.5)-1.18937)/(9.21286*0.395236)),(-9.21286))); },
  (double (*)(const double*, const double*)) [](const double* px, const double*) -> double { return (double)(*px>-0.9&&*px<0.9); },
  (double (*)(const double*, const double*)) [](const double* px, const double*) -> double { return 0.; });
  gener1->SetPtRange(0.,10.);
  gener1->SetYRange(-0.9,0.9);
  gener1->SetDecayer(decayer); //Decayer must be set explicitly for AliGenParam
  gener1->SetForceDecay(kAll);
  ctl->AddGenerator(gener3,"Hyperhelium-4",1.);

  AliGenParam* gener4 = new AliGenParam((const char*) 0, (Int_t) ninj, (int) -pdgnumber,
  (double (*)(const double*, const double*)) [](const double* px, const double*) -> double { return (double)(((*px)*0.546512*(9.21286-1)*(9.21286-2))/(9.21286*0.395236*(9.21286*0.395236+1.18937*(9.21286-2)))*pow((1+(pow((1.18937*1.18937+(*px)*(*px)),0.5)-1.18937)/(9.21286*0.395236)),(-9.21286))); },
  (double (*)(const double*, const double*)) [](const double* px, const double*) -> double { return (double)(*px>-0.9&&*px<0.9); },
  (double (*)(const double*, const double*)) [](const double* px, const double*) -> double { return 0.; });
  gener2->SetPtRange(0.,10.);
  gener2->SetYRange(-0.9,0.9);
  gener2->SetDecayer(decayer);
  gener2->SetForceDecay(kAll);
  ctl->AddGenerator(gener4,"Anti-Hyperhelium-4"",1.);
                    
  //Double-Hyperhydrogen-4
  pdgnumber = 1020010040;
  AliGenParam* gener5 = new AliGenParam((const char*) 0, (Int_t) ninj, (int) pdgnumber,
  (double (*)(const double*, const double*)) [](const double* px, const double*) -> double { return (double)(((*px)*0.546512*(9.21286-1)*(9.21286-2))/(9.21286*0.395236*(9.21286*0.395236+1.18937*(9.21286-2)))*pow((1+(pow((1.18937*1.18937+(*px)*(*px)),0.5)-1.18937)/(9.21286*0.395236)),(-9.21286))); },
  (double (*)(const double*, const double*)) [](const double* px, const double*) -> double { return (double)(*px>-0.9&&*px<0.9); },
  (double (*)(const double*, const double*)) [](const double* px, const double*) -> double { return 0.; });
  gener1->SetPtRange(0.,10.);
  gener1->SetYRange(-0.9,0.9);
  gener1->SetDecayer(decayer); //Decayer must be set explicitly for AliGenParam
  gener1->SetForceDecay(kAll);
  ctl->AddGenerator(gener5,"Double-Hyperhydrogen-4",1.);

  AliGenParam* gener6 = new AliGenParam((const char*) 0, (Int_t) ninj, (int) -pdgnumber,
  (double (*)(const double*, const double*)) [](const double* px, const double*) -> double { return (double)(((*px)*0.546512*(9.21286-1)*(9.21286-2))/(9.21286*0.395236*(9.21286*0.395236+1.18937*(9.21286-2)))*pow((1+(pow((1.18937*1.18937+(*px)*(*px)),0.5)-1.18937)/(9.21286*0.395236)),(-9.21286))); },
  (double (*)(const double*, const double*)) [](const double* px, const double*) -> double { return (double)(*px>-0.9&&*px<0.9); },
  (double (*)(const double*, const double*)) [](const double* px, const double*) -> double { return 0.; });
  gener2->SetPtRange(0.,10.);
  gener2->SetYRange(-0.9,0.9);
  gener2->SetDecayer(decayer);
  gener2->SetForceDecay(kAll);
  ctl->AddGenerator(gener6,"Anti-Double-Hyperhydrogen-4"",1.);
                    
  //4Xi(-)He
  pdgnumber = 1220010040;
  AliGenParam* gener7 = new AliGenParam((const char*) 0, (Int_t) ninj, (int) pdgnumber,
  (double (*)(const double*, const double*)) [](const double* px, const double*) -> double { return (double)(((*px)*0.546512*(9.21286-1)*(9.21286-2))/(9.21286*0.395236*(9.21286*0.395236+1.18937*(9.21286-2)))*pow((1+(pow((1.18937*1.18937+(*px)*(*px)),0.5)-1.18937)/(9.21286*0.395236)),(-9.21286))); },
  (double (*)(const double*, const double*)) [](const double* px, const double*) -> double { return (double)(*px>-0.9&&*px<0.9); },
  (double (*)(const double*, const double*)) [](const double* px, const double*) -> double { return 0.; });
  gener1->SetPtRange(0.,10.);
  gener1->SetYRange(-0.9,0.9);
  gener1->SetDecayer(decayer); //Decayer must be set explicitly for AliGenParam
  gener1->SetForceDecay(kAll);
  ctl->AddGenerator(gener7,"4Xi(-)He",1.);

  AliGenParam* gener8 = new AliGenParam((const char*) 0, (Int_t) ninj, (int) -pdgnumber,
  (double (*)(const double*, const double*)) [](const double* px, const double*) -> double { return (double)(((*px)*0.546512*(9.21286-1)*(9.21286-2))/(9.21286*0.395236*(9.21286*0.395236+1.18937*(9.21286-2)))*pow((1+(pow((1.18937*1.18937+(*px)*(*px)),0.5)-1.18937)/(9.21286*0.395236)),(-9.21286))); },
  (double (*)(const double*, const double*)) [](const double* px, const double*) -> double { return (double)(*px>-0.9&&*px<0.9); },
  (double (*)(const double*, const double*)) [](const double* px, const double*) -> double { return 0.; });
  gener2->SetPtRange(0.,10.);
  gener2->SetYRange(-0.9,0.9);
  gener2->SetDecayer(decayer);
  gener2->SetForceDecay(kAll);
  ctl->AddGenerator(gener8,"Anti-4Xi(-)He"",1.);
  
  return ctl;
}
