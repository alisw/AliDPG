AliGenerator *GeneratorCustom()
{

  Int_t triggerPart = 11;
 
  AliGenerator* gener = GeneratorPythia6(kPythia6Tune_Perugia2011);
  
  AliGenPythia* pyth = (AliGenPythia*) gener;
  pyth->SetProcess(kPyZ);
  pyth->SetTriggerParticle(triggerPart,-1.0,1.0,8,100); 
  
  return gener;

}


