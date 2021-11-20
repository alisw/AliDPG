AliGenerator *GeneratorCustom(TString opt = "cc_had")
{
  
  AliGenCocktail *ctl  = (AliGenCocktail*) GeneratorCocktail("Hijing_HF");
  TString simulation = gSystem->Getenv("CONFIG_SIMULATION");
  
  if(!simulation.Contains("Embed")){
    printf("RUNNING EMBED CASE (inside if)\n"); //DEBUG
    AliGenerator *hij = GeneratorHijing();
    ctl->AddGenerator(hij, "Hijing", 1.);
  }
  
  Int_t process[2] = {kPyCharm, kPyBeauty};
  
  const Char_t *label[2] = {
    "chadr_PYTHIA", "bchadr_PYTHIA"
  };
  
  Int_t iprocess = -1;
  if(opt.Contains("cc")) {iprocess = 0; printf("-- HF process = %s -- \n",opt.Data());}
  else if(opt.Contains("bb")) {iprocess = 1; printf("-- HF process = %s -- \n",opt.Data());}
  else {
    printf("-- ERROR! no HF process set = %s -- \n",opt.Data());
    return 0x0;
  }
  
  //Removed the choice of 'ops' from optList, only case is D forced decays
  
  AliGenPythiaPlus *phf0 = (AliGenPythiaPlus*)GeneratorPythia8(kPythia8Tune_Monash2013);
  AliGenPythiaPlus *phf1 = (AliGenPythiaPlus*)GeneratorPythia8(kPythia8Tune_Monash2013);
  AliGenPythiaPlus *phf2 = (AliGenPythiaPlus*)GeneratorPythia8(kPythia8Tune_Monash2013);
  AliGenPythiaPlus *phf3 = (AliGenPythiaPlus*)GeneratorPythia8(kPythia8Tune_Monash2013);
  AliGenPythiaPlus *phf4 = (AliGenPythiaPlus*)GeneratorPythia8(kPythia8Tune_Monash2013);
  AliGenPythiaPlus *phf5 = (AliGenPythiaPlus*)GeneratorPythia8(kPythia8Tune_Monash2013);
  AliGenPythiaPlus *phf6 = (AliGenPythiaPlus*)GeneratorPythia8(kPythia8Tune_Monash2013);
  AliGenPythiaPlus *phf7 = (AliGenPythiaPlus*)GeneratorPythia8(kPythia8Tune_Monash2013);
  AliGenPythiaPlus *phf8 = (AliGenPythiaPlus*)GeneratorPythia8(kPythia8Tune_Monash2013);
  AliGenPythiaPlus *phf9 = (AliGenPythiaPlus*)GeneratorPythia8(kPythia8Tune_Monash2013);
  
  if(iprocess==0) {
    phf0->SetProcess(kPyCharm);
    phf1->SetProcess(kPyCharm);
    phf2->SetProcess(kPyCharm);
    phf3->SetProcess(kPyCharm);
    phf4->SetProcess(kPyCharm);
    phf5->SetProcess(kPyCharm);
    phf6->SetProcess(kPyCharm);
    phf7->SetProcess(kPyCharm);
    phf8->SetProcess(kPyCharm);
    phf9->SetProcess(kPyCharm);
  }
  else if(iprocess==1) {
    phf0->SetProcess(kPyBeauty);
    phf1->SetProcess(kPyBeauty);
    phf2->SetProcess(kPyBeauty);
    phf3->SetProcess(kPyBeauty);
    phf4->SetProcess(kPyBeauty);
    phf5->SetProcess(kPyBeauty);
    phf6->SetProcess(kPyBeauty);
    phf7->SetProcess(kPyBeauty);
    phf8->SetProcess(kPyBeauty);
    phf9->SetProcess(kPyBeauty);
  }
  
  phf0->SetHeavyQuarkYRange(-1.5, 1.5);
  phf0->SetMaximumLifetime(0.7);  
  phf1->SetHeavyQuarkYRange(-1.5, 1.5);
  phf1->SetMaximumLifetime(0.7);
  phf2->SetHeavyQuarkYRange(-1.5, 1.5);
  phf2->SetMaximumLifetime(0.7);
  phf3->SetHeavyQuarkYRange(-1.5, 1.5);
  phf3->SetMaximumLifetime(0.7);
  phf4->SetHeavyQuarkYRange(-1.5, 1.5);
  phf4->SetMaximumLifetime(0.7);
  phf5->SetHeavyQuarkYRange(-1.5, 1.5);
  phf5->SetMaximumLifetime(0.7);
  phf6->SetHeavyQuarkYRange(-1.5, 1.5);
  phf6->SetMaximumLifetime(0.7);
  phf7->SetHeavyQuarkYRange(-1.5, 1.5);
  phf7->SetMaximumLifetime(0.7);
  phf8->SetHeavyQuarkYRange(-1.5, 1.5);
  phf8->SetMaximumLifetime(0.7);
  phf9->SetHeavyQuarkYRange(-1.5, 1.5);
  phf9->SetMaximumLifetime(0.7);
  
  // Pt transfer of the hard scattering (5 different cases)
  // resulting pT shape tuned to be similar to pythia6 MC (Hijing_HF001.C)
  // at least one even and one odd uidConfig for pT-hard bin to have the same pT hard bins for prompt and feed-down
  Float_t pth[6] = {3.5, 10., 20., 40., 60., -1};
  Int_t ipt;
  if      (uidConfig % 100 < 74) ipt = 0; // 74% 
  else if (uidConfig % 100 < 92) ipt = 1; // 18%
  else if (uidConfig % 100 < 96) ipt = 2; // 4%
  else if (uidConfig % 100 < 98) ipt = 3; // 2%
  else                           ipt = 4; // 2%
  
  phf0->SetPtHard(pth[ipt], pth[ipt + 1]);    
  phf1->SetPtHard(pth[ipt], pth[ipt + 1]);    
  phf2->SetPtHard(pth[ipt], pth[ipt + 1]);    
  phf3->SetPtHard(pth[ipt], pth[ipt + 1]);
  phf4->SetPtHard(pth[ipt], pth[ipt + 1]);
  phf5->SetPtHard(pth[ipt], pth[ipt + 1]);
  phf6->SetPtHard(pth[ipt], pth[ipt + 1]);    
  phf7->SetPtHard(pth[ipt], pth[ipt + 1]);    
  phf8->SetPtHard(pth[ipt], pth[ipt + 1]);    
  phf9->SetPtHard(pth[ipt], pth[ipt + 1]);    

  // Configuration of pythia8 decayer
  if(AliPythiaBase::Class()->GetMethodAny("Decayer")){
    printf("Force decays using ForceHadronicD of AliDecayerPythia8\n");
    TList *argsList = AliDecayerPythia8::Class()->GetMethodAny("ForceHadronicD")->GetListOfMethodArgs();
    phf0->SetForceDecay(kHadronicDWithout4Bodies);
    phf1->SetForceDecay(kHadronicDWithout4Bodies);
    phf2->SetForceDecay(kHadronicDWithout4Bodies);
    phf3->SetForceDecay(kHadronicDWithout4Bodies);
    phf4->SetForceDecay(kHadronicDWithout4Bodies);
    phf5->SetForceDecay(kHadronicDWithout4Bodies);
    phf6->SetForceDecay(kHadronicDWithout4Bodies);
    phf7->SetForceDecay(kHadronicDWithout4Bodies);
    phf8->SetForceDecay(kHadronicDWithout4Bodies);
    phf9->SetForceDecay(kHadronicDWithout4Bodies);
  }else{
    printf("Force decays in the Config\n");
    //add D+ decays absent in PYTHIA8 decay table and set BRs from PDG for other
    (AliPythia8::Instance())->ReadString("411:oneChannel = 1 0.0752 0 -321 211 211");
    (AliPythia8::Instance())->ReadString("411:addChannel = 1 0.0104 0 -313 211");
    (AliPythia8::Instance())->ReadString("411:addChannel = 1 0.0156 0 311 211");
    (AliPythia8::Instance())->ReadString("411:addChannel = 1 0.00276 0 333 211");
    //add Lc decays absent in PYTHIA8 decay table and set BRs from PDG for other
    (AliPythia8::Instance())->ReadString("4122:oneChannel = 1 0.0196 100 2212 -313");
    (AliPythia8::Instance())->ReadString("4122:addChannel = 1 0.0108 100 2224 -321");
    (AliPythia8::Instance())->ReadString("4122:addChannel = 1 0.022 100 3124 211");
    (AliPythia8::Instance())->ReadString("4122:addChannel = 1 0.035 0 2212 -321 211");
    (AliPythia8::Instance())->ReadString("4122:addChannel = 1 0.0159 0 2212 311");
    (AliPythia8::Instance())->ReadString("4122:addChannel = 1 0.0130 0 3122 211");
    //add Xic+ decays absent in PYTHIA8 decay table
    (AliPythia8::Instance())->ReadString("4232:addChannel = 1 0.2 0 2212 313");
    (AliPythia8::Instance())->ReadString("4232:addChannel = 1 0.2 0 2212 321 211");
    (AliPythia8::Instance())->ReadString("4232:addChannel = 1 0.2 0 3324 211");
    (AliPythia8::Instance())->ReadString("4232:addChannel = 1 0.2 0 3312 211 211");
    //add Xic0 decays absent in PYTHIA8 decay table
    (AliPythia8::Instance())->ReadString("4132:addChannel = 1 0.2 0 3312 211");
    // D+ decays
    (AliPythia8::Instance())->ReadString("411:onMode = off");
    (AliPythia8::Instance())->ReadString("411:onIfMatch = 321 211 211");
    (AliPythia8::Instance())->ReadString("411:onIfMatch = 313 211");
    (AliPythia8::Instance())->ReadString("411:onIfMatch = 333 211");
    (AliPythia8::Instance())->ReadString("313:onMode = off");
    (AliPythia8::Instance())->ReadString("313:onIfAll = 321 211");
    
    // D0 decays
    (AliPythia8::Instance())->ReadString("421:onMode = off");
    (AliPythia8::Instance())->ReadString("421:onIfMatch = 321 211");
    
    // Ds decays
    (AliPythia8::Instance())->ReadString("431:onMode = off");
    (AliPythia8::Instance())->ReadString("431:onIfMatch = 333 211");
    (AliPythia8::Instance())->ReadString("431:onIfMatch = 321 313");
    (AliPythia8::Instance())->ReadString("333:onMode = off");
    (AliPythia8::Instance())->ReadString("333:onIfAll = 321 321");

    // Lc decays
    Int_t channelOption = 0;
    (AliPythia8::Instance())->ReadString("4122:onMode = off");
    if(channelOption == 0) { //general purpose or pKpi triggered
      (AliPythia8::Instance())->ReadString("4122:onIfMatch = 2212 313");
      (AliPythia8::Instance())->ReadString("4122:onIfMatch = 2224 321");
      (AliPythia8::Instance())->ReadString("4122:onIfMatch = 3124 211");
      (AliPythia8::Instance())->ReadString("4122:onIfMatch = 2212 321 211");
      (AliPythia8::Instance())->ReadString("2224:onMode = off");
      (AliPythia8::Instance())->ReadString("2224:onIfAll = 2212 211");
      (AliPythia8::Instance())->ReadString("3124:onMode = off");
      (AliPythia8::Instance())->ReadString("3124:onIfAll = 2212 321");
      (AliPythia8::Instance())->ReadString("4122:onIfMatch = 2212 321 211");
    }
    else if(channelOption == 1) { //general purpose or pK0s triggered
      (AliPythia8::Instance())->ReadString("4122:onIfMatch = 2212 311");
    }
    else if(channelOption == 2) { //general purpose or pKpi triggered
      (AliPythia8::Instance())->ReadString("4122:onIfMatch = 2212 321 211");
    }

    // Xic+ decays
    (AliPythia8::Instance())->ReadString("4232:onMode = off");
    (AliPythia8::Instance())->ReadString("4232:onIfMatch = 2212 313");
    (AliPythia8::Instance())->ReadString("4232:onIfMatch = 2212 321 211");
    (AliPythia8::Instance())->ReadString("4232:onIfMatch = 3324 211");
    (AliPythia8::Instance())->ReadString("4232:onIfMatch = 3312 211 211");
    
    // Xic0 decays
    (AliPythia8::Instance())->ReadString("4132:onMode = off");
    (AliPythia8::Instance())->ReadString("4132:onIfMatch = 3312 211");
  }
  // Set up2date lifetimes for hadrons
  // lambda_b from PDG 2019: tau0 = 1.471 ps = 441 m/c = 0.441 mm/c
  (AliPythia8::Instance())->ReadString("5122:tau0 = 4.41000e-01");

  ctl->AddGenerator(phf0, Form("%s_0",label[iprocess]), 1.);
  ctl->AddGenerator(phf1, Form("%s_1",label[iprocess]), 1.);
  ctl->AddGenerator(phf2, Form("%s_2",label[iprocess]), 1.);
  ctl->AddGenerator(phf3, Form("%s_3",label[iprocess]), 1.);      
  ctl->AddGenerator(phf4, Form("%s_4",label[iprocess]), 1.);      
  ctl->AddGenerator(phf5, Form("%s_5",label[iprocess]), 1.);
  ctl->AddGenerator(phf6, Form("%s_6",label[iprocess]), 1.);
  ctl->AddGenerator(phf7, Form("%s_7",label[iprocess]), 1.);
  ctl->AddGenerator(phf8, Form("%s_8",label[iprocess]), 1.);
  ctl->AddGenerator(phf9, Form("%s_9",label[iprocess]), 1.);

  printf(">>>>> added generator for HF production:  %s, num %d \n", label[iprocess],0);
  printf(">>>>> added generator for HF production:  %s, num %d \n", label[iprocess],1);
  
  return ctl;
  
}

