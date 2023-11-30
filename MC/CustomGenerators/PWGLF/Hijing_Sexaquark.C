AliGenerator *GeneratorCustom(TString opt = "") {

    // `opt` should be formatted as "<reaction_channel><mass_sexaquark>"
    // - where <reaction_channel> should be replaced by a single-digit char from 'A' to 'H'
    //   - 'A' : AntiSexaquark + Neutron -> AntiLambda, K0
    //   - 'B' : AntiSexaquark + Neutron -> AntiLambda, K0, Pi-, Pi+
    //   - 'C' : AntiSexaquark + Neutron -> AntiProton, K0, K0, Pi+
    //   - 'D' : AntiSexaquark + Proton  -> AntiLambda, K+
    //   - 'E' : AntiSexaquark + Proton  -> AntiLambda, K+, Pi-, Pi+
    //   - 'F' : AntiSexaquark + Proton  -> AntiProton, K+, K0, Pi+
    //   - 'G' : AntiSexaquark + Neutron -> Xi+, Pi-
    //   - 'H' : AntiSexaquark + Proton  -> AntiProton, K+, K+, Pi0
    // - and <mass_sexaquark> corresponds to the mass of the injected anti-sexaquark
    //   - we're interested in: 1.73, 1.8, 1.87, 1.94, 2.01
    // examples of valid `opt`: "A1.8", "B1.73", "C2.01", etc.

    /* Parse option string */

    // default values
    Char_t reaction_channel = 'A';
    Double_t mass_sexaquark = 1.8;  // GeV/c^2
    if (opt != "") {
        reaction_channel = opt(0);
        mass_sexaquark = ((TString)opt(1, opt.Length())).Atof();
    }

    /* Initialize generators */

    AliGenCocktail *ctl = (AliGenCocktail *)GeneratorCocktail("Hijing+AntiNeutron+AntiSexaquark");

    AliGenHijing *hij = (AliGenHijing *)GeneratorHijing();
    ctl->AddGenerator(hij, "Hijing", 1.);

    AliGenerator *ani = GeneratorInjector(100, -2112, 1., 6., -0.8, 0.8);
    ctl->AddGenerator(ani, "Injector (Anti-Neutron)", 1.);

    AliGenSexaquarkReaction *sr = new AliGenSexaquarkReaction(20, mass_sexaquark, reaction_channel);
    sr->SetPtRange(0., 5.);                // GeV/c
    sr->SetPhiRange(0., 2 * TMath::Pi());  // radians
    sr->SetYRange(-0.8, 0.8);
    sr->SetRadiusRange(5., 180.);  // cm

    ctl->AddGenerator(sr, Form("Anti-Sexaquark Interaction (Channel %c)", reaction_channel), 1.);

    return ctl;
}
