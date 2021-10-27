AliGenerator *GeneratorCustom (TString opt = "")  {
  
    AliGenerator *pythia = GeneratorPythia8(kPythia8Tune_Monash2013);
  
    //Leading Particle p_{T}
    Float_t pt_leading = 5.0;
    
    pythia->SetTriggerParticle(+2212,0.8,0.15,200.0);
    pythia->SetTriggerParticle(-2212,0.8,0.15,200.0);
    pythia->SetTriggerParticle(+2112,0.8,0.15,200.0);
    pythia->SetTriggerParticle(-2112,0.8,0.15,200.0);
    pythia->SetTriggerParticleMinPt(pt_leading);
  
    return pythia;
}
