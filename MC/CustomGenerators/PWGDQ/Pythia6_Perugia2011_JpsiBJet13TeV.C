AliGenerator *
GeneratorCustom()
{
  // load libraries to use Evtgen
  gSystem->Load("libPhotos");
  //gSystem->Load("libEvtGenBase");
  //gSystem->Load("libEvtGenModels");
  gSystem->Load("libEvtGen");
  gSystem->Load("libEvtGenExternal");
  gSystem->Load("libTEvtGen");  
  //
  // set external decayer
  TVirtualMCDecayer* decayer = new AliDecayerPythia();
  decayer->SetForceDecay(kAll);
  decayer->Init();
  gMC->SetExternalDecayer(decayer);

  //Generating a cocktail
  AliGenCocktail *gener = new AliGenCocktail();
  gener->UsePerEventRates();

  AliGenPythia *pythiaHighPt = new AliGenPythia(-1);
  pythiaHighPt->SetMomentumRange(0, 999999.);
  pythiaHighPt->SetThetaRange(0., 180.);
  pythiaHighPt->SetYRange(-2., 2.);
  pythiaHighPt->SetPtRange(0, 1000.);
  pythiaHighPt->SetProcess(kPyBeautyppMNRwmi);
  pythiaHighPt->SetEnergyCMS(energyConfig);
  pythiaHighPt->SetTune(kPythia6Tune_Perugia2011);
  pythiaHighPt->UseNewMultipleInteractionsScenario();
  // Cuts on child - J/psi
  pythiaHighPt->SetCutOnChild(1);
  pythiaHighPt->SetPdgCodeParticleforAcceptanceCut(443);
  pythiaHighPt->SetChildYRange(-1, 1);
  //pythiaHighPt->SetChildPtRange(9, 50.); // high pT J/psi would be enhanced with the pT hard bins?
  pythiaHighPt->SetForceDecay(kBJpsiUndecayed);
  //pythiaHighPt->SetStackFillOpt(AliGenPythia::kHeavyFlavor);// this commenting should keep the full pyhia event?

  pythiaHighPt->SetPtHard(pthardminConfig, pthardmaxConfig);
  
  // not sure if this is needed, if not also remove from cocktail 
  AliGenEvtGen *gene = new AliGenEvtGen();
  gene->SetForceDecay(kBJpsiDiElectron);
  gene->SetParticleSwitchedOff(AliGenEvtGen::kHFPart);

  // cocktail
  gener->AddGenerator(pythiaHighPt, "jpsiBdecayPythiaHighPt", 1.);
  gener->AddGenerator(gene, "EvtGen", 1.);

  return gener;
}