void main_rec(const char *filename="raw.root", const char* options="")
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
      gInterpreter->ProcessLine(TString::Format("CreateSnapshot(\"OCDB.root\", \"%s\")", filename));
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
  TString filenamewithopt = filename;
  filenamewithopt += options;
  rec.SetInput(filenamewithopt.Data());
  rec.SetUseTrackingErrorsForAlignment("ITS");

  /** fast magnetic field **/
  Bool_t useFast=kFALSE; // disable fast B due to issue in the muon arm (07-May-2018)
  if(gSystem->Getenv("ALIEN_JDL_DISABLEFASTB")){
    TString optionFast=gSystem->Getenv("ALIEN_JDL_DISABLEFASTB");
    if(optionFast!=""){
      printf("Fast magnetic field is disabled via ALIEN_JDL_DISABLEFASTB\n");
      useFast=kFALSE;
    }
  }
  if (useFast && !AliMagF::Class()->GetMethodAny("SetFastFieldDefault")) {
    printf("ERROR: fast magnetic field is requested, but AliRoot version is not compatible\n");
    useFast=kFALSE;
  }
  if(useFast){
      printf(">>>>> Using fast magnetic field \n");
      AliMagF::SetFastFieldDefault(kTRUE);
  }

  // Upload CDB entries from the snapshot (local root file) if snapshot exist
  if (gSystem->AccessPathName("OCDB.root", kFileExists)==0) {
    rec.SetCDBSnapshotMode("OCDB.root");
  }

  // switch off cleanESD
  rec.SetCleanESD(kFALSE);

  //Ignore SetStopOnError
  rec.SetStopOnError(kFALSE);

 // Delete recpoints
  rec.SetDeleteRecPoints("TPC TRD");

  // Set 100% of friends
  // rec.SetFractionFriends(2.0);

  AliLog::Flush();
  rec.Run();

}
