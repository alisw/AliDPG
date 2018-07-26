AliGenerator *
GeneratorCustom(TString opt = "")
{

  //-----------------------------------------------
  // Cocktail
  AliGenCocktail *ctl = GeneratorCocktail("Run3_Generators_DPMJET_with_QED");

  //-----------------------------------------------
  // HIJING
  AliGenerator   *dpm  = GeneratorPhojet();
  ctl->AddGenerator(dpm,"Dpmjet",1);

  //-----------------------------------------------
  // QED electrons (numbers to be fixed)
  AliGenerator   *qed  = GeneratorQED();
  ctl->AddGenerator(qed,"QED",1);

  //-----------------------------------------------
  return ctl;
}
