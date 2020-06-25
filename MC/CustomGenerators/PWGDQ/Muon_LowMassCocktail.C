AliGenerator* GeneratorCustom()
{
 
  AliGenCocktail *ctl   = GeneratorCocktail("Pythia6_Perugia2011_LMRmumu");
  AliGenerator   *pyt   = GeneratorPythia6(kPythia6Tune_Perugia2011);
  //ctl->AddGenerator(pyt,  "Pythia6", 1.);

  enum {kEta2Body, kEtaDalitz, kRho2Body, kOmega2Body, kOmegaDalitz, kPhi2Body, kEtaPrimeDalitz, kPionLMR, kKaonLMR};
  Int_t   nPart  = 1;
  Float_t minPt  = 0;
  Float_t maxPt  = 50;
  Float_t minRap = -4.5;
  Float_t maxRap = -2.0;
  Float_t phiMin = 0.;
  Float_t phiMax = 360.;

  if(uidConfig % 100 <14){
    AliGenMUONLMR *eta_direct = new AliGenMUONLMR();
    eta_direct->SetPhiRange(phiMin,phiMax);
    eta_direct->SetYRange(minRap,maxRap);
    eta_direct->SetPtRange(minPt,maxPt);
    //eta_direct->SetChildThetaRange(169.0,180.0);
    eta_direct->SetCutOnChild(1);
    eta_direct->SetCMSEnergy(AliGenMUONLMR::kCMS7000GeV);
    eta_direct->GenerateSingleProcess(kEta2Body); eta_direct->SetPtParams(kEta2Body,1.,1.11794,2.78373); eta_direct->SetYParams(kEta2Body,1.,0.53516,4.07123,0.0);
    ctl->AddGenerator(eta_direct, "EtaDirect", 1.);
  }
  if(14 <= uidConfig % 100 && uidConfig % 100 <28){
    AliGenMUONLMR *eta_dalitz = new AliGenMUONLMR();
    eta_dalitz->SetPhiRange(phiMin,phiMax);
    eta_dalitz->SetYRange(minRap,maxRap);
    eta_dalitz->SetPtRange(minPt,maxPt);
    //eta_dalitz->SetChildThetaRange(169.0,180.0);
    eta_dalitz->SetCutOnChild(1);
    eta_dalitz->SetCMSEnergy(AliGenMUONLMR::kCMS7000GeV);
    eta_dalitz->GenerateSingleProcess(kEtaDalitz); eta_dalitz->SetPtParams(kEtaDalitz,1.,1.11794,2.78373); eta_dalitz->SetYParams(kEtaDalitz,1.,0.53516,4.07123,0.0);
    ctl->AddGenerator(eta_dalitz, "EtaDalitz", 1.);  
  }
  else if(28 <= uidConfig % 100 && uidConfig % 100 < 42){
    AliGenMUONLMR *etaprime = new AliGenMUONLMR();
    etaprime->SetPhiRange(phiMin,phiMax);
    etaprime->SetYRange(minRap,maxRap);
    etaprime->SetPtRange(minPt,maxPt);
    //etaprime->SetChildThetaRange(169.0,180.0);
    etaprime->SetCutOnChild(1);
    etaprime->SetCMSEnergy(AliGenMUONLMR::kCMS7000GeV);
    etaprime->GenerateSingleProcess(kEtaPrimeDalitz); etaprime->SetPtParams(kEtaPrimeDalitz,1.,1.32570,2.79526); etaprime->SetYParams(kEtaPrimeDalitz,1.,0.59152,4.11771,0.0); 
    ctl->AddGenerator(etaprime, "EtaprimeDalitz", 1.);
  }
  else if(42 <= uidConfig % 100 && uidConfig % 100 < 56){
    AliGenMUONLMR *omega_dalitz = new AliGenMUONLMR();
    omega_dalitz->SetPhiRange(phiMin,phiMax);
    omega_dalitz->SetYRange(minRap,maxRap);
    omega_dalitz->SetPtRange(minPt,maxPt);
    //omega_dalitz->SetChildThetaRange(169.0,180.0);
    omega_dalitz->SetCutOnChild(1);
    omega_dalitz->SetCMSEnergy(AliGenMUONLMR::kCMS7000GeV);
    omega_dalitz->GenerateSingleProcess(kOmegaDalitz); omega_dalitz->SetPtParams(kOmegaDalitz,1.,1.23232,2.78348); omega_dalitz->SetYParams(kOmegaDalitz,1.,0.57226,4.09762,0.0);
    ctl->AddGenerator(omega_dalitz, "OmegaDalitz", 1.);  
  }
  else if(56 <= uidConfig % 100 && uidConfig % 100 < 70){
    AliGenMUONLMR *omega_direct = new AliGenMUONLMR();
    omega_direct->SetPhiRange(phiMin,phiMax);
    omega_direct->SetYRange(minRap,maxRap);
    omega_direct->SetPtRange(minPt,maxPt);
    //omega_direct->SetChildThetaRange(169.0,180.0);
    omega_direct->SetCutOnChild(1);
    omega_direct->SetCMSEnergy(AliGenMUONLMR::kCMS7000GeV);
    omega_direct->GenerateSingleProcess(kOmega2Body); omega_direct->SetPtParams(kOmega2Body,1.,1.23232,2.78348); omega_direct->SetYParams(kOmega2Body,1.,0.57226,4.09762,0.0);
    ctl->AddGenerator(omega_direct, "OmegaDirect", 1.);
  }
  else if(70 <= uidConfig % 100 && uidConfig % 100 < 84){
    AliGenMUONLMR* rho = new AliGenMUONLMR();
    rho->SetPhiRange(phiMin,phiMax);
    rho->SetYRange(minRap,maxRap);
    rho->SetPtRange(minPt,maxPt);
    //rho->SetChildThetaRange(169.0,180.0);
    rho->SetCutOnChild(1);
    rho->SetCMSEnergy(AliGenMUONLMR::kCMS7000GeV);
    rho->GenerateSingleProcess(kRho2Body); rho->SetPtParams(kRho2Body,1.,1.36203,2.73224); rho->SetYParams(kRho2Body,1.,0.61905,3.85922,0.0); 
    ctl->AddGenerator(rho, "RhoDirect", 1.);
  }
  else{
    AliGenMUONLMR *phi = new AliGenMUONLMR();
    phi->SetPhiRange(phiMin,phiMax);
    phi->SetYRange(minRap,maxRap);
    phi->SetPtRange(minPt,maxPt);
    //phi->SetChildThetaRange(169.0,180.0);
    phi->SetCutOnChild(1);
    phi->SetCMSEnergy(AliGenMUONLMR::kCMS7000GeV);
    phi->GenerateSingleProcess(kPhi2Body); phi->SetPtParams(kPhi2Body,1.,1.36203,2.73224); phi->SetYParams(kPhi2Body,1.,0.61905,3.85922,0.0);
    ctl->AddGenerator(phi, "PhiDirect", 1.);
  }
  
  return ctl;


}
