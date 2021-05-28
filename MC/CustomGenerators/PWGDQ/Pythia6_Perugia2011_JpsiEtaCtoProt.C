AliGenerator *
GeneratorCustom()
{
  AliGenCocktail *ctl   = GeneratorCocktail("Pythia6_Perugia2011_JpsiEtaCtoProt");
  AliGenerator   *pyt   = GeneratorPythia6(kPythia6Tune_Perugia2011);
  ctl->AddGenerator(pyt,  "Pythia6", 1.);
  AliGenerator *jpsi_etac  = Generator_JpsiEtaToProton("pp13TeV", 0.5, 0.5);
  ctl->AddGenerator(jpsi_etac, "JpsiEtaCtoProt", 1., NULL, 2);
  TFile *file = new TFile("typeHF_4.proc", "recreate");
  file->Close();
  return ctl;
}
