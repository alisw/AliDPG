AliGenerator * GeneratorCustom() {

  // ====================================================
  // set environment variables

  // xml path for Pb-Pb at 2.76 TeV
  gSystem->Setenv("THERM2_PARAMS_XML_PATH", "alien:/alice/cern.ch/user/m/mbuczyns/Therminator2_custom_hypersurface/lhyquid3v");
  

  // ====================================================
  // create Therminator2 generator
  AliGenerator *therminator2 = GeneratorTherminator2();
  
  return therminator2;
  
}
