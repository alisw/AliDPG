AliGenerator *
GeneratorCustom(TString opt = "")
{

    AliPDG::AddParticlesToPdgDataBase();

    AliGenCocktail *ctl  = (AliGenCocktail*) GeneratorCocktail("Hijing_HF");
  
        AliGenerator   *hij  = GeneratorHijing();
        ctl->AddGenerator(hij, "Hijing", 1.);
    //
    
    AliGenerator *phf_c  = GeneratorPythia6Heavy(kPythia6HeavyProcess_Charm, kPythia6HeavyDecay_All, kPythia6Tune_Perugia2011);
    TFormula *formula = new TFormula("Signals_cc","50.97*exp(-0.5*((x-0.4441)*(x-0.4441)/(3.997*3.997)))");
    ctl->AddGenerator(phf_c, "call PYTHIA", 1.,formula);
    printf(">>>>> added HF generator c_all PYTHIA \n");
    
    AliGenerator *phf_b  = GeneratorPythia6Heavy(kPythia6HeavyProcess_Beauty, kPythia6HeavyDecay_All, kPythia6Tune_Perugia2011);
    TFormula *formula = new TFormula("Signals_bb","2.099*exp(-0.5*((x-0.4473)*(x-0.4473)/(3.994*3.994)))");
    ctl->AddGenerator(phf_b, "ball PYTHIA", 1.,formula);
    printf(">>>>> added HF generator b_all PYTHIA \n");
    
    
    
    //
    
    return ctl;
}

