AliGenerator * GeneratorCustom(string xmlPath) {

  // ====================================================
  // set environment variables

  // select 3+1 hydro model
  gSystem->Setenv("THERM2_PARAMS_FreezeOutModel", "Lhyquid3D");

  // expand and set freezeout xml path for the selected model
  string path = AliDataFile::GetFileName(xmlPath);
  gSystem->Setenv("THERM2_PARAMS_XML_PATH", path.c_str());
  
  // ====================================================
  // create Therminator2 generator
  AliGenerator *therminator2 = GeneratorTherminator2();
  
  return therminator2;
  
}
