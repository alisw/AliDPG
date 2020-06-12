AliGenerator *
GeneratorCustom(TString opt = "")
{
	
  AliGenCocktail *ctl  = (AliGenCocktail*) GeneratorCocktail("Hijing_HF");
  TString simulation = gSystem->Getenv("CONFIG_SIMULATION");

  if(!simulation.Contains("Embed")){
    AliGenerator *hij = GeneratorHijing();
    ctl->AddGenerator(hij, "Hijing", 1.);
  }

  Int_t process[2] = {kPythia6HeavyProcess_Charm, kPythia6HeavyProcess_Beauty};
  Int_t decay[4]   = {kPythia6HeavyDecay_Hadrons, kPythia6HeavyDecay_HadronsWithV0, kPythia6HeavyDecay_Electron, kPythia6HeavyDecay_All};
  
  const Char_t *label[2][4] = {
    "chadr_PYTHIA", "chadr_PYTHIA", "cele_PYTHIA", "ccbar_PYTHIA", 
    "bchadr_PYTHIA", "bchadr_PYTHIA", "bele_PYTHIA", "bbbar_PYTHIA"
  };
  
  Int_t iprocess = -1;
  if(opt.Contains("cc")) {iprocess = 0; printf("-- HF process = %s -- \n",opt.Data());}
  else if(opt.Contains("bb")) {iprocess = 1; printf("-- HF process = %s -- \n",opt.Data());}
  else if(opt.Contains("jet")) {iprocess = 2; printf("-- HF process = %s -- \n",opt.Data());}
  else {printf("-- no HF process set = %s -- \n",opt.Data());}
  
  TString optList[4] = {"had", "hv0", "ele", "all"};
  Int_t idecay = 0;
  for (Int_t iopt = 0; iopt < 4; iopt++){
    if (opt.Contains(optList[iopt]))
      idecay = iopt;
  }
  
  AliGenerator *phf;
  if(iprocess < 2) {
    for(int i=0; i<10;i++){
		phf  = GeneratorPythia6Heavy(process[iprocess], decay[idecay], kPythia6Tune_Perugia2011, kFALSE);
		ctl->AddGenerator(phf, label[iprocess][idecay], 1.);
		printf(">>>>> added generator for HF production:  %s \n", label[iprocess][idecay]);
	}
  }
  else if(iprocess == 2) {
    phf  = GeneratorPythia6Jets(kPythia6Tune_Perugia2011);
    ctl->AddGenerator(phf, "jetjet", 1.);
    printf(">>>>> added generator for HF production:  %s \n", "jetjet");
  }
     
  return ctl;
  
}

