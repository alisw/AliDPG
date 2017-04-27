void rec(const char *filename="raw.root")
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //
  // Reconstruction script for 2010 RAW data
  //
  /////////////////////////////////////////////////////////////////////////////////////////

  AliReconstruction rec;

  // Upload CDB entries from the snapshot (local root file) if snapshot exist
  if (gSystem->AccessPathName("OCDB.root", kFileExists)==0) {        
    rec.SetDefaultStorage("local://");
    rec.SetCDBSnapshotMode("OCDB.root");
  }
  else {
    // setup ocdb by custom (if any) or default settings
    if (gSystem->AccessPathName("OCDBconfig.C", kFileExists)==0) {
      gROOT->Macro("OCDBconfig.C");
    }
    else { // default settings
      AliCDBManager* man = AliCDBManager::Instance();
      man->SetRaw(kTRUE);
    }
    //
    TString cdbMode = gSystem->Getenv("OCDB_SNAPSHOT_CREATE");
    if (cdbMode == "kTRUE") {
      gROOT->LoadMacro("$ALIDPG_ROOT/MC/CreateSnapshot.C");
      CreateSnapshot("OCDB.root",filename);
      return;
    }
  }

  // Set reconstruction flags (skip detectors here if neded with -<detector name>
  rec.SetRunReconstruction("ALL -TPC -TRD -TOF -HLT -PMD");

  // QA options
  rec.SetRunQA("Global MUON:ALL") ;
  rec.SetQARefDefaultStorage("local://$ALICE_ROOT/QAref") ;

  // AliReconstruction settings
  rec.SetWriteESDfriend(kTRUE);
  rec.SetWriteAlignmentData();
  rec.SetInput(filename);
  rec.SetUseTrackingErrorsForAlignment("ITS");

  // Upload CDB entries from the snapshot (local root file) if snapshot exist
  if (gSystem->AccessPathName("OCDB.root", kFileExists)==0) {
    rec.SetCDBSnapshotMode("OCDB.root");
  }

  // switch off cleanESD
  rec.SetCleanESD(kFALSE);

  //Ignore SetStopOnError
  rec.SetStopOnError(kFALSE);

 // Delete recpoints
  rec->SetDeleteRecPoints("TPC TRD");

  // Set 100% of friends
  // rec.SetFractionFriends(2.0);

  AliLog::Flush();
  rec.Run();

}
