// Custom generator for J/psi in the barrel, with the Monash 2013 tune of Pythia8
// pythia8 will be triggered on jpsi in the barrel (|y|<1.5) with pt>0
AliGenerator *GeneratorCustom(TString opt = "")
{

    TVirtualMCDecayer* decayer = new AliDecayerPythia();
    //decayer->SetForceDecay(kAll);
    ((AliDecayerPythia*)decayer)->SwitchOffParticle(pdg);
    decayer->Init();
    gMC->SetExternalDecayer(decayer);

    AliGenPythiaPlus* pythia8 = (AliGenPythiaPlus*)GeneratorPythia8(kPythia8Tune_Monash2013);
    pythia8->SetTriggerParticle(443, 1.5);
    pythia8->SetTriggerY(1.5);
    pythia8->SetMomentumRange(0, 999999.);
    pythia8->SetThetaRange(0., 180.);
    pythia8->SetYRange(-2., 2.);
    pythia8->SetPtRange(0, 1000.);

    // not sure if this is needed, if not also remove from cocktail 
    AliGenEvtGen *evtgen = new AliGenEvtGen();
    evtGen->SetForceDecay(kDiElectron);
    evtGen->SetParticleSwitchedOff(AliGenEvtGen::kCharmPart);      
    evtGen->Init();

    AliGenCocktail *gener = new AliGenCocktail();
    gener->UsePerEventRates();

    // cocktail
    gener->AddGenerator(pythia8, "pythia8jpsiTrig", 1.);
    gener->AddGenerator(evtgen, "EvtGen", 1.);

    //pyth->SetProcess(kPyHeavyFlavppMNRwmi);
    
    /*
    pyth->SetCutOnChild(1);
    pyth->SetPdgCodeParticleforAcceptanceCut(13);
    pyth->SetChildYRange(-4.1, -2.4);
    pyth->SetChildPtRange(0, 10000.);
    
    pyth->SetStackFillOpt(AliGenPythiaPlus::kHeavyFlavor);
    */

    return gener;    
}