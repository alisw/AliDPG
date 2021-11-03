AliGenerator *GeneratorCustom ()  {
  
    AliGenCocktail *ctl = (AliGenCocktail*) GeneratorCocktail("Pythia8_Monash2013_TriggerParticle");
    AliGenPythiaPlus *pythia = (AliGenPythiaPlus*)GeneratorPythia8(kPythia8Tune_Monash2013);

    //Leading Particle p_{T}
    Float_t pt_leading = 5.0;
    pythia->SetTriggerParticleMinPt(pt_leading);
    ctl->AddGenerator(pythia, "Pythia8 (Monash2013) Trigger Particle", 1);

    return ctl;
}
