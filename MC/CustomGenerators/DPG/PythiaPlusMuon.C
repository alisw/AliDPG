AliGenerator *GeneratorCustom(TString opt = "")
{   comment = comment.Append(" PYTHIA8, Monash, min. bias + 1 muon per event with AliGenBox");

  AliGenCocktail *ctl  = static_cast<AliGenCocktail*>(GeneratorCocktail("Hijing_HF")); 
  AliGenerator *phf=GeneratorPythia8(kPythia8Tune_Monash2013);
  ctl->AddGenerator(phf, "pythia8", 1.);
  printf(">>>>> added generator pythia8 Monash \n");

  Int_t iprocess= uidConfig %2;// used just as a random number

  AliGenBox *pGB1=new AliGenBox(1);
  if(iprocess==0)pGB1->SetPart(13);
  else pGB1->SetPart(-13);
  pGB1->SetPtRange(0.2,50);
  pGB1->SetPhiRange(0,360);
  pGB1->SetYRange(-1,1);
  ctl->AddGenerator(pGB1,"gbMuon",1);    

  return ctl;
}
