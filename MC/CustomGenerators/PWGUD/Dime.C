AliGenerator* GeneratorCustom() {
  printf("Starting DIME generator\n");
  gSystem->Load("libEVGEN");
  gSystem->Load("libTDime");
  gSystem->Load("libdime");

  TString proc;
  if      (processConfig.Contains("Pipm")) proc = "pipm";
  else if (processConfig.Contains("Kpkm")) proc = "kpkm";
  else {
    printf("ERROR: unknown process\n");
    return 0;
  }

  TString ff;
  if      (processConfig.Contains("Orexp")) ff = "orexp";
  else if (processConfig.Contains("Exp"))   ff = "exp";
  else if (processConfig.Contains("Power")) ff = "power";
  else {
    printf("ERROR: unknown process\n");
    return 0;
  }
  
  AliGenDime* dime = new AliGenDime(1000);
  AliDimeRndm::GetDimeRandom()->SetSeed(seedConfig);
  printf("Setting new random seed: %i\n",AliDimeRndm::GetDimeRandom()->GetSeed());
  dime->GetTDime()->SetEnergyCMS(energyConfig);       // sqrt(s)
  dime->GetTDime()->SetProcess(proc.Data());          // Process {"pipm", "pi0", "kpkm", "ks", "rho"}
  dime->GetTDime()->SetFormf(ff.Data());              // Meson-Pomeron form factor {"orexp", "exp", "power"}
  dime->GetTDime()->SetFsi("true");                   // Exclusive supression {"true", "false"}
  dime->GetTDime()->SetYRange(yminConfig,ymaxConfig); // Set rapidity range of mesons
  dime->GetTDime()->SetMinPt(0.);                     // Minimum pT of mesons
  return dime;
}
