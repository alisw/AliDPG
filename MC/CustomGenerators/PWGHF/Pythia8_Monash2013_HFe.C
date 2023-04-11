AliGenerator *GeneratorCustom(TString opt = "")
{

  //Switches for prompt/nonprompt
  Process_t process[2] = {kPyCharmppMNRwmi, kPyBeautyppMNRwmi}; //charm or beauty
  Int_t iprocess = uidConfig % 2;

  AliGenPythiaPlus* pyth = (AliGenPythiaPlus*)GeneratorPythia8(kPythia8Tune_Monash2013);
  pyth->SetProcess(process[iprocess]);
  
  TVirtualMCDecayer* decayer = new AliDecayerPythia();
  decayer->SetForceDecay(kAll);
  decayer->Init();
  gMC->SetExternalDecayer(decayer);

  // Pt transfer of the hard scattering (set for all cases)
  //pyth->SetPtHard(pthardminConfig, pthardmaxConfig);

  // From overview
  pyth->SetCutOnChild(1);
  pyth->SetPdgCodeParticleforAcceptanceCut(11);
  pyth->SetChildYRange(-1.2, 1.2);
  pyth->SetChildPtRange(0, 10000.);
   
  // lambda_b from PDG 2019: tau0 = 1.471 ps = 441 m/c = 0.441 mm/c
  (AliPythia8::Instance())->ReadString("5122:tau0 = 4.41000e-01");
  
  return pyth;
}
