AliGenerator* GeneratorCustom(TString opt = ""){
    
    // Pythia
    AliGenerator *gen    = GeneratorPythia8JetsParticleTrg(kPythia8Tune_Monash2013,kCalorimeterAcceptance_FullDetector,331,1.2);

    // settings
    gen->SetForceDecay(kEtaPrime);
    
    return gen;
    
}
