AliGenerator* 
GeneratorCustom()
{

  comment = comment.Append(" pp: Phojet diff tune");

  AliGenDPMjet* dpmjet = GeneratorPhojet(); 
      
  dpmjet->SetMomentumRange(0, 999999.);
  dpmjet->SetThetaRange(0., 180.);
  dpmjet->SetYRange(-12.,12.);
  dpmjet->SetPtRange(0,1000.);
  dpmjet->SetProcess(kDpmMb);
  dpmjet->SetEnergyCMS(energyConfig);
  dpmjet->SetCrossingAngle(0,0.000280);      
  dpmjet->SetTuneForDiff();

  return dpmjet;
}
