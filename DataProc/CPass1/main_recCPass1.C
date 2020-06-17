/*
   rec.C to be used for pass0
   - reconstruction of raw data
   - QA information switched off
   - store all friends
   - default OCDB storage set to "raw://"

   Example:
   aliroot -b -q 'recCPass1.C("raw.root",100)'
*/

void main_recCPass1(const char *filename="raw.root",Int_t nevents=-1, const char *ocdb="raw://", const char* options="?Trigger=kCalibBarrelMB")
{

  Printf("\n\n in main_recCPass1");
  
  Printf("filename = %s", filename);
  if (gSystem->Getenv("ALIROOT_FORCE_COREDUMP"))
  {
    printf("ALIROOT_FORCE_COREDUMP set\n");
    gSystem->ResetSignal(kSigFloatingException);
    gSystem->ResetSignal(kSigSegmentationViolation);
  }

  // removing apparently pile-up clusters to speadup reconstruction
  const double kZOutSectorCut = 3.; // cut on clusters on wrong side of CE (added to extendedRoadZ)
  AliTPCReconstructor::SetZOutSectorCut(kZOutSectorCut);

  // Load some system libs for Grid and monitoring
  // Set the CDB storage location
  AliCDBManager * man = AliCDBManager::Instance();
  man->SetDefaultStorage(ocdb);
  
  // Reconstruction settings
  AliReconstruction rec;
  //
  // do we extract the TPC recpoints in advance
  Int_t preclustTPC = TString(gSystem->Getenv("preclusterizeTPC")).Atoi();
  Bool_t noTPCLocalRec = preclustTPC==1;
  if (noTPCLocalRec) printf("preclustTPC = %d: TPC local reconstruction assumed to be already done\n", preclustTPC);
  else printf("preclustTPC = %d: TPC local reconstruction assumed to NOT be already done\n", preclustTPC);
  //
  if (gSystem->Getenv("disableOuter")!=NULL){
    TString disOuter = gSystem->Getenv("disableOuter");
    TString disOuterLoc = disOuter;
    if (noTPCLocalRec) {
      disOuterLoc.ReplaceAll("TPC","");
      disOuterLoc.ReplaceAll("HLT","");
    }
    rec.SetRunReconstruction(disOuter.Data());
    rec.SetRunLocalReconstruction(disOuterLoc.Data());
  } 
  else if (noTPCLocalRec) {
    rec.SetRunReconstruction("ALL -HLT");
    rec.SetRunLocalReconstruction("ALL -TPC -HLT");
  }
  else {
    rec.SetRunLocalReconstruction("ALL");
    // Adding here the HLT hack (see https://alice.its.cern.ch/jira/browse/ALIROOT-6630?focusedCommentId=174164&page=com.atlassian.jira.plugin.system.issuetabpanels:comment-tabpanel#comment-174164)
    // rec.SetOption("TPC","useRAWorHLT"); // removed for LHC16i, since we are running with HLT clusters
  }

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
    man->SetDefaultStorage("local://");
    man->SetRaw(kFALSE);
    man->SetSnapshotMode("OCDB.root");
  }
  else {
    // setup ocdb by custom (if any) or default settings
    if (gSystem->AccessPathName("localOCDBaccessConfig.C", kFileExists)==0) {
      TString envRunNumBuf = gSystem->Getenv("ALIEN_JDL_LPMRUNNUMBER");
      Int_t envRunNum = envRunNumBuf.Atoi();
      if (envRunNum > 0) man->SetRun(envRunNum);
      gInterpreter->ProcessLine("localOCDBaccessConfig();");
    }
    else { // default settings
      AliCDBManager* man = AliCDBManager::Instance();
      man->SetRaw(kTRUE);
    }
    TString cdbMode = gSystem->Getenv("OCDB_SNAPSHOT_CREATE");
    if (cdbMode == "kTRUE") {
      gInterpreter->ProcessLine(TString::Format("CreateSnapshot(\"OCDB.root\", \"%s\")", filename));
      return;
    }
  }

  // All friends
  TString collSystem(gSystem->Getenv("ALIEN_JDL_LPMINTERACTIONTYPE"));
  Float_t fractionFriends = 0.5;
  if (collSystem == "PbPb" || collSystem == "XeXe") fractionFriends = 0.2;
  else if (collSystem == "pA" || collSystem == "Ap") fractionFriends = 1.0;
  rec.SetFractionFriends(fractionFriends);

 // AliReconstruction settings - hardwired MB trigger for calibration

  TString newfilename = filename;
  newfilename += options;
  rec.SetInput(newfilename.Data());

  // Set protection against too many events in a chunk (should not happen)
  if (nevents>0) rec.SetEventRange(0,nevents);

  // Remove recpoints after each event
  TString delRecPoints="TPC TRD ITS";
  if (noTPCLocalRec) delRecPoints.ReplaceAll("TPC","");
  rec.SetDeleteRecPoints(delRecPoints.Data()); 
  //

  // Switch off the V0 finder - saves time!
  rec.SetRunMultFinder(kTRUE);
  rec.SetRunV0Finder(kFALSE); 

  //
  // QA options - all QA is off
  //
  rec.SetRunQA(":");
  rec.SetRunGlobalQA(kFALSE);

  // AliReconstruction settings
  rec.SetWriteESDfriend(kTRUE);
  rec.SetWriteAlignmentData();
  rec.SetUseTrackingErrorsForAlignment("ITS");
  rec.SetCleanESD(kFALSE);

  // ITS options (see ALIROOT-7179)
  float itsErrY2[6]={0., 0., 0.1*0.1, 0.1*.1, 0.,0.};
  TVectorF *vecITSErrY2 = new TVectorF(6, itsErrY2);
  AliITSReconstructor::SetExtraErrorY2(vecITSErrY2);

  //Ignore SetStopOnError
  rec.SetStopOnError(kFALSE);

  AliLog::Flush();
  rec.Run();
}

