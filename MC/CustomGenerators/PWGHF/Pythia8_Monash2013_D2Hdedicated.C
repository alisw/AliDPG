AliGenerator *GeneratorCustom(TString opt = "")
{

  const Int_t nOptions=10;

  Char_t* label[nOptions] = {"DsDedicated", "DsDplusDedicated", "XicDedicated", "LcTopKpiDedicated", "LcTopK0sDedicated", "XicSemilepDedicated", "OmegacDedicated","LcToLpiDedicated","OmegacSemilepDedicated","OmegacToXiPiDedicated"};

  Int_t channelOption = 0;
  for (Int_t iopt = 0; iopt < nOptions; iopt++ ) {
    if (opt.EqualTo(label[iopt]))
      channelOption = iopt;
  }

  //Switches for prompt/nonprompt, sign of trigger particle, trigger particle
  Int_t triggerParticleFirst[nOptions] = {431, 431, 4132, 4122, 4122, 4132, 4332, 4122, 4332, 4332};
  Int_t triggerParticleSecond[nOptions] = {0, 411, 4232, 0, 0, 4232, 0, 0, 0, 0};
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
  if (channelOption == 1) {
    // Ds / D+ sharing
    if (uidConfig%8 <= 3)
      triggerPart = triggerParticleFirst[channelOption];
    else
      triggerPart = triggerParticleSecond[channelOption];
  }
  else if (channelOption == 2) {
    // Xic0 / Xic+ sharing
    if (uidConfig%12 <= 3) // Xic0 in 1/3 of the events
      triggerPart = triggerParticleFirst[channelOption];
    else
      triggerPart = triggerParticleSecond[channelOption];
	}
	else if (channelOption == 5) {
    // Xic0 / Xic+ sharing
    if (uidConfig%12 <= 7) // Xic0 in 2/3 of the events
      triggerPart = triggerParticleFirst[channelOption];
    else
      triggerPart = triggerParticleSecond[channelOption];
  }

  AliGenPythiaPlus* pyth = (AliGenPythiaPlus*)GeneratorPythia8(kPythia8Tune_Monash2013);
  Double_t etaMax = 999.;
  if(channelOption == 2)
    etaMax = 2.;
  pyth->SetTriggerParticle(sign * triggerPart, etaMax);
  pyth->SetProcess(process);
  pyth->SetHeavyQuarkYRange(-1.5, 1.5);
  if(AliGenPythiaPlus::Class()->GetMethodAny("SetTriggerY"))
    pyth->SetTriggerY(1.0);

  // Configuration of decayer (depending on AliRoot version)
  TList *argsList = AliDecayerPythia8::Class()->GetMethodAny("ForceHadronicD")->GetListOfMethodArgs();
  int argsNum = 0;
  if(argsList)
    argsNum = argsList->GetSize();

  // Lc decays
  if(AliPythiaBase::Class()->GetMethodAny("Decayer")){
    printf("Force decays using ForceHadronicD of AliDecayerPythia8\n");
    if(channelOption==3){
      pyth->SetForceDecay(kLcpKpi);
    }
    else if(channelOption==4){
      pyth->SetForceDecay(kLcpK0S);
    }
    else if(channelOption==5){
      if(argsNum>=4) {
        pyth->SetForceDecay(kXic0Semileptonic);
      }
      else {
        printf("ERROR: AliRoot version does not contain option for Xic semileptonic decay!\n");
        return 0x0;
      }
    }
    else if(channelOption==7) {
      pyth->SetForceDecay(kLcLpi);
    }
    else if(channelOption==8){  // Omegac0 -> Omegac + e
        if (argsNum>=6){
        pyth->SetForceDecay(kOmegac0Semileptonic);
        }
        else {
            printf("ERROR: AliRoot version does not contain option for Omegac semileptonic decay !\n");
            return 0x0;
        }
    }
    else if(channelOption==9){ //Omegac0 -> Xi pi
      pyth->SetForceDecay(kOmegac0ToXiPi);
    }
    else if(channelOption<2 && argsNum>=5){
      pyth->SetForceDecay(kHadronicDWithout4BodiesDsPhiPi);
    }
    else {
      pyth->SetForceDecay(kHadronicDWithV0);
    }
  }
  else{
    printf("Force decays in the Config\n");
    //add D+ decays absent in PYTHIA8 decay table and set BRs from PDG for other
    (AliPythia8::Instance())->ReadString("411:oneChannel = 1 0.0752 0 -321 211 211");
    (AliPythia8::Instance())->ReadString("411:addChannel = 1 0.0104 0 -313 211");
    (AliPythia8::Instance())->ReadString("411:addChannel = 1 0.0156 0 311 211");
    (AliPythia8::Instance())->ReadString("411:addChannel = 1 0.00276 0 333 211");
    // D+ decays
    (AliPythia8::Instance())->ReadString("411:onMode = off");
    (AliPythia8::Instance())->ReadString("411:onIfMatch = 321 211 211");
    (AliPythia8::Instance())->ReadString("411:onIfMatch = 313 211");
    (AliPythia8::Instance())->ReadString("411:onIfMatch = 333 211");
    (AliPythia8::Instance())->ReadString("313:onMode = off");
    (AliPythia8::Instance())->ReadString("313:onIfAll = 321 211");
    // Ds decays
    (AliPythia8::Instance())->ReadString("431:onMode = off");
    (AliPythia8::Instance())->ReadString("431:onIfMatch = 333 211");
    (AliPythia8::Instance())->ReadString("431:onIfMatch = 321 313");
    (AliPythia8::Instance())->ReadString("333:onMode = off");
    (AliPythia8::Instance())->ReadString("333:onIfAll = 321 321");
    //add Xic+ decays absent in PYTHIA8 decay table
    (AliPythia8::Instance())->ReadString("4232:addChannel = 1 0.2 0 2212 -313");
    (AliPythia8::Instance())->ReadString("4232:addChannel = 1 0.2 0 2212 -321 211");
    (AliPythia8::Instance())->ReadString("4232:addChannel = 1 0.2 0 3324 211");
    (AliPythia8::Instance())->ReadString("4232:addChannel = 1 0.2 0 3312 211 211");
    // Xic+ -> pK*0
    (AliPythia8::Instance())->ReadString("4232:onIfMatch = 2212 313");
    // Xic+ -> p K- pi+
    (AliPythia8::Instance())->ReadString("4232:onIfMatch = 2212 321 211");
    // Xic+ -> Xi*0 pi+, Xi*->Xi- pi+
    (AliPythia8::Instance())->ReadString("4232:onIfMatch = 3324 211");
    // Xic+ -> Xi- pi+ pi+
    (AliPythia8::Instance())->ReadString("4232:onIfMatch = 3312 211 211");
    // Xic0 -> Xi- pi+
    (AliPythia8::Instance())->ReadString("4132:onIfMatch = 3312 211");
    //Add Omegac0 decay absent in PYTHIA8 decay table (omegac0->Xi pi)
    (AliPythia8::Instance())->ReadString("4332:addChannel = 1 0.2 0 3312 211");

    //Omegac0 decays
    (AliPythia8::Instance())->ReadString("4332:onMode = off");
    if(channelOption ==6){ // Omegac0 -> Omega- pi+
        (AliPythia8::Instance())->ReadString("4332:onIfMatch = 3334 211");
      }
    if(channelOption ==8){ // Omegac0 semileptonic decay
        (AliPythia8::Instance())->ReadString("4332:onIfMatch = 3334 -11 12");
      }
    if(channelOption ==9){ //Omegac0 -> Xi Pi
        (AliPythia8::Instance())->ReadString("4332:onIfMatch = 3312 211");
      }

    //add Lc decays absent in PYTHIA8 decay table and set BRs from PDG for others
    (AliPythia8::Instance())->ReadString("4122:oneChannel = 1 0.0196 100 2212 -313");
    (AliPythia8::Instance())->ReadString("4122:addChannel = 1 0.0108 100 2224 -321");
    (AliPythia8::Instance())->ReadString("4122:addChannel = 1 0.022 100 3124 211");
    (AliPythia8::Instance())->ReadString("4122:addChannel = 1 0.035 0 2212 -321 211");
    (AliPythia8::Instance())->ReadString("4122:addChannel = 1 0.0159 0 2212 -311");
    (AliPythia8::Instance())->ReadString("4122:addChannel = 1 0.0130 0 3122 211");
    // switch them on
    (AliPythia8::Instance())->ReadString("4122:onIfMatch = 2212 313");
    (AliPythia8::Instance())->ReadString("4122:onIfMatch = 2224 321");
    (AliPythia8::Instance())->ReadString("4122:onIfMatch = 3124 211");
    (AliPythia8::Instance())->ReadString("4122:onIfMatch = 2212 321 211");
    (AliPythia8::Instance())->ReadString("4122:onIfMatch = 3122 211");
    (AliPythia8::Instance())->ReadString("2224:onMode = off");
    (AliPythia8::Instance())->ReadString("2224:onIfAll = 2212 211");
    (AliPythia8::Instance())->ReadString("3124:onMode = off");
    (AliPythia8::Instance())->ReadString("3124:onIfAll = 2212 321");
    (AliPythia8::Instance())->ReadString("4122:onIfMatch = 2212 311");

    if(channelOption==3){
      Printf("Forcing only Lc -> pKpi channel");
      (AliPythia8::Instance())->ReadString("4122:onMode = off");
      (AliPythia8::Instance())->ReadString("4122:onIfMatch = 2212 313");
      (AliPythia8::Instance())->ReadString("4122:onIfMatch = 2224 321");
      (AliPythia8::Instance())->ReadString("4122:onIfMatch = 3124 211");
      (AliPythia8::Instance())->ReadString("4122:onIfMatch = 2212 321 211");
      (AliPythia8::Instance())->ReadString("4122:onIfMatch = 3122 211");
    }
    else if(channelOption==4){
      Printf("Lc -> pK0s channel");
      (AliPythia8::Instance())->ReadString("4122:onMode = off");
      (AliPythia8::Instance())->ReadString("4122:onIfMatch = 2212 311");
    }
    else if(channelOption==7){
      Printf("Lc -> Lpi channel");
      (AliPythia8::Instance())->ReadString("4122:onMode = off");
      (AliPythia8::Instance())->ReadString("4122:onIfMatch = 3122 211");
    }
  }
  // Set up2date lifetimes for hadrons
  // lambda_b from PDG 2019: tau0 = 1.471 ps = 441 m/c = 0.441 mm/c
  (AliPythia8::Instance())->ReadString("5122:tau0 = 4.41000e-01");



  return pyth;
}
