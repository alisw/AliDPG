AliGenerator *
GeneratorCustom(TString opt = "")
{

  //-----------------------------------------------
  // Cocktail
  AliGenCocktail *ctl = (AliGenCocktail*) GeneratorCocktail("Run3_Generators_HIJING_with_QED");

  //-----------------------------------------------
  // HIJING
  AliGenerator   *hij  = GeneratorHijing();
  ctl->AddGenerator(hij,"Hijing",1);

  //-----------------------------------------------
  // QED electrons (numbers to be fixed)
  AliGenerator   *qed  = GeneratorQED();
  ctl->AddGenerator(qed,"QED",1);

  //-----------------------------------------------
  return ctl;
}
