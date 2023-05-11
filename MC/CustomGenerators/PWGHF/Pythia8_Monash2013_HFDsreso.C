AliGenerator *GeneratorCustom(TString opt = "Ds2")
{

  //Switches for prompt/nonprompt
  Process_t process[2] = {kPyCharmppMNRwmi, kPyBeautyppMNRwmi}; //charm or beauty
  Int_t iprocess = uidConfig % 2;

  AliGenPythiaPlus* pyth = (AliGenPythiaPlus*)GeneratorPythia8(kPythia8Tune_Monash2013);
  pyth->SetProcess(process[iprocess]);
  pyth->SetHeavyQuarkYRange(-1.5, 1.5);
  // Pt transfer of the hard scattering (set for all cases)
  pyth->SetPtHard(pthardminConfig, pthardmaxConfig);

  //setting multiplicity threshold
  TString multthr = opt;
  Int_t stop = opt.First('_');
  if(stop==-1) {
    printf("Warning! Multiplicity threshold has to be set via the macro parameter!\n");
  return 0x0;
  }
  multthr.Remove(0,stop+1);
  pyth->SetTriggerChargedMultiplicity(multthr.Atoi(), 1.2);

  printf(">>>>> HF generator with mult threshold %d\n", multthr.Atoi());


  // Configuration of decayer
  pyth->SetForceDecay(kHadronicDWithout4Bodies);
  if (iprocess == 0) {
    if (opt.EqualTo("Ds1")){
      pyth->SetReplaceD(413, 10433);
    } else if (opt.EqualTo("Ds2")) {
      pyth->SetReplaceD(411, 435);
    }
  }
  return pyth;
}
