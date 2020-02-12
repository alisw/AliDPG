AliGenerator *GeneratorCustom(TString opt = "")
{
  Int_t iprocess = uidConfig % 2;
 
  Int_t particleTrigger[4]={4232, -4232, 4132, -4132};
  Int_t iparticle =0;
  Int_t idxParticleTrigger=0;
  Float_t randPart = gRandom->Rndm();
  if(randPart<=0.3) {
    iparticle=0;
    idxParticleTrigger=0;
  }
  else if (randPart>0.3 && randPart<=0.6) {
    iparticle=0;
    idxParticleTrigger=1;
  }
  else if (randPart>0.6 && randPart<=0.8) {
    iparticle=1;
    idxParticleTrigger=2;
  }
  else if (randPart>0.8) {
    iparticle=1;
    idxParticleTrigger=3;
  }

  TString label[2]={"XiCPlus to hadrons", "XiC0 to hadrons"};
  TString strLabFlav="charm";
  
  AliGenCocktail *ctl = static_cast<AliGenCocktail*>(GeneratorCocktail("Perugia2011_HF"));			
  AliGenPythia *pythia = (AliGenPythia*) GeneratorPythia6(kPythia6Tune_Perugia2011);
  if(iprocess==0){
    pythia->SetProcess(kPyCharmppMNRwmi);
  }
  else {
    pythia->SetProcess(kPyBeautyppMNRwmi);
    strLabFlav="beauty";
  }
  pythia->SetHeavyQuarkYRange(-1.5, 1.5); 
  pythia->SetTriggerY(1.0);
  pythia->SetTriggerParticle(particleTrigger[idxParticleTrigger],2.,1.,100); //Xic //pdgcode,eta,ptmin,ptmax
  
  ctl->AddGenerator(pythia, Form("MC %s",strLabFlav.Data(),label[iparticle].Data()),1.);

  return ctl;
}
