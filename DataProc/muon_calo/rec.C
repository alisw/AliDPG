void rec(const char *filename="raw.root")
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //
  // Reconstruction script for 2010 RAW data
  //
  /////////////////////////////////////////////////////////////////////////////////////////

  AliReconstruction rec;

  // Upload CDB entries from the snapshot (local root file) if snapshot exist
  if (gSystem->AccessPathName("OCDB.root", kFileExists)!=0) { // we don't have the snapshot        
    //
    TString cdbMode = gSystem->Getenv("OCDB_SNAPSHOT_CREATE");
    if (cdbMode == "kTRUE") {
      gROOT->LoadMacro("$ALIDPG_ROOT/MC/OCDBConfig.C");   
      gROOT->LoadMacro("$ALIDPG_ROOT/MC/CreateSnapshot.C");
    }
  }

  gROOT->Macro(TString::Format("main_rec.C(\"%s\")", filename));
  
}
