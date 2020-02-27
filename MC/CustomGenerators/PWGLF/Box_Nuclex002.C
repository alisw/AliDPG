AliGenerator *
GeneratorCustom()
{
    AliGenCocktail *ctl = GeneratorCocktail("Box_Nuclex002");

    const Int_t pdgcode = 1010010030;
    const Char_t *name = "Hypertriton";
    for (Int_t negative = 0; negative < 2; negative++)
    {
        AliGenBox *box = new AliGenBox(1);
        Int_t pdg = pdgcode;
        if (negative)
            pdg = -pdg;
        box->SetPart(pdg);
        box->SetPtRange(0., 10.);
        box->SetPhiRange(0., 360.);
        box->SetYRange(-0.8, 0.8);
        ctl->AddGenerator(box, Form("%s%i", name, negative), 1);
    }
    return ctl;
}
