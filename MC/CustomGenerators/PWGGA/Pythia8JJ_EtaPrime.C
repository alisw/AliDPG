AliGenerator* GeneratorCustom(TString opt = ""){
    
    // Pythia
    AliGenerator *gen    = GeneratorPythia8JetsParticleTrg(kPythia8Tune_Monash2013,kCalorimeterAcceptance_FullDetector,331,1.2);

    // force decay: 
    // eta prime -> pi plus pi minus eta    100%
    // eta -> gamma gamma                   100%
    (AliPythia8::Instance())->ReadString("331:onMode = off");
    (AliPythia8::Instance())->ReadString("331:onIfMatch = 211 -211 221");
    (AliPythia8::Instance())->ReadString("221:onMode = off");
    (AliPythia8::Instance())->ReadString("221:onIfMatch = 22 22");

    return gen;
}
