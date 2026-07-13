// Custom generator for J/psi in the barrel, with the Monash 2013 tune of Pythia8
// pythia8 will be triggered on jpsi in the barrel (|y|<1.5) with pt>0
AliGenerator *GeneratorCustom(TString opt = "")
{
    // load libraries to use Evtgen
    gSystem->Load("libPhotos");
    gSystem->Load("libEvtGen");
    gSystem->Load("libEvtGenExternal");
    gSystem->Load("libTEvtGen");

    // set external decayer
  TVirtualMCDecayer* decayer = new AliDecayerPythia();
  decayer->SetForceDecay(kAll);
  decayer->Init();
  ((AliDecayerPythia*)decayer)->SwitchOffParticle(443);
  gMC->SetExternalDecayer(decayer);

  //Generating a cocktail signal undecayed + evtgen
  AliGenCocktail *gener = new AliGenCocktail();
  gener->UsePerEventRates();

  AliGenEvtGen *gene = new AliGenEvtGen(); 
  gene->SetUserDecayTable(gSystem->ExpandPathName("$ALIDPG_ROOT/MC/CustomDecayTables/BTOPSIJPSITODIELECTRON.DEC"));
  gene->SetParticleSwitchedOff(AliGenEvtGen::kHFPart);

    /*TVirtualMCDecayer* decayer = new AliDecayerPythia();
    decayer->SetForceDecay(kAll);
    ((AliDecayerPythia*)decayer)->SwitchOffParticle(443);
    decayer->Init();
    gMC->SetExternalDecayer(decayer);*/

    AliGenPythiaPlus* pythia8 = (AliGenPythiaPlus*)GeneratorPythia8(kPythia8Tune_Monash2013);
    pythia8->SetTriggerParticle(443, 2.5);
    pythia8->SetTriggerY(2.5);
    pythia8->SetMomentumRange(0, 999999.);
    pythia8->SetThetaRange(0., 180.);
    pythia8->SetYRange(-2.5, 2.5);
    pythia8->SetPtRange(0, 1000.);
    //pythia8->SetForceDecay(kBJpsiUndecayed);
    (AliPythia8::Instance())->ReadString("443:mayDecay = off");
    cout << "Pythia8 generator configured for J/psi in the barrel with Monash 2013 tune" << endl;
    //pythia8->SetStackFillOpt(AliGenPythia::kHeavyFlavor);

    /*
    AliGenEvtGen *gene = new AliGenEvtGen(); 
    gene->SetUserDecayTable(gSystem->ExpandPathName("$ALIDPG_ROOT/MC/CustomDecayTables/BTOPSIJPSITODIELECTRON.DEC"));
    gene->SetParticleSwitchedOff(AliGenEvtGen::kHFPart);
    //gene->SetParticleSwitchedOff(AliGenEvtGen::kCharmPart);    
    */

    

    // cocktail
    gener->AddGenerator(pythia8, "pythia8jpsiTrig", 1.);
    gener->AddGenerator(gene, "EvtGen", 1.);

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