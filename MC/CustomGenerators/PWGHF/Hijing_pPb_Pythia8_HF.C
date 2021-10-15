AliGenerator *GeneratorCustom(TString opt = "")
{
  AliGenCocktail *ctl  = (AliGenCocktail*) GeneratorCocktail("Hijing_HF");
  Float_t randHF = gRandom->Rndm();
  if(randHF>0.176797){
    AliGenerator   *hij  = GeneratorHijing();
    ctl->AddGenerator(hij, "Hijing", 1.);
  }

  const Char_t *label[2][1] = {
    { "chadr PYTHIA8"},
    {"bchadr PYTHIA8" },
  };

    Int_t idecay=0;
    TString optList[1]={"had"};//could be useful in the future
    for (Int_t iopt = 0; iopt < 1; iopt++){
    if (opt.EqualTo(optList[iopt]))
      idecay = iopt;
    }
     Process_t process[2]={kPyCharmppMNRwmi, kPyBeautyppMNRwmi}; //charm or beauty
     Int_t iprocess = uidConfig % 2;

    AliGenPythiaPlus* pyth  = (AliGenPythiaPlus*)GeneratorPythia8(kPythia8Tune_Monash2013);
    pyth->SetProcess(process[iprocess]);
    pyth->SetHeavyQuarkYRange(-1.5, 1.5);
    // Configuration of decayer
  if(AliPythiaBase::Class()->GetMethodAny("Decayer")){
    printf("Force decays using ForceHadronicD of AliDecayerPythia8\n");
    pyth->SetForceDecay(kHadronicDWithout4Bodies);
   }else{
    printf("NO Force decays in the Config\n");
    }
    // Set up2date lifetimes for hadrons
    // lambda_b from PDG 2019: tau0 = 1.471 ps = 441 m/c = 0.441 mm/c
    (AliPythia8::Instance())->ReadString("5122:tau0 = 4.41000e-01");

    ctl->AddGenerator(pyth, label[iprocess][idecay], 1.);
    printf(">>>>> added HF generator %s \n", label[iprocess][idecay]);
  return ctl;
}
