AliGenerator *GeneratorCustom(TString opt = "")
{
  const Char_t *label[3] = {    "cele PYTHIA", "bele PYTHIA"  };
  AliGenCocktail *ctl  = static_cast<AliGenCocktail *>(GeneratorCocktail("Monash2013HFe"));

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

  ctl->AddGenerator(pyth, label[iprocess], 1.);

  AliGenPHOSlib *plib = new AliGenPHOSlib();
  Double_t ptMaxInjected = 50.;
  if(ptmaxConfig != -1){
    ptMaxInjected = ptmaxConfig;
  }

  AliGenParam *pi0 = new AliGenParam(2, plib, AliGenPHOSlib::kPi0Flat);
  pi0->SetPhiRange(0., 360.) ;
  pi0->SetYRange(-1.2, 1.2) ;
  pi0->SetPtRange(0., ptMaxInjected);
  pi0->SetKeepIfOneChildSelected(kTRUE);
  pi0->SetCutOnChild(1);
  pi0->SetPdgCodeParticleforAcceptanceCut(11);
  pi0->SetChildYRange(-1.2, 1.2);
  pi0->SetChildPtRange(pttrigminConfig, 10000.);
  ctl->AddGenerator(pi0,  "pi0", 1.);

  AliGenParam *eta = new AliGenParam(1, plib, AliGenPHOSlib::kEtaFlat);
  eta->SetPhiRange(0., 360.) ;
  eta->SetYRange(-1.2, 1.2) ;
  eta->SetPtRange(0., ptMaxInjected);
  eta->SetKeepIfOneChildSelected(kTRUE);
  eta->SetCutOnChild(1);
  eta->SetPdgCodeParticleforAcceptanceCut(11);
  eta->SetChildYRange(-1.2, 1.2);
  eta->SetChildPtRange(pttrigminConfig, 10000.);
  ctl->AddGenerator(eta,  "eta", 1.);
  
  return ctl;
}
