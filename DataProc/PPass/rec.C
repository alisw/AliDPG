void rec(const char *filename="raw.root", const char* options="")
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //
  // Reconstruction script for RAW data
  //
  /////////////////////////////////////////////////////////////////////////////////////////

  
  AliReconstruction rec;
  // Set reconstruction flags (skip detectors here if neded with -<detector name>

  // do we extract the TPC recpoints in advance
  Int_t preclustTPC = TString(gSystem->Getenv("preclusterizeTPC")).Atoi();
  Bool_t noTPCLocalRec = preclustTPC==1;
  if (noTPCLocalRec) printf("preclustTPC = %d: TPC local reconstruction assumed to be already done\n", preclustTPC);
  else printf("preclustTPC = %d: TPC local reconstruction assumed to NOT be already done\n", preclustTPC);
  //
  if (noTPCLocalRec) {
    rec.SetRunReconstruction("ALL -HLT");
    rec.SetRunLocalReconstruction("ALL -TPC -HLT");
  }
  else {
    rec.SetRunLocalReconstruction("ALL");
  } 

  /** fast magnetic field **/
  Bool_t useFast=kTRUE;
  if(gSystem->Getenv("ALIEN_JDL_DISABLEVDTANDFASTB")){
    TString optionFast=gSystem->Getenv("ALIEN_JDL_DISABLEVDTANDFASTB");
    if(optionFast!=""){
      printf("Fast magnetic field is disabled via ALIEN_JDL_DISABLEVDTANDFASTB\n");
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



  // removing apparently pile-up clusters to speadup reconstruction
  const double kZOutSectorCut = 3.; // cut on clusters on wrong side of CE (added to extendedRoadZ)
  AliTPCReconstructor::SetZOutSectorCut(kZOutSectorCut);


  // QA options
  //  rec.SetRunQA("Global:ESDs") ;
  //  rec.SetRunQA(":") ;
  //  rec.SetRunQA("ALL:ALL") ;
  rec.SetRunQA("Global MUON:ALL") ;
  rec.SetQARefDefaultStorage("local://$ALICE_ROOT/QAref") ;

  // AD stuff (see ALIROOT-6663)

  // rec.SetOption("AD", "+SaturationCorrection"); // --> not needed anymore, see https://alice.its.cern.ch/jira/browse/ALIROOT-6778

  // AliReconstruction settings
  rec.SetWriteESDfriend(kTRUE);
  rec.SetWriteAlignmentData();
  TString filenamewithopt = filename;
  filenamewithopt += options;
  rec.SetInput(filenamewithopt.Data());
  rec.SetUseTrackingErrorsForAlignment("ITS");

  // Specific AD storage, see https://alice.its.cern.ch/jira/browse/ALIROOT-6056
  //  rec.SetSpecificStorage("AD/Calib/TimeSlewing", "alien://Folder=/alice/simulation/2008/v4-15-Release/Ideal");

  // switch off cleanESD
  rec.SetCleanESD(kFALSE);

  //Ignore SetStopOnError
  rec.SetStopOnError(kFALSE);

  // Delete recpoints
  // Remove recpoints after each event
  TString delRecPoints="TPC TRD";
  if (noTPCLocalRec) delRecPoints.ReplaceAll("TPC","");
  rec.SetDeleteRecPoints(delRecPoints.Data()); 
  //

  // Set 0.7% as fraction of friends (Ruben, 2017-08-22)
  rec.SetFractionFriends(0.007);

  AliLog::Flush();
  rec.Run();

}
