AliGenerator *
GeneratorCustom(TString opt = "")
{
  AliGenerator *py8 = GeneratorPythia8(kPythia8Tune_Monash2013);
  
  // select events based on charged particle multiplicity at forward rapidity
  ((AliGenPythiaPlus*)py8)->SetTriggerMultiplicityEtaRange(70, 2.8, 5.1);
  
  return py8;
}
