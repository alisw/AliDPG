AliGenerator * GeneratorCustom(TString opt = "")
{
  Decay_t decay[3] = {AliDecayer::kLcpKpi,AliDecayer::kLcpK0S,AliDecayer::kLcpK0SBDTsig};
  Int_t idecay=0;
  const Char_t *label[3] = { "LcpKpi PYTHIA", "LcpK0S PYTHIA", "LcpK0S PYTHIA (forced kaon decay)" } ;
  
  TString optList[3] = {"LcpKpi","LcpK0S","LcpK0SBDTsig"};
  for (Int_t iopt = 0; iopt < 3; iopt++){
    if (opt.EqualTo(optList[iopt])) idecay = iopt;
  }

  AliGenCocktail *ctl = (AliGenCocktail*) GeneratorCocktail("Hijing_Lc");

  Float_t randHF = gRandom->Rndm();
  if (randHF>0.176797){ // add Hijing for p-Pb

  AliGenHijing *hij = GeneratorHijing();
 
  hij->SetSpectators(0);
  
  hij->SetDecaysOff(0);
  hij->SetSelectAll(1);

 ctl->AddGenerator(hij, "Hijing",1.);
}

  AliGenPythia* pyth = GeneratorPythia6(kPythia6Tune_Perugia2011);
  pyth->SetProcess(kPyCharmppMNRwmi);
  pyth->SetTriggerParticle(4122,999,999,-1,1000); //Lc, etamin, etamax, ptmin, ptmax
  pyth->SetTriggerY(1.0);
  pyth->SetHeavyQuarkYRange(-1.5,1.5);
  pyth->SetForceDecay(decay[idecay]);  //Force Lc decay mode in PYTHIA
  pyth->SetPtHard(pthardminConfig,pthardmaxConfig);
  ctl->AddGenerator(pyth, "Lc PYTHIA", 1.);  
  return ctl;
}

