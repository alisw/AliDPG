AliGenerator * GeneratorCustom(TString xmlPath) {

  // ====================================================
  // set environment variables

  // select 3+1 hydro model
  gSystem->Setenv("THERM2_PARAMS_FreezeOutModel", "Lhyquid3D");

  // set freezeout xml path for the selected model
  gSystem->Setenv("THERM2_PARAMS_XML_PATH", xmlPath.Data());
  
  // ====================================================
  // create Therminator2 generator
  AliGenerator *therminator2 = GeneratorTherminator2();
  
  return therminator2;
  
}
