AliGenerator* 
GeneratorCustom()
{

  comment = comment.Append(" pp: Phojet diff tune");

  AliGenDPMjet* dpmjet = GeneratorPhojet(); 
      
  dpmjet->SetTuneForDiff();

  return dpmjet;
}
