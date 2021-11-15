AliGenerator *GeneratorCustom()  {
    
    AliGenCocktail *ctl = (AliGenCocktail*) GeneratorCocktail("Box_Nuclex_002_Extended");

    const Int_t pdgcodes[4] = {
        2212,
        3122,
        3312,
        3334
    };
  
    const Char_t *names[4] = {
        "Proton",
        "Lambda"
        "Xi",
        "Omega"
    };
    
    int npart = 20;
    
    double maxpt[4] = {3.,3.5,4.,5.};
  
    for (Int_t ipart = 0; ipart < 4; ++ipart) {
        for (Int_t negative = 0; negative < 2; negative++) {
            
            AliGenBox *box = new AliGenBox(npart);
            Int_t pdg = pdgcodes[ipart];
            if (negative) pdg = -pdg;
            box->SetPart(pdg);
            box->SetPtRange(0., maxpt[ipart]);
            box->SetPhiRange(0., 360.);
            box->SetYRange(-1.,1.);
            ctl->AddGenerator(box, Form("%s%i",names[ipart],negative), 1);
        }
    }
  return ctl;
}
