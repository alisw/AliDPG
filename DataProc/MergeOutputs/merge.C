void merge(const char *filesToMerge="FilterEvents_Trees.root", const char *inputCollection="wn.xml")
{
  TString noGridConnect = gSystem->Getenv("ALIEN_JDL_NOGRIDCONNECT");
  if (noGridConnect != "1") {
    TGrid::Connect("alien://");
    if (!gGrid || !gGrid->IsConnected()) {
      ::Error("QAtrain", "No grid connection");
      return;
    }
  }

  // Set temporary compilation directory to current one
  gSystem->SetBuildDir(gSystem->pwd(), kTRUE);

  // Set temporary merging directory to current one
  gSystem->Setenv("TMPDIR", gSystem->pwd());

  AliAnalysisManager *mgr1  = new AliAnalysisManager("Analysis Train", "Production train");
  mgr1->SetCacheSize(0);

  AliESDInputHandler *esdHandler = new AliESDInputHandler();
  mgr1->SetInputEventHandler(esdHandler);

  AliAODHandler* aodHandler   = new AliAODHandler();
  aodHandler->SetOutputFileName("AliAOD.root");
  mgr1->SetOutputEventHandler(aodHandler);

  mgr1->SetFileInfoLog("fileinfo.log");

  // Merging method. No staging and no terminate phase.
  TStopwatch timer;
  timer.Start();
  TString outputDir = inputCollection;
  TString outputFiles = filesToMerge;
  TString mergeExcludes = "";
  TObjArray *list = outputFiles.Tokenize(",");
  TIter *iter = new TIter(list);
  TObjString *str;
  TString outputFile;
  Bool_t merged = kTRUE;

  while((str=(TObjString*)iter->Next())) {
    outputFile = str->GetString();
    // Skip already merged outputs
    if (!gSystem->AccessPathName(outputFile)) {
       printf("Output file <%s> found. Not merging again.", outputFile.Data());
       continue;
    }
    if (mergeExcludes.Contains(outputFile.Data())) continue;
    merged = AliAnalysisAlien::MergeOutput(outputFile, outputDir, 10, 0);
    if (!merged) {
       printf("ERROR: Cannot merge %s\n", outputFile.Data());
       continue;
    }
  }

  // all outputs merged, validate
  AliAnalysisManager *mgr2 = new AliAnalysisManager("Manager for merging", "Manager for calibration merging");
  mgr2->InitAnalysis();
  mgr2->SetGridHandler(new AliAnalysisAlien);
  mgr2->StartAnalysis("gridterminate", (TTree *)0x0);
  ofstream out;
  out.open("outputs_valid", ios::out);
  out.close();
  timer.Print();
}
