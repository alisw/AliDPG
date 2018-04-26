/*
   rec.C to be used for pass0
   - reconstruction of raw data
   - QA information switched off
   - store all friends
   - default OCDB storage set to "raw://"

   Example:
   aliroot -b -q 'recCPass1.C("raw.root",100)'
*/

void recCPass1(const char *filename="raw.root",Int_t nevents=-1, const char *ocdb="raw://", const char* options="?Trigger=kCalibBarrelMB")
{

  // in case of specific storages (typically for local running)
  if (gSystem->AccessPathName("localOCDBaccessConfig.C", kFileExists)==0) {
    gROOT->LoadMacro("localOCDBaccessConfig.C");
  }
  else { // default settings
    AliCDBManager* man = AliCDBManager::Instance();
    man->SetRaw(kTRUE);
  }
  TString cdbMode = gSystem->Getenv("OCDB_SNAPSHOT_CREATE");
  if (cdbMode == "kTRUE") {
    // macro to create the snapshot
    gROOT->LoadMacro("$ALIDPG_ROOT/MC/CreateSnapshot.C");
  }
  
  // main macro
  if (gSystem->AccessPathName("main_recCPass1.C", kFileExists)==0) {
    Printf("Using local main_recCPass1.C");
    gROOT->Macro(TString::Format("main_recCPass1.C(\"%s\", %d, \"%s\", \"%s\")", filename, nevents, ocdb, options));
  }
  else {
    gROOT->Macro(TString::Format("$ALIDPG_ROOT/DataProc/CPass1/main_recCPass1.C(\"%s\", %d, \"%s\", \"%s\")", filename, nevents, ocdb, options));
  }

}

