//Custom Generator -> kPythia8Tune_Monash2013 + injected Sigma+/Anti-Sigma-
AliGenerator* GeneratorCustom()
{
  // Create cocktail to add the generators   
  AliGenCocktail *ctl = (AliGenCocktail*) GeneratorCocktail("Monash2013_SigmaPlus01");

  // Create pythia8 background event and add it to cocktail
  AliGenerator* py8 = GeneratorPythia8(kPythia8Tune_Monash2013);
  ctl->AddGenerator(py8,"Pythia8 (Monash2013)",1.);

  //Create pythia decayer 
  AliDecayerPythia* decayer = new AliDecayerPythia();

  //Parametric injector -> Particles injected according to the pt distribution defined in "PtSigmapp" 
  //Efficiency is strongly pt dependent, thus a non-flat injection is appropriate
  //Pt distribution is a Levi-Tsallis function tuned to HM-triggered pass1 data
  AliGenParam* gener1 = new AliGenParam((const char*) 0, (Int_t) 10, (int) 3222,
  (double (*)(const double*, const double*)) [](const double* px, const double*) -> double { return (double)(((*px)*0.546512*(9.21286-1)*(9.21286-2))/(9.21286*0.395236*(9.21286*0.395236+1.18937*(9.21286-2)))*pow((1+(pow((1.18937*1.18937+(*px)*(*px)),0.5)-1.18937)/(9.21286*0.395236)),(-9.21286))); },
  (double (*)(const double*, const double*)) [](const double* px, const double*) -> double { return (double)(*px>-0.9&&*px<0.9); },
  (double (*)(const double*, const double*)) [](const double* px, const double*) -> double { return 0.; });
  gener1->SetPtRange(0.,12.);
  gener1->SetYRange(-0.9,0.9);
  gener1->SetDecayer(decayer); //Decayer must be set explicitly for AliGenParam
  gener1->SetForceDecay(kNoDecay);
  ctl->AddGenerator(gener1,"Sigma+(1189) injector",1.);

  AliGenParam* gener2 = new AliGenParam((const char*) 0, (Int_t) 10, (int) -3222,
  (double (*)(const double*, const double*)) [](const double* px, const double*) -> double { return (double)(((*px)*0.546512*(9.21286-1)*(9.21286-2))/(9.21286*0.395236*(9.21286*0.395236+1.18937*(9.21286-2)))*pow((1+(pow((1.18937*1.18937+(*px)*(*px)),0.5)-1.18937)/(9.21286*0.395236)),(-9.21286))); },
  (double (*)(const double*, const double*)) [](const double* px, const double*) -> double { return (double)(*px>-0.9&&*px<0.9); },
  (double (*)(const double*, const double*)) [](const double* px, const double*) -> double { return 0.; });
  gener2->SetPtRange(0.,12.);
  gener2->SetYRange(-0.9,0.9);
  gener2->SetDecayer(decayer);
  gener2->SetForceDecay(kNoDecay);
  ctl->AddGenerator(gener2,"Sigma-_bar(1189) injector",1.);

  //Yield drops faster than efficiency increases, so statistics dies out at high pT 
  //Hence, 1 Sigma per event is additionally injected flat in pT
  //Box injector -> Particles injected flat in pt and y
  AliGenBox *box1 = new AliGenBox(1);
  box1->SetPart(3222);
  box1->SetPtRange(0.,10.);
  box1->SetYRange(-0.9,0.9);
  ctl->AddGenerator(box1,"Sigma+(1189) box injector",1.);

  AliGenBox *box2 = new AliGenBox(1);
  box2->SetPart(-3222);
  box2->SetPtRange(0.,10.);
  box2->SetYRange(-0.9,0.9);
  ctl->AddGenerator(box2,"Sigma-_bar(1189) box injector",1.);

  return ctl;
}