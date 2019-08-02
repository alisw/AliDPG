AliGenerator *GeneratorCustom(TString opt = "")

{

  Decay_t decay[4] = {AliDecayer::kLcpKpi, AliDecayer::kLcpK0S, AliDecayer::kLcpK0SBDTsig, AliDecayer::kHadronicDWithV0};
  Int_t   triggerParticle[4] = {4122,  4122, 4122, 431};
  TString optList[3][4] = {{"LcpKpi","LcpK0S", "LcpK0SBDTsig","DsDedicated"},
			   {"LcpKpiHMSPD","LcpK0SHMSPD","LcpK0SBDTsigHMSPD","DsDedicatedHMSPD"},
			   {"LcpKpiV0HM", "LcpK0SV0HM" ,"LcpK0SBDTsigV0HM" ,"DsDedicatedV0HM"}};
		

  TString label[4] = {"LcpKpi PYTHIA","LcpK0S PYTHIA", "LcpK0S PYTHIA (forced K decay)", "DsDedicated PYTHIA"};

    
  //Switches for prompt/nonprompt, sign of trigger particle
  Process_t iprocess = 0;	//charm or beauty
  Int_t sign = 0; //Sign of trigger particle

  if (uidConfig%2 == 0)  iprocess = kPyCharmppMNRwmi; else iprocess = kPyBeautyppMNRwmi;
  if (uidConfig%4 <= 1)  sign = 1; 		      else sign = -1; //switch sign of trigger particle
  for (Int_t imult = 0; imult < 3; imult++) {
     for (Int_t iopt = 0; iopt < 4; iopt++ ) {
       if (opt.EqualTo(optList[imult][iopt])) { multOption = imult;  channelOption = iopt;}
     }
  }
//  AliGenCocktail *ctl = GeneratorCocktail("Perugia2011_HF");
  AliGenPythia* pyth = GeneratorPythia6(kPythia6Tune_Perugia2011);
  pyth->SetProcess(iprocess);
  pyth->SetTriggerParticle(sign * triggerParticle[channelOption],999,999,-1,1000); //Lc or Ds, etamin, etamax, ptmin, ptmax
  switch (imult) {  //multiplicity triggers
    case 0: break; //INT7
    case 1: pyth->SetTriggerChargedMultiplicity(40,2.0); break; //HMSPD
    case 2: pyth->SetTriggerMultiplicityEtaRange(40,2.8,5.1);   //V0A
            pyth->SetTriggerMultiplicityEtaRange(40,-3.7,-1.7); //V0C
            break;
  }
  pyth->SetTriggerY(1.0);
  pyth->SetHeavyQuarkYRange(-1.5,1.5);
  pyth->SetForceDecay(decay[channelOption]);
  pyth->SetPtHard(pthardminConfig,pthardmaxConfig);
  return pyth;

}


