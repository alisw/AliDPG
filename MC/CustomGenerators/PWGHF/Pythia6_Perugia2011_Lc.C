AliGenerator *GeneratorCustom(TString opt = "")

{

  Decay_t decay[3] = {AliDecayer::kLcpKpi, AliDecayer::kLcpK0S, AliDecayer::kLcpK0SBDTsig};
  TString optList[3] = {"LcpKpi","LcpK0S", "LcpK0SBDTsig"};
  TString label[3] = {"LcpKpi PYTHIA","LcpK0S PYTHIA", "LcpK0S PYTHIA (forced K decay)"};

  Int_t idecay = 0;  
  for (Int_t iopt = 0; iopt < 3; iopt++ ) {
      if (opt.EqualTo(optList[iopt])) idecay = iopt;
  }

//  AliGenCocktail *ctl = GeneratorCocktail("Perugia2011_HF");
  AliGenPythia* pyth = GeneratorPythia6(kPythia6Tune_Perugia2011);
  pyth->SetProcess(kPyCharmppMNRwmi);
  pyth->SetTriggerParticle(4122,999,999,-1,1000); //Lc, etamin, etamax, ptmin, ptmax
  pyth->SetTriggerY(1.0);
  pyth->SetHeavyQuarkYRange(-1.5,1.5);
  pyth->SetForceDecay(decay[idecay]);
  pyth->SetPtHard(pthardminConfig,pthardmaxConfig);
  return pyth;

}


