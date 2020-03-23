AliGenerator* GeneratorCustom(TString opt = ""){
    
    // Pythia
    AliGenerator *gen    = GeneratorPythia6(kPythia6Tune_Perugia2011,331,1.2);
    AliGenPythia *pythia = (AliGenPythia*) gen;

    // settings
    pythia->SetForceDecay(kEtaPrime);
    
    return gen;
    
}
