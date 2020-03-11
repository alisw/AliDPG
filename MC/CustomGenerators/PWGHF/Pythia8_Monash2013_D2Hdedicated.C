AliGenerator *GeneratorCustom(TString opt = "")
{
  Char_t* label[2] = {"DsDedicated", "DsDplusDedicated"};

  Int_t channelOption = 0;
  for (Int_t iopt = 0; iopt < 2; iopt++ ) {
    if (opt.EqualTo(label[iopt]))
      channelOption = iopt;
  }

  //Switches for prompt/nonprompt, sign of trigger particle, trigger particle
  Int_t triggerParticleFirst[2] = {431, 431};
  Int_t triggerParticleSecond[2] = {0, 411};
  Process_t process; //charm or beauty
  Int_t sign = 0; //Sign of trigger particle
  Int_t triggerPart = triggerParticleFirst[channelOption]; //trigger particle

  //switch prompt/nonprompt
  if (uidConfig%2 == 0)
    process = kPyCharmppMNRwmi;
  else
    process = kPyBeautyppMNRwmi;

  //switch sign of trigger particle
  if (uidConfig%4 <= 1)
    sign = 1;
  else
    sign = -1;

  //switch trigger particle (if 2 enabled)
  if (triggerParticleSecond[channelOption] != 0) {
    if (uidConfig%8 <= 3)
      triggerPart = triggerParticleFirst[channelOption];
    else
      triggerPart = triggerParticleSecond[channelOption];
  }

  AliGenPythiaPlus* pyth = (AliGenPythiaPlus*)GeneratorPythia8(kPythia8Tune_Monash2013);
  pyth->SetTriggerParticle(sign * triggerPart, 999);
  pyth->SetProcess(process);
  pyth->SetHeavyQuarkYRange(-1.5, 1.5);
  // Pt transfer of the hard scattering (set for all cases)
  pyth->SetPtHard(pthardminConfig, pthardmaxConfig);

  // Configuration of decayer
  if(AliPythiaBase::Class()->GetMethodAny("Decayer")){
    printf("Force decays using ForceHadronicD of AliDecayerPythia8\n");
    pyth->SetForceDecay(kHadronicDWithV0);
  }else{
    printf("Force decays in the Config\n");
    //add D+ decays absent in PYTHIA8 decay table and set BRs from PDG for other
    (AliPythia8::Instance())->ReadString("411:oneChannel = 1 0.0752 0 -321 211 211");
    (AliPythia8::Instance())->ReadString("411:addChannel = 1 0.0104 0 -313 211");
    (AliPythia8::Instance())->ReadString("411:addChannel = 1 0.0156 0 311 211");
    // D+ decays
    (AliPythia8::Instance())->ReadString("411:onMode = off");
    (AliPythia8::Instance())->ReadString("411:onIfMatch = 321 211 211");
    (AliPythia8::Instance())->ReadString("411:onIfMatch = 313 211");
    (AliPythia8::Instance())->ReadString("313:onMode = off");
    (AliPythia8::Instance())->ReadString("313:onIfAll = 321 211");
    // Ds decays
    (AliPythia8::Instance())->ReadString("431:onMode = off");
    (AliPythia8::Instance())->ReadString("431:onIfMatch = 333 211");
    (AliPythia8::Instance())->ReadString("431:onIfMatch = 321 313");
    (AliPythia8::Instance())->ReadString("333:onMode = off");
    (AliPythia8::Instance())->ReadString("333:onIfAll = 321 321");
  }

  // Set up2date lifetimes for hadrons
  // lambda_b from PDG 2019: tau0 = 1.471 ps = 441 m/c = 0.441 mm/c
  (AliPythia8::Instance())->ReadString("5122:tau0 = 4.41000e-01");

  return pyth;
}
