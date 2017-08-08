AliGenerator *
GeneratorCustom(TString opt = "d")
{
    //Generator meant to create events destined for material budget variations: special variant
    //in the future: pass more as command line arguments and write less custom generators...
    TString optList[4] = {"a", "b", "c", "d"};
    Int_t ninjk0[4] = {10, 5, 1, 10};
    Int_t ninjla[4] = {10, 5, 1, 10};
    Int_t ninjxi[4] = {20, 8, 1, 10};
    Int_t ninjom[4] = {17, 4, 1, 10};
    Int_t iinj = 0;
    for (Int_t iopt = 0; iopt < 4; iopt++)
        if (opt.EqualTo(optList[iopt]))
            iinj = iopt;
    Int_t sign = uidConfig % 2 == 0 ? 1 : -1;
    AliGenCocktail *ctl  = GeneratorCocktail("Hijing_Str003");
    AliGenerator   *hij  = GeneratorHijing();
    AliGenerator   *ik0 = GeneratorInjector(ninjk0[iinj],         310, 0., 3., -0.7, 0.7);
    AliGenerator   *ila = GeneratorInjector(ninjla[iinj], sign * 3122, 0., 3., -0.7, 0.7);
    AliGenerator   *ixi = GeneratorInjector(ninjxi[iinj], sign * 3312, 0., 3., -0.7, 0.7);
    AliGenerator   *iom = GeneratorInjector(ninjom[iinj], sign * 3334, 0., 3., -0.7, 0.7);
    ctl->AddGenerator(hij,  "Hijing",            1.);
    ctl->AddGenerator(ik0, "Injector (K0s)", 1.);
    ctl->AddGenerator(ila, "Injector (Lambda)", 1.);
    ctl->AddGenerator(ixi, "Injector (Xi)", 1.);
    ctl->AddGenerator(iom, "Injector (Omega)", 1.);
    return ctl;
}
