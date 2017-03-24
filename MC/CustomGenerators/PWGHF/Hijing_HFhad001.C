AliGenerator *
GeneratorCustom()
{
  AliGenCocktail *ctl  = GeneratorCocktail("Hijing_HF");
  AliGenerator   *hij  = GeneratorHijing();
  ctl->AddGenerator(hij, "Hijing", 1.);
  //
  Int_t process[2] = {kPythia6HeavyProcess_Charm, kPythia6HeavyProcess_Beauty};
  const Char_t *label[2] = {"chadr PYTHIA", "bchadr PYTHIA"};
  TFormula *formula = new TFormula("Signals", "max(1.,120.*(x<5.)+80.*(1.-x/20.)*(x>5.)*(x<11.)+240.*(1.-x/13.)*(x>11.))");
  Int_t iprocess = uidConfig % 2;
  AliGenerator *phf  = GeneratorPythia6Heavy(process[iprocess], kPythia6HeavyDecay_Hadrons, kPythia6Tune_Perugia2011);
  //
  Float_t pth[4] = {2.76, 20., 50., 1000.};
  Int_t ipt;
  if      ((uidConfig / 2) % 10 < 7) ipt = 0;
  else if ((uidConfig / 2) % 10 < 9) ipt = 1;
  else                               ipt = 2;
  ((AliGenPythia *)phf)->SetPtHard(pth[ipt], pth[ipt + 1]);
  ctl->AddGenerator(phf, label[iprocess], 1., formula);
  printf(">>>>> added HF generator %s \n", label[iprocess]);
  return ctl;
}
