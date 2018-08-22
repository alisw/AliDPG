AliGenerator *
GeneratorCustom(TString opt = "")
{
  const Char_t *label[2][4] = {
    "chadr PYTHIA", "chadr PYTHIA", "cele PYTHIA", "cele PYTHIA eleTrig",
    "bchadr PYTHIA", "bchadr PYTHIA", "bele PYTHIA", "bele PYTHIA eleTrig"
  };
  //
  Int_t process[2] = {kPythia6HeavyProcess_Charm, kPythia6HeavyProcess_Beauty};
  Int_t iprocess = uidConfig % 2;
  //
  Int_t decay[4]   = {kPythia6HeavyDecay_Hadrons, kPythia6HeavyDecay_HadronsWithV0, kPythia6HeavyDecay_Electron, kPythia6HeavyDecay_Electron};
  TString optList[4] = {"had", "hv0", "ele","eleTrig"};
  Int_t idecay = 0;
  for (Int_t iopt = 0; iopt < 4; iopt++)
    if (opt.EqualTo(optList[iopt]))
      idecay = iopt;
  //
  AliGenCocktail *ctl  = GeneratorCocktail("Perugia2011_HF");
  //
  AliGenerator *phf  = GeneratorPythia6Heavy(process[iprocess], decay[idecay], kPythia6Tune_Perugia2011, kFALSE);
  ctl->AddGenerator(phf, label[iprocess][idecay], 1.);

  if(idecay == 3){ //apply cut on pte (steered by pttrigminConfig)
    ((AliGenPythia *)phf)->SetChildPtRange(pttrigminConfig, 10000.);
    printf(">>>>> added HF generator %s with pT cut = %.2f \n", label[iprocess][idecay], pttrigminConfig);
  }
  else{
    printf(">>>>> added HF generator %s \n", label[iprocess][idecay]);
  }
  
  // add pi0 and eta enhancement
  if (decay[idecay] == kPythia6HeavyDecay_Electron) {
    AliGenPHOSlib *plib = new AliGenPHOSlib();

    AliGenParam *pi0 = new AliGenParam(2, plib, AliGenPHOSlib::kPi0Flat);
    pi0->SetPhiRange(0., 360.) ;
    pi0->SetYRange(-1.2, 1.2) ;
    if(idecay == 3){
      pi0->SetPtRange(0., 80.) ;
      //select electrons from decay
      pi0->SetCutOnChild(1);
      pi0->SetPdgCodeParticleforAcceptanceCut(11);
      pi0->SetChildYRange(-1.2, 1.2);
      pi0->SetChildPtRange(pttrigminConfig, 10000.);
      Printf("pi0: Apply pt cut %f",pttrigminConfig);
    }
    else{
      pi0->SetPtRange(0., 50.) ;
      Printf("pi0: Don't apply pt cut %f",pttrigminConfig);
    }
    ctl->AddGenerator(pi0,  "pi0", 1.);

    AliGenParam *eta = new AliGenParam(1, plib, AliGenPHOSlib::kEtaFlat);
    eta->SetPhiRange(0., 360.) ;
    eta->SetYRange(-1.2, 1.2) ;
    if(idecay == 3){
      eta->SetPtRange(0., 80.) ;
      //select electrons from decay
      eta->SetCutOnChild(1);
      eta->SetPdgCodeParticleforAcceptanceCut(11);
      eta->SetChildYRange(-1.2, 1.2);
      eta->SetChildPtRange(pttrigminConfig, 10000.);
      Printf("eta: Apply pt cut %f",pttrigminConfig);
    }
    else{
      eta->SetPtRange(0., 50.) ;
      Printf("eta: Don't apply pt cut %f",pttrigminConfig);
    }
    ctl->AddGenerator(eta,  "eta", 1.);
  }
  //
  return ctl;
}
