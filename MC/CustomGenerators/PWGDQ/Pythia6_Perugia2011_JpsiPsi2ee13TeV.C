AliGenerator *
GeneratorCustom()
{
  AliGenCocktail *ctl   = (AliGenCocktail *) GeneratorCocktail("Pythia6_Perugia2011_JpsiPsi2ee13TeV");
  AliGenerator   *pyt   = GeneratorPythia6(kPythia6Tune_Perugia2011);
  ctl->AddGenerator(pyt,  "Pythia6", 1.);
  if (uidConfig % 10 < 7) {
    AliGenerator *jpsi  = Generator_JpsiAndPsiee("UserParam_pp13TeV", 0.7, 0.0, 0.3, kTRUE);
    ctl->AddGenerator(jpsi, "Psi2SJpsi2ee", 1., NULL, 1);
    TFile *file = new TFile("typeHF_4.proc", "recreate");
    file->Close();
  }
  else {
    AliGenerator *bjpsi = Generator_JpsiAndPsiee("Pythia BBar", 0.0, 0.0, 0.0, kFALSE);
    ctl->AddGenerator(bjpsi, "B2Jpsi2ee", 1.);
    TFile *file = new TFile("typeHF_5.proc", "recreate");
    file->Close();
  }
  return ctl;
}
