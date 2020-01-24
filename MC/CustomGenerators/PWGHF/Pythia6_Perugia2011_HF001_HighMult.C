AliGenerator *
GeneratorCustom(TString opt = "")
{
  const Char_t *label[2][3] = {
    "chadr PYTHIA", "chadr PYTHIA", "cele PYTHIA",
    "bchadr PYTHIA", "bchadr PYTHIA", "bele PYTHIA"
  };
  //
  Int_t process[2] = {kPythia6HeavyProcess_Charm, kPythia6HeavyProcess_Beauty};
  Int_t iprocess = uidConfig % 2;
  //
  Int_t decay[3]   = {kPythia6HeavyDecay_Hadrons, kPythia6HeavyDecay_HadronsWithV0, kPythia6HeavyDecay_Electron};
  TString optList[3] = {"had", "hv0", "ele"};
  Int_t idecay = 0;
  for (Int_t iopt = 0; iopt < 3; iopt++)
    if (opt.Contains(optList[iopt]))
      idecay = iopt;
  //
  AliGenCocktail *ctl  = GeneratorCocktail("Perugia2011_HF");
  //
  AliGenerator *phf  = GeneratorPythia6Heavy(process[iprocess], decay[idecay], kPythia6Tune_Perugia2011, kFALSE);
  
  //setting multiplicity threshold
  TString multthr = opt;
  Int_t stop = opt.First('_');
  if(stop==-1) {
  	printf("Warning! Multiplicity threshold has to be set via the macro parameter!\n");
	return 0x0;
  }
  multthr.Remove(0,stop+1);
  ((AliGenPythia*)phf)->SetTriggerChargedMultiplicity(multthr.Atoi(), 1.2);
  
  ctl->AddGenerator(phf, label[iprocess][idecay], 1.);
  printf(">>>>> added HF generator %s with mult threshold %d\n", label[iprocess][idecay],multthr.Atoi());
  // add pi0 and eta enhancement
  if (decay[idecay] == kPythia6HeavyDecay_Electron) {
    AliGenPHOSlib *plib = new AliGenPHOSlib();
    AliGenParam *pi0 = new AliGenParam(2, plib, AliGenPHOSlib::kPi0Flat);
    pi0->SetPhiRange(0., 360.) ;
    pi0->SetYRange(-1.2, 1.2) ;
    pi0->SetPtRange(0., 50.) ;
    ctl->AddGenerator(pi0,  "pi0", 1.);
    AliGenParam *eta = new AliGenParam(1, plib, AliGenPHOSlib::kEtaFlat);
    eta->SetPhiRange(0., 360.) ;
    eta->SetYRange(-1.2, 1.2) ;
    eta->SetPtRange(0., 50.) ;
    ctl->AddGenerator(eta,  "eta", 1.);
  }
  //
  return ctl;
}
