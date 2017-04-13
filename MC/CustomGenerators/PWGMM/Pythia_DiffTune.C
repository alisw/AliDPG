AliGenerator *
GeneratorCustom()
{ 
      comment = comment.Append(" pp: Pythia low-pt (Perugia0) diff tune");

      AliGenPythia *pythia = GeneratorPythia6(kPythia6Tune_Perugia0); 
      
      pythia->UseNewMultipleInteractionsScenario();
      pythia->SetTuneForDiff();
      
      return pythia;
}
