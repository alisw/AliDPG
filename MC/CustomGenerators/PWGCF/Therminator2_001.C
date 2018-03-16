AliGenerator * GeneratorCustom(TString xmlPath) {

  // ====================================================
  // set environment variables

  // select 3+1 hydro model
  gSystem->Setenv("THERM2_PARAMS_FreezeOutModel", "Lhyquid3D");

  // expand and set freezeout xml path for the selected model
  string fullPath = AliDataFile::GetFileName(xmlPath.Data());
  if(fullPath == "")
    fullPath = xmlPath;
  gSystem->Setenv("THERM2_PARAMS_XML_PATH", fullPath.c_str());
  
  // ====================================================
  // create Therminator2 generator
  AliGenerator *therminator2 = GeneratorTherminator2();
  
  return therminator2;
  
}
