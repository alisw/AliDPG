void merge(const char *filesToMerge="FilterEvents_Trees.root")
{
  TGrid::Connect("alien://");
  if (!gGrid || !gGrid->IsConnected()) {
    ::Error("QAtrain", "No grid connection");
    return;
  }
    
  // Set temporary compilation directory to current one
  gSystem->SetBuildDir(gSystem->pwd(), kTRUE);
    
  // Set temporary merging directory to current one
  gSystem->Setenv("TMPDIR", gSystem->pwd());
    
  AliAnalysisManager *mgr  = new AliAnalysisManager("Analysis Train", "Production train");
  mgr->SetCacheSize(0);

  AliESDInputHandler *esdHandler = new AliESDInputHandler();
  mgr->SetInputEventHandler(esdHandler);  
   
  AliAODHandler* aodHandler   = new AliAODHandler();
  aodHandler->SetOutputFileName("AliAOD.root");
  mgr->SetOutputEventHandler(aodHandler);
  
  mgr->SetFileInfoLog("fileinfo.log");
  
  // Merging method. No staging and no terminate phase.
  TStopwatch timer;
  timer.Start();
  TString outputDir = "wn.xml";
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
       printf("Output file <%s> found. Not merging again.",outputFile.Data());
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
  AliAnalysisManager *mgr  = new AliAnalysisManager("Manager for merging", "Manager for calibration merging");
  mgr->InitAnalysis();
  mgr->SetGridHandler(new AliAnalysisAlien);
  mgr->StartAnalysis("gridterminate",0);
  ofstream out;
  out.open("outputs_valid", ios::out);
  out.close();
  timer.Print();
}
