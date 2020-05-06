AliGenerator *
GeneratorCustom()
{
  AliGenCocktail *ctl   = GeneratorCocktail("Box_Nuclex001");

  const Int_t pdgcodes[5] = {
    2212,
    1000010020,
    1000020030,
    1000010030,
    1000020040
  };
  const Char_t *names[5] = {
    "Proton",
    "Deuteron",
    "Helium-3",
    "Triton",
    "Helium-4"
  };
  double maxpt[5] = {5.,5.,10.,5.,10.};
  for (Int_t ipart = 0; ipart < 5; ++ipart) {
    for (Int_t negative = 0; negative < 2; negative++) {
      AliGenBox *box = new AliGenBox(1);
      Int_t pdg = pdgcodes[ipart];
      if (negative) pdg = -pdg;
      box->SetPart(pdg);
      box->SetPtRange(0., maxpt[ipart]);
      box->SetPhiRange(0., 360.);
      box->SetYRange(-0.8,0.8);
      ctl->AddGenerator(box, Form("%s%i",names[ipart],negative), 1);
    }
  }
  return ctl;
}
