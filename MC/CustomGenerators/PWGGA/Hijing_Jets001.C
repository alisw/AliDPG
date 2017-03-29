AliGenerator *
GeneratorCustom(TString opt = "")
{
  TString optList[6] = {"", "a", "b", "c", "d", "e"};
  Int_t ninjlist[6] = {1, 10, 5, 3, 2, 1};
  Int_t iinj = 0;
  for (Int_t iopt = 0; iopt < 3; iopt++)
    if (opt.EqualTo(optList[iopt]))
      iinj = iopt;
  Int_t ninj = ninjlist[iinj];
  AliGenCocktail *ctl   = GeneratorCocktail("Hijing_Jets001");
  AliGenerator   *hij   = GeneratorHijing();
  ctl->AddGenerator(hij,  "Hijing", 1.);
  AliGenerator   *jet   = GeneratorPythia8Jets();
  ctl->AddGenerator(jet,  "Jets", 1., new TFormula(Form("ninj_%d", ninj), Form("%d", ninj)));
  return ctl;
}

