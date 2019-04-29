AliGenerator *
GeneratorCustom()
{ 
      comment = comment.Append(" pp: Pythia 6 (Perugia2011 tune) high mult");

      AliGenPythia* pythia = (AliGenPythia*)GeneratorPythia6(kPythia6Tune_Perugia2011); 

      // multiplicity threshold set to 60 in |eta| < 1.4
      pythia->SetTriggerChargedMultiplicity(60, 1.4);
      
      return pythia;
}
