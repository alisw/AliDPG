AliGenerator *
GeneratorCustom()
{
    AliGenCocktail *ctl = (AliGenCocktail*) GeneratorCocktail("Box_Nuclex003");
    //
    AliGenBox *box = new AliGenBox(160);
    Int_t pdg = -1000020030;
    box->SetPart(pdg);
    box->SetPtRange(0., 12.);
    box->SetPhiRange(0., 360.);
    box->SetYRange(-0.8, 0.8);
    //
    ctl->AddGenerator(box, "anti-3He", 1.0);
    //
    return ctl;

}
