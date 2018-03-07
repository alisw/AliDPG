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
  TString cdbMode = gSystem->Getenv("OCDB_SNAPSHOT_CREATE");
  if (cdbMode == "kTRUE") {
    // macro to configure the snapshot
    gROOT->LoadMacro("$ALIDPG_ROOT/MC/OCDBConfig.C");
    gROOT->LoadMacro("$ALIDPG_ROOT/MC/CreateSnapshot.C");
  }
  gROOT->Macro(TString::Format("$ALIDPG_ROOT/DataProc/PPass/main_rec.C(\"%s\", \"%s\")", filename, options));
  
}
