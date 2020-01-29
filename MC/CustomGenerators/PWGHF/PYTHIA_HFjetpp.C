AliGenerator *
GeneratorCustom(TString opt = "")
{
    // set the xmldoc path using PYTHIA8DATA enviroement var
    gSystem->Setenv("PYTHIA8DATA", gSystem->ExpandPathName("$ALICE_ROOT/PYTHIA8/pythia8/xmldoc"));

    AliGenCocktail *ctl  = GeneratorCocktail("PYTHIA_HF");
    
    Int_t process[2] = {kPythia6HeavyProcess_Charm, kPythia6HeavyProcess_Beauty};
    Int_t decay[5]   = {kPythia6HeavyDecay_Hadrons, kPythia6HeavyDecay_HadronsWithV0, kPythia6HeavyDecay_Electron, kPythia6HeavyDecay_All, kPythia6HeavyDecay_All_bDecaysEvtGen};
    
    const Char_t *label[2][5] = {
        "chadr PYTHIA", "chadr PYTHIA", "cele PYTHIA", "ccbar PYTHIA", "ccbar PYTHIA+EvtGen",
        "bchadr PYTHIA", "bchadr PYTHIA", "bele PYTHIA", "bbbar PYTHIA", "bbbar PYTHIA+EvtGen"
    };
    
    Int_t iprocess = -1;
    
    if(opt.Contains("cc")) {iprocess = 0; printf("-- HF process = %s -- \n",opt.Data());}
    else if(opt.Contains("bb")) {iprocess = 1; printf("-- HF process = %s -- \n",opt.Data());}
    else {printf("-- no HF process set = %s -- \n",opt.Data());}
    
    TString optList[6] = {"had", "hv0", "ele", "all", "bEvtGen", "jetjet"};
    Int_t idecay = 0;
    for (Int_t iopt = 0; iopt < 6; iopt++){
        if (opt.Contains(optList[iopt]))
            idecay = iopt;
    }
    
    AliGenerator *phf;
    if(idecay < 5) {
        phf  = GeneratorPythia6Heavy(process[iprocess], decay[idecay], kPythia6Tune_Perugia2011, kFALSE);
        ctl->AddGenerator(phf, label[iprocess][idecay], 1.);
        printf(">>>>> added generator for HF production:  %s \n", label[iprocess][idecay]);
    }
    else if(idecay == 5) {
        phf  = GeneratorPythia6Jets(kPythia6Tune_Perugia2011);
        ctl->AddGenerator(phf, "jetjet", 1.);
        printf(">>>>> added generator for HF production:  %s \n", "jetjet");
    }

    if(idecay == 4) { //EvtGen for b-decays
      AliGenEvtGen *gener = new AliGenEvtGen();
      gener->SetParticleSwitchedOff(AliGenEvtGen::kBeautyPart);
      ctl->AddGenerator(gener,"EvtGen",1.);
    }

    return ctl;
}
