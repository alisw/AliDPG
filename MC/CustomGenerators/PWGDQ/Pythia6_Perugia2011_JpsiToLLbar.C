AliGenerator *
GeneratorCustom()
{
  AliGenCocktail *ctl   = GeneratorCocktail("Pythia6_Perugia2011_JpsiToLLbar");
  AliGenerator   *pyt   = GeneratorPythia6(kPythia6Tune_Perugia2011);
  ctl->AddGenerator(pyt,  "Pythia6", 1.);

  if (uidConfig % 10 < 7) {
    AliGenerator *jpsi  = Generator_JpsiToLLbar("UserParam_pp13TeV", 1.0,0.,kFALSE);
    ctl->AddGenerator(jpsi, "Jpsi2LL", 1., NULL, 1);
    TFile *file = new TFile("typeHF_4.proc", "recreate");
    file->Close();
  }
  else {
    AliGenerator *bjpsi = Generator_JpsiToLLbar("Pythia BBar", 0.0, 1.0,kTRUE);
    ctl->AddGenerator(bjpsi, "B2Jpsi2LL", 1.);
    TFile *file = new TFile("typeHF_5.proc", "recreate");
    file->Close();
  }

  return ctl;
}
