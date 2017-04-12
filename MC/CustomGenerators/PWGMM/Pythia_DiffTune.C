AliGenerator *
GeneratorCustom()
{ 
      comment = comment.Append(" pp: Pythia low-pt (Perugia0) diff tune");

      AliGenPythia *pythia = GeneratorPythia6(kPythia6Tune_Perugia0); 
      
      pythia->SetMomentumRange(0, 999999.);
      pythia->SetThetaRange(0., 180.);
      pythia->SetYRange(-12.,12.);
      pythia->SetPtRange(0,1000.);
      pythia->SetProcess(kPyMb);
      pythia->SetEnergyCMS(energyConfig);
      pythia->UseNewMultipleInteractionsScenario();
      pythia->SetCrossingAngle(0,0.000280);
      pythia->SetTuneForDiff();
//
      return pythia;
}
