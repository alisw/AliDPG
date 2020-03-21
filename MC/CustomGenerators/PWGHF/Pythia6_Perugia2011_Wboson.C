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

  AliGenPythia* pyth = GeneratorPythia6(kPythia6Tune_Perugia2011);
  pyth->SetProcess(kPyW);
  pyth->SetTriggerParticle(11,-1.0,1.0,8,100);   // take event that electrons in EMCal
  return pyth;

}


