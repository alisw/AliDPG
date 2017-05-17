AliGenerator* GeneratorCustom() {
  AliGenStarLight *genStarLight = (AliGenStarLight *) GeneratorStarlight();
  if (processConfig.Contains("Incoh")) {
    Float_t bslope = 3.75;
    if      (systemConfig.EqualTo("p-Pb")) bslope = 6.4;   // Pb towards muon arm
    else if (systemConfig.EqualTo("Pb-p")) bslope = 3.75;  // p  towards muon arm
    printf("GeneratorCustom: b-slope = %.2f\n",bslope);
    genStarLight->SetParameter("BSLOPE_DEFINITION = 1");
    genStarLight->SetParameter(Form("BSLOPE_VALUE      = %.2f",bslope));
  }
  return genStarLight;
}
