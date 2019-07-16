AliGenerator* GeneratorCustom(TString opt = ""){
    
    // Pythia
    AliGenPythiaPlus *pythia = (AliGenPythiaPlus*)GeneratorPythia6(kPythia6Tune_Perugia2011,331,1.2);
    
    // settings
    pythia->SetForceDecay(kEtaPrime);
    
    return pythia;
    
}
