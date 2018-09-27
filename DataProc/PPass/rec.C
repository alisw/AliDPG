void rec(const char *filename="raw.root", const char* options="")
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //
  // Reconstruction script for RAW data
  //
  /////////////////////////////////////////////////////////////////////////////////////////

  
  // setup ocdb by custom (if any) or default settings
  if (gSystem->AccessPathName("OCDBconfig.C", kFileExists)==0) {
    gROOT->LoadMacro("OCDBconfig.C");
  }
  // Alternative OCDB setup (release validation/override)
  if (gSystem->AccessPathName("localOCDBaccessConfig.C", kFileExists)==0) {
    gROOT->LoadMacro("localOCDBaccessConfig.C");
  }
  TString cdbMode = gSystem->Getenv("OCDB_SNAPSHOT_CREATE");
  if (cdbMode == "kTRUE") {
    gROOT->LoadMacro("$ALIDPG_ROOT/MC/CreateSnapshot.C");
  }
  if (gSystem->AccessPathName("main_rec.C", kFileExists)==0) {
    Printf("Using local main_rec.C");
    gROOT->Macro(TString::Format("main_rec.C(\"%s\", \"%s\")", filename, options));
  }
  else {
    gROOT->Macro(TString::Format("$ALIDPG_ROOT/DataProc/PPass/main_rec.C(\"%s\", \"%s\")", filename, options));
  }
}
