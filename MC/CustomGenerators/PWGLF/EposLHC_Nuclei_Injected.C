AliGenerator *GeneratorCustom()  {
    
    AliGenCocktail *ctl = (AliGenCocktail*) GeneratorCocktail("EposLHC_Nuclei_Injected");
 
    AliGenerator   *epos  = GeneratorEPOSLHC();
    ctl->AddGenerator(epos,"EPOSLHC", 1.);

    const Int_t pdgcodes[4] = {
        2212,
        1000010020,
        1000020030,
        1000010030
    };
  
    const Char_t *names[4] = {
        "Proton",
        "Deuteron",
        "Helium-3",
        "Triton"
    };
    
    int npart = 2;
  
    for (Int_t ipart = 0; ipart < 4; ++ipart) {
        for (Int_t negative = 0; negative < 2; negative++) {
            
            AliGenBox *box = new AliGenBox(npart);
            Int_t pdg = pdgcodes[ipart];
            if (negative) pdg = -pdg;
            box->SetPart(pdg);
            box->SetPtRange(1.0,10.0);
            box->SetPhiRange(0.0,360.0);
            box->SetYRange(-0.5,0.5);
            ctl->AddGenerator(box, Form("%s%i",names[ipart],negative), 1);
        }
    }
    
    return ctl;    
}











    
    
    
     
