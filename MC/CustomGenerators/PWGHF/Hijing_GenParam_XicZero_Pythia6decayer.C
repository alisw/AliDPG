//--------------------------------------------------------------------------------------------------//
//                     MC production for XicZero analysis in PbPb at 5.02 TeV                    //
//                                                                                                  //
// MC is for a pure signals of Xiczer0 , with flat y and pT input shapes    //
// Production should be done on the run-by-run basis.
//
// Note that IF IT IS NOT USED WITH EMBEDDING, THE SETTINGS ARE OK ONLY FOR CENTRAL PbPB, i.e.  THE NUMBER OF XICZERO INJECTED PER EVENT IS LARGE (100) //
//--------------------------------------------------------------------------------------------------//


Int_t IpXicZeroPbPb( TRandom *ran);
Double_t PtFlatXicZeroPbPb( const Double_t *px, const Double_t */*dummy*/ );
Double_t YFlatXicZeroPbPb( const Double_t *py, const Double_t */*dummy*/ );
Double_t V2XicZeroPbPb( const Double_t *pv, const Double_t */*dummy*/ );

//-------------------------------------------------------------------------
AliGenerator *GeneratorCustom(TString opt = "")
{

  // HIJING EVENT	
  AliGenCocktail *ctl  = static_cast<AliGenCocktail*>(GeneratorCocktail("Hijing_HF")); 
  TString simulation = gSystem->Getenv("CONFIG_SIMULATION");
  Int_t ntimes=1;
  Bool_t isForFeedDown=kFALSE;
  Bool_t noCutOnChildPrompt=kFALSE;

  //for (Int_t iopt = 0; iopt < 4; iopt++)
  if (opt.EqualTo("feeddown"))isForFeedDown=kTRUE;
    
  Double_t bEvalFormula=-1;
  Bool_t isEmbedding=kFALSE;
  if(opt.Contains("noCutOnChildEta")){
    noCutOnChildPrompt=kTRUE;
  }
  
  if(!simulation.Contains("Embed")){
    AliGenerator *hij = GeneratorHijing();
    ctl->AddGenerator(hij, "Hijing", 1.);
  }
  else{
    isEmbedding=kTRUE;
    
    if(bminConfig>bmaxConfig)Fatal("Hijing_GenParam_XicZero_Pythia6decayer","max impact parameter smaller than min impact parameter");
    if(bminConfig<0.)Fatal("Hijing_GenParam_XicZero_Pythia6decayer","min impact parameter <0");
    if(bmaxConfig>20)Fatal("Hijing_GenParam_XicZero_Pythia6decayer","unphysical impact parameter");
    bEvalFormula=(bmaxConfig+bminConfig)/2.;
  }
  // Note that IF IT IS NOT USED WITH EMBEDDING, THE SETTINGS ARE OK ONLY FOR CENTRAL PbPB, i.e.  THE NUMBER OF XICZERO INJECTED PER EVENT IS LARGE (100) //
  

  TVirtualMCDecayer* decayer = new AliDecayerPythia();
  decayer->SetForceDecay(kAll);//kAll
  decayer->Init();
  gMC->SetExternalDecayer(decayer);

  // Set pseudorapidity range from -1. to 1. (used in AliGenParams Generators)
  Float_t thmin          = (180./TMath::Pi())*2.*atan(exp(-1.));  
  Float_t thmax          = (180./TMath::Pi())*2.*atan(exp( 1.)); 

  if(!isForFeedDown){
    AliGenParam *XicZeroGener;
    if(bEvalFormula<5)XicZeroGener = new AliGenParam("GenParamXicZero",50,4132); //particle
    else if(bEvalFormula<13)XicZeroGener = new AliGenParam("GenParamXicZero",15,4132); //particle
    XicZeroGener->SetMomentumRange(0., 999.);
    XicZeroGener->SetPtRange(0., 16.);
    XicZeroGener->SetYRange(-1, 1);
    XicZeroGener->SetPhiRange(0., 360.);
    if(!noCutOnChildPrompt){
      XicZeroGener->SetCutOnChild(1);   
      XicZeroGener->SetChildThetaRange(thmin,thmax);
      XicZeroGener->SetSelectAll(kTRUE);
    }
    XicZeroGener->SetForceDecay(kBeautyUpgrade);
    
    
    AliGenParam *XicZerobarGener;
    if(bEvalFormula<5)XicZerobarGener = new AliGenParam("GenParamXicZerobar",50,-4132); //antiparticle
    else if(bEvalFormula<13)XicZerobarGener = new AliGenParam("GenParamXicZerobar",15,-4132); //antiparticle
    XicZerobarGener->SetMomentumRange(0., 999.);
    XicZerobarGener->SetPtRange(0., 16.);
    XicZerobarGener->SetYRange(-1, 1);
    XicZerobarGener->SetPhiRange(0., 360.);
    if(!noCutOnChildPrompt){
      XicZerobarGener->SetCutOnChild(1);   
      XicZerobarGener->SetChildThetaRange(thmin,thmax);
      XicZerobarGener->SetSelectAll(kTRUE);
    }
    XicZerobarGener->SetForceDecay(kBeautyUpgrade);
  
  

    // add XicZero generator to cocktail
    ctl->AddGenerator(XicZeroGener,"GenParamXicZero",1);
    printf(">>>>> added HF generator GenParamXicZero \n");
    
    ctl->AddGenerator(XicZerobarGener,"XicZerobar",1);
    printf(">>>>> added HF generator GenParamXicZerobar \n");
  }
  else{
    
    AliGenParam *genBZeroGener;
    if(bEvalFormula<5)genBZeroGener = new AliGenParam("GenParamBZero",7,511); //particle
    else if(bEvalFormula<13)genBZeroGener = new AliGenParam("GenParamBZero",2,511); //particle
    genBZeroGener->SetMomentumRange(0., 999.);
    genBZeroGener->SetPtRange(0., 16.);
    genBZeroGener->SetYRange(-1, 1);
    genBZeroGener->SetPhiRange(0., 360.);
    
    
    AliGenParam *genBZerobarGener;
    if(bEvalFormula<5)genBZerobarGener = new AliGenParam("GenParamgenBZerobar",7,-511); //antiparticle
    else if(bEvalFormula<13)genBZerobarGener = new AliGenParam("GenParamBZerobar",2,-511); //antiparticle
    genBZerobarGener->SetMomentumRange(0., 999.);
    genBZerobarGener->SetPtRange(0., 16.);
    genBZerobarGener->SetYRange(-1, 1);
    genBZerobarGener->SetPhiRange(0., 360.); 
  

  // add BZero generator to cocktail
  ctl->AddGenerator(genBZeroGener,"GenParamBZero",1);
  printf(">>>>> added HF generator GenParamBZero \n");

  ctl->AddGenerator(genBZerobarGener,"GenParamBZerobar",1);
  printf(">>>>> added HF generator GenParamBZerobar \n");


      AliGenParam *genBplusGener;
    if(bEvalFormula<5)genBplusGener = new AliGenParam("GenParamBplus",7,521); //particle
    else if(bEvalFormula<13)genBplusGener = new AliGenParam("GenParamBplus",2,521); //particle
    genBplusGener->SetMomentumRange(0., 999.);
    genBplusGener->SetPtRange(0., 16.);
    genBplusGener->SetYRange(-1, 1);
    genBplusGener->SetPhiRange(0., 360.);
    
    
    AliGenParam *genBplusbarGener;
    if(bEvalFormula<5)genBplusbarGener = new AliGenParam("GenParamgenBplusbar",7,-521); //antiparticle
    else if(bEvalFormula<13)genBplusbarGener = new AliGenParam("GenParamBplusbar",2,-521); //antiparticle
    genBplusbarGener->SetMomentumRange(0., 999.);
    genBplusbarGener->SetPtRange(0., 16.);
    genBplusbarGener->SetYRange(-1, 1);
    genBplusbarGener->SetPhiRange(0., 360.);
    
      // add Bplus generator to cocktail
  ctl->AddGenerator(genBplusGener,"GenParamBplus",1);
  printf(">>>>> added HF generator GenParamBplus \n");

  ctl->AddGenerator(genBplusbarGener,"GenParamBplusbar",1);
  printf(">>>>> added HF generator GenParamBplusbar \n");


    AliGenParam *genBsGener;
    if(bEvalFormula<5)genBsGener = new AliGenParam("GenParamBs",7,531); //particle
    else if(bEvalFormula<13)genBsGener = new AliGenParam("GenParamBs",2,531); //particle
    genBsGener->SetMomentumRange(0., 999.);
    genBsGener->SetPtRange(0., 16.);
    genBsGener->SetYRange(-1, 1);
    genBsGener->SetPhiRange(0., 360.);
    
    AliGenParam *genBsbarGener;
    if(bEvalFormula<5)genBsbarGener = new AliGenParam("GenParamgenBsbar",7,-531); //antiparticle
    else if(bEvalFormula<13)genBsbarGener = new AliGenParam("GenParamBsbar",2,-531); //antiparticle
    genBsbarGener->SetMomentumRange(0., 999.);
    genBsbarGener->SetPtRange(0., 16.);
    genBsbarGener->SetYRange(-1, 1);
    genBsbarGener->SetPhiRange(0., 360.);
    
      // add Bs generator to cocktail
  ctl->AddGenerator(genBsGener,"GenParamBs",1);
  printf(">>>>> added HF generator GenParamBs \n");

  ctl->AddGenerator(genBsbarGener,"GenParamBsbar",1);
  printf(">>>>> added HF generator GenParamBsbar \n");

    
    
    AliGenParam *genLambdabGener;   
    if(bEvalFormula<5)genLambdabGener = new AliGenParam("GenParamLambdab",7,5122); //particle
    else if(bEvalFormula<13)genLambdabGener = new AliGenParam("GenParamLambdab",2,5122); //particle
    genLambdabGener->SetMomentumRange(0., 999.);
    genLambdabGener->SetPtRange(0., 16.);
    genLambdabGener->SetYRange(-1, 1);
    genLambdabGener->SetPhiRange(0., 360.);
    
    
    AliGenParam *genLambdabbarGener;
    if(bEvalFormula<5)genLambdabbarGener = new AliGenParam("GenParamgenLambdabbar",7,-5122); //antiparticle
    else if(bEvalFormula<13)genLambdabbarGener = new AliGenParam("GenParamLambdabbar",2,-5122); //antiparticle
    genLambdabbarGener->SetMomentumRange(0., 999.);
    genLambdabbarGener->SetPtRange(0., 16.);
    genLambdabbarGener->SetYRange(-1, 1);
    genLambdabbarGener->SetPhiRange(0., 360.);

  

  // add Lambdab generator to cocktail
  ctl->AddGenerator(genLambdabGener,"GenParamLambdab",1);
  printf(">>>>> added HF generator GenParamLambdab \n");

  ctl->AddGenerator(genLambdabbarGener,"GenParamLambdabbar",1);
  printf(">>>>> added HF generator GenParamLambdabbar \n");



      AliGenParam *genXibMinusGener;   
    if(bEvalFormula<5)genXibMinusGener = new AliGenParam("GenParamXibMinus",7,5132); //particle
    else if(bEvalFormula<13)genXibMinusGener = new AliGenParam("GenParamXibMinus",2,5132); //particle
    genXibMinusGener->SetMomentumRange(0., 999.);
    genXibMinusGener->SetPtRange(0., 16.);
    genXibMinusGener->SetYRange(-1, 1);
    genXibMinusGener->SetPhiRange(0., 360.);
    
    
    AliGenParam *genXibMinusbarGener;
    if(bEvalFormula<5)genXibMinusbarGener = new AliGenParam("GenParamgenXibMinusbar",7,-5132); //antiparticle
    else if(bEvalFormula<13)genXibMinusbarGener = new AliGenParam("GenParamXibMinusbar",2,-5132); //antiparticle
    genXibMinusbarGener->SetMomentumRange(0., 999.);
    genXibMinusbarGener->SetPtRange(0., 16.);
    genXibMinusbarGener->SetYRange(-1, 1);
    genXibMinusbarGener->SetPhiRange(0., 360.);

  

  // add XibMinus generator to cocktail
  ctl->AddGenerator(genXibMinusGener,"GenParamXibMinus",1);
  printf(">>>>> added HF generator GenParamXibMinus \n");

  ctl->AddGenerator(genXibMinusbarGener,"GenParamXibMinusbar",1);
  printf(">>>>> added HF generator GenParamXibMinusbar \n");


      AliGenParam *genXibZeroGener;   
    if(bEvalFormula<5)genXibZeroGener = new AliGenParam("GenParamXibZero",7,5232); //particle
    else if(bEvalFormula<13)genXibZeroGener = new AliGenParam("GenParamXibZero",2,5232); //particle
    genXibZeroGener->SetMomentumRange(0., 999.);
    genXibZeroGener->SetPtRange(0., 16.);
    genXibZeroGener->SetYRange(-1, 1);
    genXibZeroGener->SetPhiRange(0., 360.);
    
    AliGenParam *genXibZerobarGener;
    if(bEvalFormula<5)genXibZerobarGener = new AliGenParam("GenParamgenXibZerobar",7,-5232); //antiparticle
    else if(bEvalFormula<13)genXibZerobarGener = new AliGenParam("GenParamXibZerobar",2,-5232); //antiparticle
    genXibZerobarGener->SetMomentumRange(0., 999.);
    genXibZerobarGener->SetPtRange(0., 16.);
    genXibZerobarGener->SetYRange(-1, 1);
    genXibZerobarGener->SetPhiRange(0., 360.);

  // add XibZero generator to cocktail
  ctl->AddGenerator(genXibZeroGener,"GenParamXibZero",1);
  printf(">>>>> added HF generator GenParamXibZero \n");

  ctl->AddGenerator(genXibZerobarGener,"GenParamXibZerobar",1);
  printf(">>>>> added HF generator GenParamXibZerobar \n");

    
  }
 

  return ctl;
}

//-------------------------------------------------------------------------
Int_t IpXicZeroPbPb( TRandom *)
{
  return 4132;
}

//-------------------------------------------------------------------------
Double_t PtFlatXicZeroPbPb( const Double_t *px, const Double_t */*dummy*/ )
{
  return 1;
 
}

//-------------------------------------------------------------------------
Double_t YFlatXicZeroPbPb( const Double_t *py, const Double_t */*dummy*/ )
{
  return 1;
 
}

//-------------------------------------------------------------------------
Double_t V2XicZeroPbPb( const Double_t */*dummy*/, const Double_t */*dummy*/ )
{
  
  return 0.;
}
