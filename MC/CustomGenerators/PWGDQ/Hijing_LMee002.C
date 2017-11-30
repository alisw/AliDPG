AliGenerator *
GeneratorCustom()
{

  // To be done: possibility to customize
  Int_t collisionsSystem      = 400;
  Int_t centrality            = 0;
  Int_t decayMode             = 3;
  Int_t selectedMothers       = 63;
  TString paramFile           = "$ALICE_PHYSICS/PWG/Cocktail/parametrisations/pPb_5TeV.root";
  TString paramFileDir        = "5TeV_MB_Comb";
  Int_t numberOfParticles     = 1;
  Double_t minPt              = 0.;
  Double_t maxPt              = 20;
  Int_t pythiaErrorTolerance  = 2000;
  Bool_t externalDecayer      = 1;
  Bool_t decayLongLived       = 0;
  Bool_t dynamicalPtRange     = 0;
  Bool_t useYWeights          = 0;
  TString paramV2FileDir      = "";
  Bool_t toFixEP              = 0;

  // collisions systems defined:
  // 0   : pp 900 GeV
  // 100 : pp 2.76 TeV
  // 200 : pp 7 TeV
  // 300 : pPb 5.023 TeV
  // 400 : PbPb 2.76 TeV

  // selected mothers defined:
  // This function allows to select the particle which should be procude based on 1 Integer value
  // this integer value is then bitwise compare to the values in SelectParticle
  // Examples:
  // a) you would like to switch on: pi0, eta, rho0, omega, eta', phi, jpsi, sigma0 and all deltas
  //    implies you want the binary number: 00 1111 0100 0111 1111 =
  //   which translates 62591_10 (in decimal) and F47F_16 (in hexadecimal)
  // b) you would like to switch on: pi0, eta, rho0, omega, eta', phi, sigma0 and
  //    implies you want the binary number: 00 0000 0100 0011 1111 =
  //   which translates 1087_10 (in decimal) and 43F_16 (in hexadecimal)
  // c) you would like to switch on: pi0, eta, rho0, omega, eta', phi, sigma0 and all deltas
  //    implies you want the binary number: 00 1111 0100 0011 1111 =
  //   which translates 62527_10 (in decimal) and F43F_16 (in hexadecimal)
  // d) you would like to switch on: pi0, eta, rho0, omega, eta', phi
  //    implies you want the binary number: 00 0000 0000 0011 1111 =
  //   which translates 63_10 (in decimal) and 3F_16 (in hexadecimal)

  AliGenCocktail *ctl   = GeneratorCocktail("Hijing_LMee002");
 
  // Create and Initialize Generator
  AliGenEMCocktailV2 *gener     = new AliGenEMCocktailV2();
  
  //=======================================================================
  // Set External decayer
  TVirtualMCDecayer *decayer  = new AliDecayerPythia();
  if (externalDecayer) decayer->AliDecayerPythia::SetDecayerExodus();
  if (decayLongLived) decayer->AliDecayerPythia::DecayLongLivedParticles();
  decayer->Init();
  gMC->SetExternalDecayer(decayer);

  
  gener->SetParametrizationFile(paramFile);
  gener->SetParametrizationFileDirectory(paramFileDir);
  gener->SetNPart(numberOfParticles);                         // source multiplicity per event
  gener->SetPtRange(minPt,maxPt);
  gener->SetFixedEventPlane(toFixEP) ;
  gener->SetDynamicalPtRange(dynamicalPtRange);
  gener->SetUseYWeighting(useYWeights);
  gener->SetYRange(-1.,1.);
  gener->SetPhiRange(0., 360.);
  gener->SetOrigin(0.,0.,0.); 
  gener->SetSigma(0.,0.,0.);
  gener->SetVertexSmear(kPerEvent);
  gener->SetTrackingFlag(0);
  gener->SelectMotherParticles(selectedMothers);
  gener->SetCollisionSystem(collisionsSystem);                //pp 7 TeV
  gener->SetCentrality(centrality);                           // kpp
  if(paramV2FileDir.Length()>0)
    gener->SetParametrizationFileV2Directory(paramV2FileDir);
  (AliPythia::Instance())->SetMSTU(22, pythiaErrorTolerance);   // tolerance for error due to rhos
  
  if (decayMode == 1){
    gener->SetDecayMode(kGammaEM);    	// kGammaEM      => single photon
  } else if (decayMode == 2){
    gener->SetDecayMode(kElectronEM);	 // kElectronEM   => single electron
  } else if (decayMode == 3){
    gener->SetDecayMode(kDiElectronEM); // kDiElectronEM => electron-positron
  }
  gener->SetDecayer(decayer);
  gener->SetWeightingMode(kNonAnalog); 	// select weighting:
                      // kNonAnalog => weight ~ dN/dp_T
                      // kAnalog    => weight ~ 1
  gener->CreateCocktail();

  ctl->AddGenerator(gener,  "Cocktail", 1.);

   // Background events: HIJING
  AliGenerator   *hij   = GeneratorHijing();
  ctl->AddGenerator(hij,  "Hijing", 1.);
  
  return ctl;
}
