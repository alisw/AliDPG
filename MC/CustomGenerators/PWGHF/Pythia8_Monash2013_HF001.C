AliGenerator *GeneratorCustom(TString opt = "")
{

  //not necessary for the moment, but helpful when more options will be added 
  const Char_t *label[2][1] = {
    {"chadr PYTHIA8"},
    {"bhadr PYTHIA8"},
  };

  TString optList[1] = {"had"};
  Int_t channelOption = 0;
  for (Int_t iopt = 0; iopt < 1; iopt++)
    if (opt.EqualTo(optList[iopt]))
      channelOption = iopt;

  //Switches for prompt/nonprompt
  Process_t process[2] = {kPyCharmppMNRwmi, kPyBeautyppMNRwmi}; //charm or beauty
  Int_t iprocess = uidConfig % 2;

  AliGenPythiaPlus* pyth = (AliGenPythiaPlus*)GeneratorPythia8(kPythia8Tune_Monash2013);
  pyth->SetProcess(process[iprocess]);
  pyth->SetHeavyQuarkYRange(-1.5, 1.5);
  // Pt transfer of the hard scattering (set for all cases)
  pyth->SetPtHard(pthardminConfig, pthardmaxConfig);

  // Configuration of decayer
  if(AliPythiaBase::Class()->GetMethodAny("Decayer")){
    printf("Force decays using ForceHadronicD of AliDecayerPythia8\n");
    pyth->SetForceDecay(kHadronicDWithout4Bodies);
  }else{
    printf("Force decays in the Config\n");
    //add D+ decays absent in PYTHIA8 decay table and set BRs from PDG for other
    (AliPythia8::Instance())->ReadString("411:oneChannel = 1 0.0752 0 -321 211 211");
    (AliPythia8::Instance())->ReadString("411:addChannel = 1 0.0104 0 -313 211");
    (AliPythia8::Instance())->ReadString("411:addChannel = 1 0.0156 0 311 211");
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
    (AliPythia8::Instance())->ReadString("4122:onIfMatch = 2212 313");
    (AliPythia8::Instance())->ReadString("4122:onIfMatch = 2224 321");
    (AliPythia8::Instance())->ReadString("4122:onIfMatch = 3124 211");
    (AliPythia8::Instance())->ReadString("4122:onIfMatch = 2212 321 211");
    (AliPythia8::Instance())->ReadString("4122:onIfMatch = 3122 211");
    (AliPythia8::Instance())->ReadString("4122:onIfMatch = 2212 311");
    (AliPythia8::Instance())->ReadString("2224:onMode = off");
    (AliPythia8::Instance())->ReadString("2224:onIfAll = 2212 211");
    (AliPythia8::Instance())->ReadString("3124:onMode = off");
    (AliPythia8::Instance())->ReadString("3124:onIfAll = 2212 321");

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
  
  //not necessary for the moment, but helpful when more options will be added 
  //AliGenCocktail *ctl  = static_cast<AliGenCocktail *>(GeneratorCocktail("Monash2013_HF"));
  //ctl->AddGenerator(pyth, label[iprocess][channelOption], 1.);

  return pyth;
}
