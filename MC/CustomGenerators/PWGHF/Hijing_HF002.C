AliGenerator *
GeneratorCustom(TString opt = "")
{
  AliGenCocktail *ctl  = GeneratorCocktail("Hijing_HF");
  Float_t randHF = gRandom->Rndm();
  if(randHF>0.176797){
    AliGenerator   *hij  = GeneratorHijing();
    ctl->AddGenerator(hij, "Hijing", 1.);
  }
  //
  Int_t process[2] = {kPythia6HeavyProcess_Charm, kPythia6HeavyProcess_Beauty};
  Int_t decay[3]   = {kPythia6HeavyDecay_Hadrons, kPythia6HeavyDecay_HadronsWithV0, kPythia6HeavyDecay_Electron};

  const Char_t *label[2][3] = {
    "chadr PYTHIA", "chadr PYTHIA", "cele PYTHIA",
    "bchadr PYTHIA", "bchadr PYTHIA", "bele PYTHIA"
  };
  Int_t iprocess = uidConfig % 2;
  TString optList[3] = {"had", "hv0", "ele"};
  Int_t idecay = 0;
  for (Int_t iopt = 0; iopt < 3; iopt++){
    if (opt.EqualTo(optList[iopt]))
      idecay = iopt;
  }
  //
  AliGenerator *phf  = GeneratorPythia6Heavy(process[iprocess], decay[idecay], kPythia6Tune_Perugia2011, kFALSE);
    //
    ctl->AddGenerator(phf, label[iprocess][idecay], 1.);
    printf(">>>>> added HF generator %s \n", label[iprocess][idecay]);
    // add pi0 and eta enhancement
    if (decay[idecay] == kPythia6HeavyDecay_Electron) {
      AliGenPHOSlib *plib = new AliGenPHOSlib();
      AliGenParam *pi0 = new AliGenParam(1, plib, AliGenPHOSlib::kPi0Flat);
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
    return ctl;
}
