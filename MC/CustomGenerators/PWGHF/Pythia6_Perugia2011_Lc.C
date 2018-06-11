AliGenerator *GeneratorCustom(TString opt = "")

{

  Decay_t decay[2] = {AliDecayer::kLcpKpi, AliDecayer::kLcpK0S};
  TString optList[2] = {"LcpKpi","LcpK0S"};
  TString label[2] = {"LcpKpi PYTHIA","LcpK0S PYTHIA"};

  Int_t idecay = 0;  
  for (Int_t iopt = 0; iopt < 2; iopt++ ) {
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


