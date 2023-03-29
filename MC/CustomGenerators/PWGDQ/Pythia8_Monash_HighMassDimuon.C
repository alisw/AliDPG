AliGenerator *GeneratorCustom(TString opt = "")

{
    
    AliGenPythiaPlus* pyth = (AliGenPythiaPlus*)GeneratorPythia8(kPythia8Tune_Monash2013);
    
    pyth->SetProcess(kPyHeavyFlavppMNRwmi);
    
    pyth->SetCutOnChild(1);
    pyth->SetPdgCodeParticleforAcceptanceCut(13);
    pyth->SetChildYRange(-4.1, -2.4);
    pyth->SetChildPtRange(0, 10000.);
    
    pyth->SetStackFillOpt(AliGenPythiaPlus::kHeavyFlavor);
    
    return pyth;
    
}