AliGenerator *GeneratorCustom(TString opt = "")
{
  Int_t iprocess = uidConfig % 4;
 
  Int_t particleTrigger[4]={4232, -4232, 4132, -4132};
  Int_t iparticle =0;
  if (iprocess>1) iparticle=1;

  TString label[2]={"XiCPlus to hadrons", "XiC0 to hadrons"};

  AliGenCocktail *ctl = static_cast<AliGenCocktail*>(GeneratorCocktail("Perugia2011_HF"));			
  AliGenPythia *pythia = (AliGenPythia*) GeneratorPythia6(kPythia6Tune_Perugia2011);
  pythia->SetProcess(kPyCharmppMNRwmi);
  pythia->SetHeavyQuarkYRange(-1.5, 1.5); 
  pythia->SetTriggerY(1.0);
  pythia->SetTriggerParticle(particleTrigger[iprocess],1.5,1.,100); //Xic //pdgcode,eta,ptmin,ptmax
  
  ctl->AddGenerator(pythia, Form("MC %s",label[iparticle].Data()),1.);

  return ctl;
}
