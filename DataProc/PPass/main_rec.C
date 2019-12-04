void main_rec(const char *filename="raw.root", const char* options="")
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //
  // Reconstruction script for RAW data
  //
  /////////////////////////////////////////////////////////////////////////////////////////

  
  AliReconstruction rec;

  TString evtPerJobStr = gSystem->Getenv("EVENTS_PER_JOB");
  Int_t evtPerJob = evtPerJobStr.Atoi()-1;
  if (evtPerJob >= 0) {
    printf("Limiting to %d events\n", evtPerJob+1);
    rec.SetEventRange(0, evtPerJob);
  }

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
  AliCDBManager* man = AliCDBManager::Instance();

  if (gSystem->AccessPathName("OCDB.root", kFileExists)==0) {        
    man->SetDefaultStorage("local://");
    man->SetRaw(kFALSE);
    man->SetSnapshotMode("OCDB.root");
  }
  else {
    // setup ocdb by custom (if any) or default settings
    if (gSystem->AccessPathName("OCDBconfig.C", kFileExists)==0) {
      gROOT->ProcessLine("OCDBconfig.C");
    }
    else if (gSystem->AccessPathName("localOCDBaccessConfig.C", kFileExists)==0) {
      // Alternative OCDB setup (release validation/override)
      man->SetDefaultStorage("raw://");
      TString envRunNumBuf = gSystem->Getenv("ALIEN_JDL_LPMRUNNUMBER");
      Int_t envRunNum = envRunNumBuf.Atoi();
      if (envRunNum > 0) man->SetRun(envRunNum);
      gInterpreter->ProcessLine("localOCDBaccessConfig();");
    }
    else { // default settings
      man->SetRaw(kTRUE);
    }
    //
    TString cdbMode = gSystem->Getenv("OCDB_SNAPSHOT_CREATE");
    if (cdbMode == "kTRUE") {
      if (!man->IsDefaultStorageSet()) man->SetRaw(kTRUE);
      gROOT->ProcessLine(TString::Format("CreateSnapshot(\"OCDB.root\", \"%s\")", filename));
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

  // switch off cleanESD if LPMESDCLEANOFF is set to 1 in the JDL (default now is ON)
  TString envClean = gSystem->Getenv("ALIEN_JDL_LPMESDCLEANOFF");
  rec.SetCleanESD( envClean.Atoi() == 1 ? kFALSE : kTRUE );

  //Ignore SetStopOnError
  rec.SetStopOnError(kFALSE);

  // keep digits for TRD
  if(gSystem->Getenv("ALIEN_JDL_DOTRDDIGITSFILTERING")) rec.SetOption("TRD", "cw,dc");
  
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
