AliGenerator *GeneratorCustom(TString opt = "")
{

  AliGenCocktail *ctl  = (AliGenCocktail*) GeneratorCocktail("Hijing_Str_V0Casc_pbpb2018");

  //check if this is an embedding simulation. Otherwise, simulate hijing event before injection
  TString simulation = gSystem->Getenv("CONFIG_SIMULATION");
  bool isEmbedding=kFALSE;
  if(!simulation.Contains("Embed")){
    AliGenerator *hij = GeneratorHijing();
    ctl->AddGenerator(hij, "Hijing", 1.);
  }
  else isEmbedding=kTRUE;

  //if it is embedding, check impact parameter. If impossible, Fatal
  double bEvalFormula=-1;
  if(isEmbedding){
    if(bminConfig>bmaxConfig)Fatal("Hijing_Str_V0Casc_pbpb2018","max impact parameter smaller than min impact parameter");
    if(bminConfig<0.)Fatal("Hijing_Str_V0Casc_pbpb2018","min impact parameter <0");
    if(bmaxConfig>20)Fatal("Hijing_Str_V0Casc_pbpb2018","unphysical impact parameter");
    bEvalFormula=(bmaxConfig+bminConfig)/2.;
  }
  
  //decide ho many particles to inject, depending on the impact parameter and on the particle species
  const int nCentRanges = 4, nParticles = 4; 
  int ninj[nCentRanges][nParticles] = {{25, 50, 50, 50}, 
                                       {6,  20, 32, 24},  
                                       {4,   8,  8,  8}, 
                                       {1,   1,  1,  1}};
  int iinj = 0;
  if(isEmbedding){
    if     ( bEvalFormula>0   && bEvalFormula<6 )    iinj = 0;
    else if( bEvalFormula>5   && bEvalFormula<9 )    iinj = 1;
    else if( bEvalFormula>7.5 && bEvalFormula<12.5 ) iinj = 2;
    else if( bEvalFormula>11  && bEvalFormula<15)    iinj = 3;
  }
  else{
    TString optList[4] = {"a", "b", "c", "d"};
    for (int iopt = 0; iopt < 4; iopt++)
      if (opt.EqualTo(optList[iopt]))
        iinj = iopt;
  }

  Int_t sign = uidConfig % 2 == 0 ? 1 : -1;

  //define and add signal generators
  AliGenerator   *ik0s = GeneratorInjector(ninj[iinj][0],         310, 1.5, 20., -0.6, 0.6);
  AliGenerator   *ilam = GeneratorInjector(ninj[iinj][1], sign * 3122, 1.5, 17., -0.6, 0.6);
  AliGenerator   *ixi  = GeneratorInjector(ninj[iinj][2], sign * 3312, 0.5,  5., -0.6, 0.6);
  AliGenerator   *iom  = GeneratorInjector(ninj[iinj][3], sign * 3334, 0.5, 4.5, -0.6, 0.6);
  ctl->AddGenerator(ik0s, "Injector (K0s)",    1.);
  ctl->AddGenerator(ilam, "Injector (Lambda)", 1.);
  ctl->AddGenerator(ixi,  "Injector (Xi)",     1.);
  ctl->AddGenerator(iom,  "Injector (Omega)",  1.);

  return ctl;
}
