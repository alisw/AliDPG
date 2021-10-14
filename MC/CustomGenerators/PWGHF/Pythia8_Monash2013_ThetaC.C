AliGenerator * GeneratorCustom()
{
    AliGenCocktail *ctl = (AliGenCocktail*) GeneratorCocktail("Monash2013_ThetaC");

    // pythia8
    AliGenPythiaPlus* py8 = (AliGenPythiaPlus*)GeneratorPythia8(kPythia8Tune_Monash2013);
    py8->SetMaximumLifetime(0.7);
    ctl->AddGenerator(py8, "Pythia8 (Monash2013)", 1.);

    // D0 decays
    // we only force D* and D0, no need to force other decays
    (AliPythia8::Instance())->ReadString("413:onMode = off");
    (AliPythia8::Instance())->ReadString("413:onIfMatch = 421 211");
    (AliPythia8::Instance())->ReadString("421:onMode = off");
    (AliPythia8::Instance())->ReadString("421:onIfMatch = 321 211");

    ctl->UseSingleInjectionPerEvent();
    AliGenerator *inj = GeneratorInjector(1, 9422111, 0., 50., -2., 2.);
    ctl->AddGenerator(inj, "Theta_c_3100 injector", 1.);

    return ctl;
}
