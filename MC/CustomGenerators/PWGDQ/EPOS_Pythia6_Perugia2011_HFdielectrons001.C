AliGenerator *EPOS_MbPythiaTunePerugia2011bcforcele(Int_t flag);
Float_t energy;

AliGenerator *
GeneratorCustom()
{

  Int_t flag = (Int_t)gRandom->Uniform(0,100);
  energy = energyConfig;
  return EPOS_MbPythiaTunePerugia2011bcforcele(flag);
}

AliGenerator* EPOS_MbPythiaTunePerugia2011bcforcele(Int_t flag)
{


  // load libraries to use Evtgen
  gSystem->Load("libPhotos");
  gSystem->Load("libEvtGen");
  gSystem->Load("libEvtGenExternal");
  gSystem->Load("libTEvtGen");  

  AliGenCocktail *gener = new AliGenCocktail();
  gener->UsePerEventRates();


  // add EPOS generator for p-Pb
  AliGenerator *epos = GeneratorEPOSLHC();
  gener->AddGenerator(epos, "EPOS-LHC", 1.);


  comment = comment.Append(" pp: HF->ee in |y|<1.2");

  ////////////////////////////////////////////
  //    Pythia cc->ee
  ////////////////////////////////////////////
  AliGenPythia* pythiaCC = new AliGenPythia(-1);
  pythiaCC->SetTitle("PYTHIA-HF-Cdiele");
  pythiaCC->SetMomentumRange(0, 999999.);
  pythiaCC->SetThetaRange(0., 180.);
  pythiaCC->SetPtRange(0,1000.);
  pythiaCC->SetProcess(kPyCharmppMNRwmi);
  pythiaCC->SetEnergyCMS(energy);
  pythiaCC->SetForceDecay(kSemiElectronic);
  //    Tune                                                                
  //    320     Perugia 0 
  //    350     Perugia 2011
  pythiaCC->SetTune(350);
  pythiaCC->UseNewMultipleInteractionsScenario();
  //
  //    decays 
  pythiaCC->SetCutOnChild(2);
  pythiaCC->SetPdgCodeParticleforAcceptanceCut(11);
  pythiaCC->SetChildYRange(-1.2,1.2);
  pythiaCC->SetChildPtRange(0,10000.);
  pythiaCC->SetStackFillOpt(AliGenPythia::kHeavyFlavor);

  ////////////////////////////////////////////
  //    Pythia bb->ee
  ////////////////////////////////////////////
  AliGenPythia* pythiaBB = new AliGenPythia(-1);
  pythiaBB->SetTitle("PYTHIA-HF-Bdiele");
  pythiaBB->SetMomentumRange(0, 999999.);
  pythiaBB->SetThetaRange(0., 180.);
  pythiaBB->SetPtRange(0,1000.);
  pythiaBB->SetProcess(kPyBeautyppMNRwmi);
  pythiaBB->SetEnergyCMS(energy);
  pythiaBB->SetForceDecay(kSemiElectronic);
  //    Tune                                                                
  //    320     Perugia 0 
  //    350     Perugia 2011
  pythiaBB->SetTune(350);
  pythiaBB->UseNewMultipleInteractionsScenario();
  //
  //    decays 
  pythiaBB->SetCutOnChild(2);
  pythiaBB->SetPdgCodeParticleforAcceptanceCut(11);
  pythiaBB->SetChildYRange(-1.2,1.2);
  pythiaBB->SetChildPtRange(0,10000.);
  pythiaBB->SetStackFillOpt(AliGenPythia::kHeavyFlavor);


  ////////////////////////////////////////////
  //    Pythia b,c->e
  ////////////////////////////////////////////
  AliGenPythia* pythiaB= new AliGenPythia(-1);
  pythiaB->SetTitle("PYTHIA-HF-Bele");
  pythiaB->SetMomentumRange(0, 999999.);
  pythiaB->SetThetaRange(0., 180.);
  pythiaB->SetPtRange(0,1000.);
  pythiaB->SetProcess(kPyBeautyppMNRwmi);
  pythiaB->SetEnergyCMS(energy);
  //    Tune                                                                
  //    320     Perugia 0 
  //    350     Perugia 2011
  pythiaB->SetTune(350);
  pythiaB->UseNewMultipleInteractionsScenario();
  //
  //    decays 
  pythiaB->SetCutOnChild(1);
  pythiaB->SetPdgCodeParticleforAcceptanceCut(11);
  pythiaB->SetChildYRange(-1.2,1.2);
  pythiaB->SetChildPtRange(0,10000.);
  pythiaB->SetStackFillOpt(AliGenPythia::kHeavyFlavor);


  ////////////////////////////////////////////
  //  Create cocktail
  ////////////////////////////////////////////
  
  if(flag>=0 && flag<10){ //  0.1 x 30 M = 3 M
    gener->AddGenerator(pythiaCC,"Pythia CC",1.);
  }else if(flag>=10 && flag<20){ //  0.1 x 30 M = 3 M
    gener->AddGenerator(pythiaBB,"Pythia BB",1.);
  }else if(flag>=20 && flag<100){ //  0.8 x 30 M = 24 M
    gener->AddGenerator(pythiaB,"Pythia B",1.);
  }

  return gener;
}
