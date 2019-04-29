/*
   rec.C to be used for pass0
   - reconstruction of raw data
   - QA information switched off
   - store all friends
   - default OCDB storage set to "raw://"

   Example:
   aliroot -b -q 'recCPass0.C("raw.root",100)'
*/

void recCPass0(const char *filename="raw.root",Int_t nevents=-1, const char *ocdb="raw://", const char* options="?Trigger=kCalibBarrel")
{

  // in case of specific storages (typically for local running
  if (gSystem->AccessPathName("localOCDBaccessConfig.C", kFileExists)==0) {        
    gROOT->LoadMacro("localOCDBaccessConfig.C");
  }

  TString cdbMode = gSystem->Getenv("OCDB_SNAPSHOT_CREATE");
  if (cdbMode == "kTRUE") {
    // macro to create the snapshot
    gROOT->LoadMacro("$ALIDPG_ROOT/MC/CreateSnapshot.C");
  }
  
  // main macro
  if (gSystem->AccessPathName("main_recCPass0.C", kFileExists)==0) {
    Printf("Using local main_recCPass0.C");
    gROOT->Macro(TString::Format("main_recCPass0.C(\"%s\", %d, \"%s\", \"%s\")", filename, nevents, ocdb, options));
  }
  else {
    gROOT->Macro(TString::Format("$ALIDPG_ROOT/DataProc/CPass0/main_recCPass0.C(\"%s\", %d, \"%s\", \"%s\")", filename, nevents, ocdb, options));
  }
}

