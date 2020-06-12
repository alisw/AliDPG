AliGenerator * GeneratorCustom(TString opt = "")
{

  //
  // Nuclei and hypernuclei injection: flat in pt up to 10 GeV/c and in rapidity within |y|<1.0
  // Central     : a - 300 nuclei or anti-nuclei, 100 hyper-nuclei or anti-hypernuclei
  // Semi-Central: b - 100 nuclei or anti-nuclei, 40 hyper-nuclei or anti-hypernuclei
  // Peripheral  : c - 15 nuclei or anti-nuclei, 5 hyper-nuclei or anti-hypernuclei
  //
  
  //-----------------------------------------------
  // Cocktail
  AliGenCocktail *ctl   = (AliGenCocktail*) GeneratorCocktail("Run3_Hijing_with_QED_Nuclex001");

  //-----------------------------------------------
  // HIJING
  AliGenerator   *hij   = GeneratorHijing();
  ctl->AddGenerator(hij,  "Hijing", 1.);
  
  //-----------------------------------------------
  // QED electrons (numbers to be fixed)
  AliGenerator   *qed  = GeneratorQED();
  ctl->AddGenerator(qed,"QED",1);
    
  //-----------------------------------------------
  //Nuclei and hypernuclei

  TString optList[3] = {"a", "b", "c"};
  Int_t ninj_nuclei[3] = {300, 100, 15};
  Int_t ninj_hypern[3] = {100,  40,  5};
  Int_t iinj = 0;

  for (Int_t iopt = 0; iopt < 3; iopt++)
    if (opt.EqualTo(optList[iopt]))
      iinj = iopt;
  
  AliGenerator   *nu1a  = Generator_Nuclex(0x18C01F, kFALSE, ninj_nuclei[iinj], 10.0, 1.0);
  AliGenerator   *nu1b  = Generator_Nuclex(0x18C01F, kTRUE, ninj_nuclei[iinj], 10.0, 1.0);
  AliGenerator   *hyp1a  = Generator_Nuclex(0x18C01F, kFALSE, ninj_hypern[iinj], 10.0, 1.0);
  AliGenerator   *hyp1b  = Generator_Nuclex(0x18C01F, kTRUE, ninj_hypern[iinj], 10.0, 1.0);
  
  ctl->AddGenerator(nu1a,  "Nuclex1a", 1.);
  ctl->AddGenerator(nu1b,  "Nuclex1b", 1.);
  ctl->AddGenerator(hyp1a, "Hyper1a", 1.);
  ctl->AddGenerator(hyp1b, "Hyper1b", 1.);
  return ctl;
}
