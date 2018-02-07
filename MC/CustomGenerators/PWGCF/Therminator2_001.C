AliGenerator * GeneratorCustom() {

  // ====================================================
  // set environment variables

  // select 3+1 hydro model
  gSystem->Setenv("THERM2_PARAMS_FreezeOutModel", "Lhyquid3D");

  // xml path for Pb-Pb at 2.76 TeV (0-5%)
  gSystem->Setenv("THERM2_PARAMS_XML_PATH", "alien:/alice/cern.ch/user/m/mbuczyns/Therminator2_custom_hypersurface/lhyquid3v/LHCPbPb2760b2.3Ti512t0.60Tf140a0.08b0.08h0.24x2.3v2.xml");
  
  // ====================================================
  // create Therminator2 generator
  AliGenerator *therminator2 = GeneratorTherminator2();
  
  return therminator2;
  
}
