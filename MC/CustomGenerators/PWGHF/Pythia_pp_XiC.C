AliGenerator *GeneratorCustom(TString opt =)
{
  Int_t iprocess = uidConfig % 4;

  Int_t particleTrigger[4]={4232, -4232, 4132, -4132};

  AliGenCocktail *ctl = static_cast<AliGenCocktail*>(GeneratorCocktail("Perugia8MonashTune_HF"));			
  AliGenPythiaPlus *pythia = (AliGenPythiaPlus*) GeneratorPythia8(kPythia8Tune_Monash2013);
  pythia->SetProcess(kPyCharmppMNRwmi);
  pythia->SetHeavyQuarkYRange(-1.5, 1.5); 
  pythia->SetTriggerParticle(particleTrigger[iprocess],1.5); //Xic+ //pdgcode,eta,ptmin,ptmax
  pythia->SetTriggerParticleMinPt(1.);
  
  
//  AliGenEvtGen *gene = new AliGenEvtGen();
//  gene->SetUserDecayTable("./XIC.DEC");
  //gene->SetParticleSwitchedOff(AliGenEvtGen::kAllPart);

  //ctl->AddGenerator(pythia, Form("MC %s",label[iparticle].Data()),1.);
  ctl->AddGenerator(pythia, "MC XiC0, XiC+ PYTHIA 8",1.);
  

  return ctl;
}
