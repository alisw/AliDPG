AliGenerator *
GeneratorCustom()
{
  AliGenCocktail *ctl   = GeneratorCocktail("Pythia6_Perugia2011_Jpsiee001");
  AliGenerator   *pyt   = GeneratorPythia6Jets(kPythia6Tune_Perugia2011);
  ctl->AddGenerator(pyt,  "Pythia6", 1.);
  if (uidConfig % 10 < 5) {
    AliGenerator *jpsi  = Generator_Jpsiee("pp 5.03", 0.7, 0.0, 0.3, 0.0);
    ctl->AddGenerator(jpsi, "Jpsi2ee", 1., NULL, 2);
    TFile *file = new TFile("typeHF_4.proc", "recreate");
    file->Close();
  }
  else {
    AliGenerator *bjpsi = Generator_Jpsiee("Pythia BBar", 0.0, 0.0, 0.0, 0.3, kFALSE, 0.7);
    ctl->AddGenerator(bjpsi, "B2Jpsi2ee", 1.);
    TFile *file = new TFile("typeHF_5.proc", "recreate");
    file->Close();
  }
  return ctl;
}
