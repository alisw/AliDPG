AliGenerator *
GeneratorCustom(TString opt = "")
{

  AliGenCocktail *ctl   = GeneratorCocktail("Hijing_LMee001");

  // Background events: HIJING
  AliGenerator   *hij   = GeneratorHijing();
  ctl->AddGenerator(hij,  "Hijing", 1.);

  // LMee cocktail settings:
  Int_t   nPart  = 1;
  Float_t minPt  = 0;
  Float_t maxPt  = 25;
  Float_t minRap = -1.2;
  Float_t maxRap = 1.2;
  Float_t phiMin = 0.;
  Float_t phiMax = 360.;
  Float_t thmin  = (180./TMath::Pi())*2.*atan(exp(minRap));  
  Float_t thmax  = (180./TMath::Pi())*2.*atan(exp(maxRap));  
  Weighting_t weightMode = kNonAnalog;
  AliGenEMlib *emlib = new AliGenEMlib();

  AliDecayerPythia* decayerInt = NULL; 
  AliDecayerPythia* decayerExt = NULL;

  // set internal decayer (for AliGenParam with Exodus only)
  if(opt.Contains("Exodus")){
    decayerInt = new AliDecayerPythia();
    Printf("Use Exodus decayer for cocktail");
    decayerInt->SetDecayerExodus();
    decayerInt->SetForceDecay(kDiElectronEM);
    decayerInt->Init();
  }
  
  // set external decayer (if Exodus not used, to initialize AliGen Param properly)
  decayerExt = new AliDecayerPythia();
  decayerExt->SetForceDecay(kDiElectronEM);
  decayerExt->Init();
  gMC->SetExternalDecayer(decayerExt);
    
  // Pi0
  AliGenParam *pizero = new AliGenParam(nPart, emlib, AliGenEMlib::kPizero,"pizero");

  pizero->SetPtRange(minPt, maxPt);
  pizero->SetYRange(minRap, maxRap);
  pizero->SetPhiRange(phiMin, phiMax);
  pizero->SetWeighting(weightMode);  // flat pt 
  pizero->SetForceDecay(kDiElectronEM);
  if(decayerInt)
    pizero->SetDecayer(decayerInt);
  pizero->Init();
    
  // Eta
  AliGenParam *eta = new AliGenParam(nPart,emlib, AliGenEMlib::kEta,"eta");
  eta->SetPtRange(minPt, maxPt);
  eta->SetYRange(minRap, maxRap);
  eta->SetPhiRange(phiMin, phiMax);
  eta->SetWeighting(weightMode);  // flat pt 
  eta->SetForceDecay(kDiElectronEM);
  if(decayerInt)
    eta->SetDecayer(decayerInt);
  eta->Init();

  // Etaprime
  AliGenParam *etaprime = new AliGenParam(nPart,emlib, AliGenEMlib::kEtaprime,"etaprime");
  etaprime->SetPtRange(minPt, maxPt);
  etaprime->SetYRange(minRap, maxRap);
  etaprime->SetPhiRange(phiMin, phiMax);
  etaprime->SetWeighting(weightMode);  // flat pt 
  etaprime->SetForceDecay(kDiElectronEM);
  if(decayerInt)
    etaprime->SetDecayer(decayerInt);
  etaprime->Init();

  // Rho 
  AliGenParam *rho = new AliGenParam(nPart,emlib, AliGenEMlib::kRho0,"rho");
  rho->SetPtRange(minPt, maxPt);
  rho->SetYRange(minRap, maxRap);
  rho->SetPhiRange(phiMin, phiMax);
  rho->SetWeighting(weightMode);  // flat pt 
  rho->SetForceDecay(kDiElectronEM);
  if(decayerInt)
    rho->SetDecayer(decayerInt);
  rho->Init();
  
  // Omega
  AliGenParam *omega = new AliGenParam(nPart,emlib, AliGenEMlib::kOmega,"omega");  
  omega->SetPtRange(minPt, maxPt);
  omega->SetYRange(minRap, maxRap);
  omega->SetPhiRange(phiMin, phiMax);
  omega->SetWeighting(weightMode);  // flat pt 
  omega->SetForceDecay(kDiElectronEM);
  if(decayerInt)
    omega->SetDecayer(decayerInt);
  omega->Init();

  //Phi
  AliGenParam *phi = new AliGenParam(nPart,emlib,AliGenEMlib::kPhi,"phi");
  phi->SetPtRange(minPt, maxPt);
  phi->SetYRange(minRap, maxRap);
  phi->SetPhiRange(phiMin, phiMax);
  phi->SetWeighting(weightMode);  // flat pt 
  phi->SetForceDecay(kDiElectronEM);
  if(decayerInt)
    phi->SetDecayer(decayerInt);
  phi->Init();

  // J/psi 
  AliGenParam * jpsi = new AliGenParam(nPart, emlib, AliGenEMlib::kJpsi, "Flat");//, "jpsi");
  jpsi->SetPtRange(minPt, maxPt);
  jpsi->SetYRange(minRap, maxRap);
  jpsi->SetPhiRange(phiMin, phiMax);
  jpsi->SetWeighting(weightMode);  // flat pt 
  jpsi->SetForceDecay(kDiElectron);
  // if(decayerInt) // not working in current implementation ((TDatabasePDG::Instance()->GetParticle(443))->Width() = 0) 
  //   jpsi->SetDecayer(decayerInt); 
  jpsi->Init();


  ////////////////////////////////////////////
  //    Pythia cc->ee
  ////////////////////////////////////////////
  AliGenPythia* pythiaCC = new AliGenPythia(-1);
  pythiaCC->SetTitle("PYTHIA-HF-Cdiele");
  pythiaCC->SetMomentumRange(0, 999999.);
  pythiaCC->SetThetaRange(0., 180.);
  pythiaCC->SetPtRange(0,1000.);
  pythiaCC->SetProcess(kPyCharmppMNRwmi);
  pythiaCC->SetEnergyCMS(energyConfig);
  pythiaCC->SetForceDecay(kSemiElectronic);
  //    Tune                                                                
  //    320     Perugia 0 
  //    350     Perugia 2011
  pythiaCC->SetTune(350);
  pythiaCC->UseNewMultipleInteractionsScenario();
  //
  //    decays 
  pythiaCC->SetCutOnChild(2);
  pythiaCC->SetPdgCodeParticleforAcceptanceCut(11);
  pythiaCC->SetChildYRange(-1.2,1.2);
  pythiaCC->SetChildPtRange(0,10000.);
  pythiaCC->SetStackFillOpt(AliGenPythia::kHeavyFlavor);

  ////////////////////////////////////////////
  //    Pythia bb->ee
  ////////////////////////////////////////////
  AliGenPythia* pythiaBB = new AliGenPythia(-1);
  pythiaBB->SetTitle("PYTHIA-HF-Bdiele");
  pythiaBB->SetMomentumRange(0, 999999.);
  pythiaBB->SetThetaRange(0., 180.);
  pythiaBB->SetPtRange(0,1000.);
  pythiaBB->SetProcess(kPyBeautyppMNRwmi);
  pythiaBB->SetEnergyCMS(energyConfig);
  pythiaBB->SetForceDecay(kSemiElectronic);
  //    Tune                                                                
  //    320     Perugia 0 
  //    350     Perugia 2011
  pythiaBB->SetTune(350);
  pythiaBB->UseNewMultipleInteractionsScenario();
  //
  //    decays 
  pythiaBB->SetCutOnChild(2);
  pythiaBB->SetPdgCodeParticleforAcceptanceCut(11);
  pythiaBB->SetChildYRange(-1.2,1.2);
  pythiaBB->SetChildPtRange(0,10000.);
  pythiaBB->SetStackFillOpt(AliGenPythia::kHeavyFlavor);


  ////////////////////////////////////////////
  //    Pythia b,c->e
  ////////////////////////////////////////////
  AliGenPythia* pythiaB= new AliGenPythia(-1);
  pythiaB->SetTitle("PYTHIA-HF-Bele");
  pythiaB->SetMomentumRange(0, 999999.);
  pythiaB->SetThetaRange(0., 180.);
  pythiaB->SetPtRange(0,1000.);
  pythiaB->SetProcess(kPyBeautyppMNRwmi);
  pythiaB->SetEnergyCMS(energyConfig);
  //    Tune                                                                
  //    320     Perugia 0 
  //    350     Perugia 2011
  pythiaB->SetTune(350);
  pythiaB->UseNewMultipleInteractionsScenario();
  //
  //    decays 
  pythiaB->SetCutOnChild(1);
  pythiaB->SetPdgCodeParticleforAcceptanceCut(11);
  pythiaB->SetChildYRange(-1.2,1.2);
  pythiaB->SetChildPtRange(0,10000.);
  pythiaB->SetStackFillOpt(AliGenPythia::kHeavyFlavor);


  ////////////////////////////////////////////
  //  Create cocktail
  ////////////////////////////////////////////
  
  ctl->AddGenerator(pizero,"pizero", 1.);
  ctl->AddGenerator(eta,"eta", 1.);
  ctl->AddGenerator(etaprime,"etaprime", 1.);
  ctl->AddGenerator(rho,"rho", 1.);
  ctl->AddGenerator(omega,"omega", 1.);
  ctl->AddGenerator(phi,"phi", 1.);
  ctl->AddGenerator(jpsi,"jpsi", 1.);

  // HF part

  // centrality dependent yield (from PWGHF/Hijing_HF001)
  TFormula *formula = new TFormula("Signals","max(1.,120.*(x<5.)+80.*(1.-x/20.)*(x>5.)*(x<11.)+240.*(1.-x/13.)*(x>11.))");

  Int_t flag = (Int_t)gRandom->Uniform(0,100);
  if(flag>=0 && flag<20){ 
    ctl->AddGenerator(pythiaCC,"Pythia CC",1.,formula);
  }else if(flag>=20 && flag<40){ 
    ctl->AddGenerator(pythiaBB,"Pythia BB",1.,formula);
  }else if(flag>=40 && flag<100){ 
    ctl->AddGenerator(pythiaB,"Pythia B",1.,formula);
  }
  
  return ctl;
}
