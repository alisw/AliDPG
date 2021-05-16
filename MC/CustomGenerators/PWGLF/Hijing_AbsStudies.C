AliGenerator *
GeneratorCustom(TString opt = "d")
{
    TString optList[4] = {"a", "b", "c", "d"};
    Int_t ninjH3[4] = {25, 15, 10, 5};
    Int_t ninjAntiH3[4] = {25, 15, 10, 5};
    Int_t ninjHe3[4] = {35, 25, 15, 10};
    Int_t iinj = 0;
    for (Int_t iopt = 0; iopt < 4; iopt++)
        if (opt.EqualTo(optList[iopt]))
            iinj = iopt;
    AliGenCocktail *ctl  = (AliGenCocktail*) GeneratorCocktail("Hijing_AbsStudies");
    AliGenerator   *hij  = GeneratorHijing();
    AliGenerator   *iH3 = GeneratorInjector(ninjH3[iinj], AliPID::ParticleCode(AliPID::kTriton), 0.5, 3., -1., 1.);
    AliGenerator   *iAntiH3 = GeneratorInjector(ninjAntiH3[iinj], -AliPID::ParticleCode(AliPID::kTriton), 0.5, 3., -1., 1.);
    AliGenerator   *iHe3 = GeneratorInjector(ninjHe3[iinj], AliPID::ParticleCode(AliPID::kHe3), 2., 14., -1., 1.);
    ctl->AddGenerator(hij,  "Hijing", 1.);
    ctl->AddGenerator(iH3, "Triton", 1.);
    ctl->AddGenerator(iAntiH3, "AntiTriton", 1.);
    ctl->AddGenerator(iHe3, "He3", 1.);
    return ctl;
}
