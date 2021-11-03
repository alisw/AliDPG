AliGenerator *GeneratorCustom ()  {
  
    AliGenPythiaPlus *pythia = (AliGenPythiaPlus*)GeneratorPythia8(kPythia8Tune_Monash2013);
  
    //Leading Particle p_{T}
    Float_t pt_leading = 5.0;
    pythia->SetTriggerParticleMinPt(pt_leading);
    pythia->SetHadronInBarrel(kTRUE);

    return pythia;
}
