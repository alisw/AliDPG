AliGenerator *GeneratorCustom()

{

  Int_t triggerPart = 11;
  if (uidConfig%2 == 0)
      {
       triggerPart = 11;
      }
  else
      {
       triggerPart = -11;
      }

  AliGenerator* gener = GeneratorPythia6(kPythia6Tune_Perugia2011);
  
  AliGenPythia* pyth = (AliGenPythia*) gener;
  pyth->SetProcess(kPyW);
  pyth->SetTriggerParticle(triggerPart,-1.0,1.0,8,100);   // take event with electrons in EMCal
  
  return (AliGenerator*) gener;

}


