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
  
  Double_t bEvalFormula=-1;
  Bool_t isEmbedding=kFALSE;
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

  AliGenParam *XicZeroGener;
  if(bEvalFormula<5)XicZeroGener = new AliGenParam("GenParamXicZero",50,4132); //particle
  else if(bEvalFormula<13)XicZeroGener = new AliGenParam("GenParamXicZero",15,4132); //particle
  XicZeroGener->SetMomentumRange(0., 999.);
  XicZeroGener->SetPtRange(0., 16.);
  XicZeroGener->SetYRange(-1, 1);
  XicZeroGener->SetPhiRange(0., 360.);
  XicZeroGener->SetCutOnChild(1);   
  XicZeroGener->SetChildThetaRange(thmin,thmax);
  XicZeroGener->SetSelectAll(kTRUE);
  XicZeroGener->SetForceDecay(kBeautyUpgrade);


  AliGenParam *XicZerobarGener;
  if(bEvalFormula<5)XicZerobarGener = new AliGenParam("GenParamXicZerobar",50,-4132); //antiparticle
  else if(bEvalFormula<13)XicZerobarGener = new AliGenParam("GenParamXicZerobar",15,-4132); //antiparticle
  XicZerobarGener->SetMomentumRange(0., 999.);
  XicZerobarGener->SetPtRange(0., 16.);
  XicZerobarGener->SetYRange(-1, 1);
  XicZerobarGener->SetPhiRange(0., 360.);
  XicZerobarGener->SetCutOnChild(1);   
  XicZerobarGener->SetChildThetaRange(thmin,thmax);
  XicZerobarGener->SetSelectAll(kTRUE);
  XicZerobarGener->SetForceDecay(kBeautyUpgrade);

  

  // add XicZero generator to cocktail
  ctl->AddGenerator(XicZeroGener,"GenParamXicZero",1);
  printf(">>>>> added HF generator GenParamXicZero \n");

  ctl->AddGenerator(XicZerobarGener,"XicZerobar",1);
  printf(">>>>> added HF generator GenParamXicZerobar \n");

 

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
