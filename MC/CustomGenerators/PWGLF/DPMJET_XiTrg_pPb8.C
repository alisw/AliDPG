AliGenerator *GeneratorCustom(){

  Int_t sign = uidConfig % 2 == 0 ? 1 : -1;
  
  AliGenDPMjet *dpmjet  = (AliGenDPMjet*) GeneratorPhojet();
  
  dpmjet->SetTriggerParticle(sign * 3312, 1.2, 0., 4.);

  return dpmjet;
  
}
