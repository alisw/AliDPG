AliGenerator *
GeneratorCustom()
{
  AliGenCocktail *ctl  = GeneratorCocktail("Perugia2011_HF");
  //
  Int_t process[2] = {kPythia6HeavyProcess_Charm, kPythia6HeavyProcess_Beauty};
  const Char_t *label[2] = {"chadr PYTHIA", "bchadr PYTHIA"};
  Int_t iprocess = uidConfig % 2;
  AliGenerator *phf  = GeneratorPythia6Heavy(process[iprocess], kPythia6HeavyDecay_Hadrons, kPythia6Tune_Perugia2011, kFALSE);
  ctl->AddGenerator(phf, label[iprocess], 1.);
  printf(">>>>> added HF generator %s \n", label[iprocess]);
  return ctl;
}
