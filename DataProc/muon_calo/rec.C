void rec(const char *filename="raw.root", const char* options="")
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //
  // Reconstruction script for RAW data
  //
  /////////////////////////////////////////////////////////////////////////////////////////


  // Upload CDB entries from the snapshot (local root file) if snapshot exist
  if (gSystem->AccessPathName("OCDB.root", kFileExists)!=0) { // we don't have the snapshot        
    //
    TString cdbMode = gSystem->Getenv("OCDB_SNAPSHOT_CREATE");
    if (cdbMode == "kTRUE") {
      gROOT->LoadMacro("$ALIDPG_ROOT/MC/CreateSnapshot.C");
    }
  }


  if(gSystem->Exec("ls main_rec.C > /dev/null 2>&1")==0){
    printf("Use main_rec.C macro passed as input\n");
    gROOT->Macro(TString::Format("main_rec.C(\"%s\", \"%s\")", filename, options));
  }else{
    printf("Use main_rec.C macro from AliDPG\n");
    gROOT->Macro(TString::Format("$ALIDPG_ROOT/DataProc/muon_calo/main_rec.C(\"%s\", \"%s\")", filename, options));
  }
}
