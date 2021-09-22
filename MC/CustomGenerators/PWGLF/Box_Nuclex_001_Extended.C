AliGenerator *GeneratorCustom()  {
    
    AliGenCocktail *ctl = (AliGenCocktail*) GeneratorCocktail("Box_Nuclex_001_Extended");

    const Int_t pdgcodes[6] = {
        2212,
        1000010020,
        1000020030,
        1000010030,
        1000020040,
        1010010030
    };
  
    const Char_t *names[6] = {
        "Proton",
        "Deuteron",
        "Helium-3",
        "Triton",
        "Helium-4",
        "Hypertriton"
    };
    
    int npart = 20;
    
    double maxpt[6] = {10.0,10.0,10.0,10.0,10.0,10.0};
  
    for (Int_t ipart = 0; ipart < 6; ++ipart) {
        for (Int_t negative = 0; negative < 2; negative++) {
            
            AliGenBox *box = new AliGenBox(npart);
            Int_t pdg = pdgcodes[ipart];
            if (negative) pdg = -pdg;
            box->SetPart(pdg);
            box->SetPtRange(0., maxpt[ipart]);
            box->SetPhiRange(0., 360.);
            box->SetYRange(-0.5,0.5);
            ctl->AddGenerator(box, Form("%s%i",names[ipart],negative), 1);
        }
    }
  return ctl;
}
