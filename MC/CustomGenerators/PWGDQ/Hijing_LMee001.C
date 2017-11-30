AliGenerator *
GeneratorCustom()
{

  AliGenCocktail *ctl   = GeneratorCocktail("Hijing_LMee001");

  // Background events: HIJING
  AliGenerator   *hij   = GeneratorHijing();
  ctl->AddGenerator(hij,  "Hijing", 1.);

  // LMee cocktail settings:
  // number of particles per event is calculated from formula
  TFormula* formula = new TFormula("formula", "20.+ 80.*exp(- 0.5 * x * x / 5.12 / 5.12)");
  TFormula* one     = new TFormula("one", "1");
  Int_t   nPart  = 1;
  Float_t minPt  = 0;
  Float_t maxPt  = 50;
  Float_t minRap = -1.2;
  Float_t maxRap = 1.2;
  Float_t phiMin = 0.;
  Float_t phiMax = 360.;
  Float_t thmin  = (180./TMath::Pi())*2.*atan(exp(minRap));  
  Float_t thmax  = (180./TMath::Pi())*2.*atan(exp(maxRap));  
  Weighting_t weightMode = kNonAnalog;
  AliGenEMlib *emlib = new AliGenEMlib();

  // set external decayer
  AliDecayerPythia* decayer = new AliDecayerPythia();
  decayer->SetDecayerExodus();
  decayer->SetForceDecay(kDiElectronEM);
  decayer->Init();
  gMC->SetExternalDecayer(decayer);
  cout<<"MW: "<<gMC->GetDecayer()->GetName()<<endl;
  
  // Pi0
  AliGenParam *pizero = new AliGenParam(nPart, emlib, AliGenEMlib::kPizero,"pizero");

  pizero->SetPtRange(minPt, maxPt);
  pizero->SetYRange(minRap, maxRap);
  pizero->SetPhiRange(phiMin, phiMax);
  pizero->SetWeighting(weightMode);  // flat pt 
  pizero->SetForceDecay(kDiElectronEM);
  pizero->Init();
    
  // Eta
  AliGenParam *eta = new AliGenParam(nPart,emlib, AliGenEMlib::kEta,"eta");
  eta->SetPtRange(minPt, maxPt);
  eta->SetYRange(minRap, maxRap);
  eta->SetPhiRange(phiMin, phiMax);
  eta->SetWeighting(weightMode);  // flat pt 
  eta->SetForceDecay(kDiElectronEM);
  eta->Init();

  // Etaprime
  AliGenParam *etaprime = new AliGenParam(nPart,emlib, AliGenEMlib::kEtaprime,"etaprime");
  etaprime->SetPtRange(minPt, maxPt);
  etaprime->SetYRange(minRap, maxRap);
  etaprime->SetPhiRange(phiMin, phiMax);
  etaprime->SetWeighting(weightMode);  // flat pt 
  etaprime->SetForceDecay(kDiElectronEM);
  etaprime->Init();

  // Rho 
  AliGenParam *rho = new AliGenParam(nPart,emlib, AliGenEMlib::kRho0,"rho");
  rho->SetPtRange(minPt, maxPt);
  rho->SetYRange(minRap, maxRap);
  rho->SetPhiRange(phiMin, phiMax);
  rho->SetWeighting(weightMode);  // flat pt 
  rho->SetForceDecay(kDiElectronEM);
  rho->Init();
  
  // Omega
  AliGenParam *omega = new AliGenParam(nPart,emlib, AliGenEMlib::kOmega,"omega");  
  omega->SetPtRange(minPt, maxPt);
  omega->SetYRange(minRap, maxRap);
  omega->SetPhiRange(phiMin, phiMax);
  omega->SetWeighting(weightMode);  // flat pt 
  omega->SetForceDecay(kDiElectronEM);
  omega->Init();

  //Phi
  AliGenParam *phi = new AliGenParam(nPart,emlib,AliGenEMlib::kPhi,"phi");
  phi->SetPtRange(minPt, maxPt);
  phi->SetYRange(minRap, maxRap);
  phi->SetPhiRange(phiMin, phiMax);
  phi->SetWeighting(weightMode);  // flat pt 
  phi->SetForceDecay(kDiElectronEM);
  phi->Init();

  // J/psi 
  AliGenParam * jpsi = new AliGenParam(nPart, emlib, AliGenMUONlib::kJpsi, "Flat");//, "jpsi");
  jpsi->SetPtRange(minPt, maxPt);
  jpsi->SetYRange(minRap, maxRap);
  jpsi->SetPhiRange(phiMin, phiMax);
  jpsi->SetWeighting(weightMode);  // flat pt 
  jpsi->SetForceDecay(kDiElectron);
  jpsi->Init();

  // gamma
  AliGenBox* gamma = new AliGenBox(1); 
  gamma->SetPart(22);
  gamma->SetPtRange(minPt, maxPt);
  gamma->SetThetaRange(thmin, thmax);

  // add LMee cocktail
  ctl->AddGenerator(pizero,"pizero", nPart, formula);
  ctl->AddGenerator(eta,"eta", nPart, formula);
  ctl->AddGenerator(etaprime,"etaprime", nPart, formula);
  ctl->AddGenerator(rho,"rho", nPart, formula);
  ctl->AddGenerator(omega,"omega", nPart, formula);
  ctl->AddGenerator(phi,"phi", nPart, formula);
  ctl->AddGenerator(jpsi,"jpsi", nPart, formula);
  ctl->AddGenerator(gamma, "gamma",  nPart, formula);
  return ctl;
}
