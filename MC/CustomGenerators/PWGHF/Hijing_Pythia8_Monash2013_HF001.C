AliGenerator *GeneratorCustom(TString opt = "")
{

  TString optList[4] = {"had", "LcK0Sp", "LcpKpi", "DsKKpi"};
  Int_t channelOption = 0;
  for (Int_t iopt = 0; iopt < 4; iopt++)
    if (opt.EqualTo(optList[iopt]))
      channelOption = iopt;

  AliGenCocktail *ctl  = (AliGenCocktail*) GeneratorCocktail("Hijing_Pythia8_HF");

  TString simulation = gSystem->Getenv("CONFIG_SIMULATION");
  Int_t ntimes=1;

  Double_t bEvalFormula=-1;
  Bool_t isEmbedding=kFALSE;
  if(!simulation.Contains("Embed")){
    AliGenerator *hij = GeneratorHijing();
    ctl->AddGenerator(hij, "Hijing", 1.);
  }
  else{
    isEmbedding=kTRUE;

    if(bminConfig>bmaxConfig)Fatal("Hijing_Pythia8_HF001_MCtoMCembedding","max impact parameter smaller than min impact parameter");
    if(bminConfig<0.)Fatal("Hijing_Pythia8_HF001_MCtoMCembedding","min impact parameter <0");
    if(bmaxConfig>20)Fatal("Hijing_Pythia8_HF001_MCtoMCembedding","unphysical impact parameter");
    bEvalFormula=(bmaxConfig+bminConfig)/2.;
  }

  const Char_t *label[2][4] = {
    {"chadr PYTHIA8", "chadr PYTHIA8 LcK0SpDedicated", "chadr PYTHIA8 LcpKpiDedicated", "chadr PYTHIA8 DsDedicated"},
    {"bchadr PYTHIA8", "bchadr PYTHIA8 LcK0SpDedicated", "bchadr PYTHIA8 LcpKpiDedicated", "bchadr PYTHIA8 DsDedicated"}
  };

  //Switches for prompt/nonprompt
  Process_t process[2] = {kPyCharm, kPyBeauty}; //charm or beauty with HardQCD for pT hard bins
  Int_t iprocess = uidConfig % 2;

  //switch sign of trigger particle
  Int_t sign = (uidConfig%4 <= 1) ? 1 : -1;

  //switch trigger particle (if enabled)
  Int_t triggerPart = -1;
  if (channelOption > 0 && channelOption < 3)
    triggerPart = 4122; // Lc
  else if (channelOption == 3)
    triggerPart = 431; // Ds 

  AliGenPythiaPlus* pyth = (AliGenPythiaPlus*)GeneratorPythia8(kPythia8Tune_Monash2013);
  pyth->SetProcess(process[iprocess]);
  pyth->SetHeavyQuarkYRange(-1.5, 1.5);
  pyth->SetMaximumLifetime(0.7);
  if(channelOption >= 1) {
    pyth->SetTriggerParticle(sign * triggerPart, 999);
    if(AliGenPythiaPlus::Class()->GetMethodAny("SetTriggerY"))
      pyth->SetTriggerY(1.0);
  }

  // Pt transfer of the hard scattering (5 different cases)
  // resulting pT shape tuned to be similar to pythia6 MC (Hijing_HF001.C)
  // at least one even and one odd uidConfig for pT-hard bin to have the same pT hard bins for prompt and feed-down
  Float_t pth[6] = {0., 3.5, 10., 20., 40., 60.};
  Int_t ipt;
  if      (uidConfig % 100 < 74) ipt = 0; // 74% 
  else if (uidConfig % 100 < 92) ipt = 1; // 18%
  else if (uidConfig % 100 < 96) ipt = 2; // 4%
  else if (uidConfig % 100 < 98) ipt = 3; // 2%
  else                           ipt = 4; // 2%
  pyth->SetPtHard(pth[ipt], pth[ipt + 1]);

  // Configuration of pythia8 decayer
  if(AliPythiaBase::Class()->GetMethodAny("Decayer")){
    printf("Force decays using ForceHadronicD of AliDecayerPythia8\n");
    pyth->SetForceDecay(kHadronicDWithout4Bodies);
    if(channelOption == 1)
      pyth->SetForceDecay(kLcpK0S);
    else if(channelOption == 2)
      pyth->SetForceDecay(kLcpKpi);
    else if(channelOption == 3 && AliDecayerPythia8::Class()->GetMethod("ForceHadronicD", "0,0,0,0,1"))
      pyth->SetForceDecay(kHadronicDWithout4BodiesDsPhiPi);
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

  TFormula *formula = new TFormula("Signals","max(1.,120.*(x<5.)+80.*(1.-x/20.)*(x>5.)*(x<11.)+240.*(1.-x/13.)*(x>11.))");
  if(isEmbedding){
    ntimes=formula->Eval(bEvalFormula);
    delete formula; formula=0x0;
  }

  ctl->AddGenerator(pyth, label[iprocess][channelOption], 1., formula, ntimes);
  printf(">>>>> added HF generator %s \n", label[iprocess][channelOption]);

  return ctl;
}
