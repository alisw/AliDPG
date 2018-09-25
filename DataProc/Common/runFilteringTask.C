void runFilteringTask( const char* esdList,   
		       Float_t scalingTracks,
		       Float_t scalingV0,
		       const char* ocdb = "cvmfs://", //local:///cvmfs/alice.gsi.de/alice/data/2010/OCDB/" ,
		       Int_t nFiles = 1000000,  
		       Int_t firstFile=0, 
		       Int_t nEvents=30000000, 
		       Int_t firstEvent =0,
		       const char* esdFileName="AliESDs.root",
		       Bool_t mc=kFALSE)
{

    gSystem->SetIncludePath("-I$ROOTSYS/include -I$ALICE_PHYSICS/include -I$ALICE_PHYSICS/ITS -I$ALICE_PHYSICS -I$ALICE_PHYSICS/TRD");

    gSystem->Load("libANALYSIS");
    gSystem->Load("libANALYSISalice");
    gSystem->Load("libTender");
    gSystem->Load("libCORRFW");
    gSystem->Load("libPWGUDbase");
    gSystem->Load("libTPCcalib");
    gSystem->Load("libPWGPP");
    gSystem->Load("libPWGLFspectra");

    gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/PilotTrain/AddTaskCDBconnect.C");

    if (gSystem->AccessPathName("localOCDBaccessConfig.C", kFileExists)==0) {
      gROOT->LoadMacro("localOCDBaccessConfig.C");
    }

    gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/macros/AddTaskFilteredTree.C");


  // now running the main macro
  if (gSystem->AccessPathName("main_runFilteringTask.C", kFileExists)==0) {
    Printf("Using local main_runFilteringTask.C");
    gROOT->Macro(TString::Format("main_runFilteringTask.C(\"%s\", %f, %f, \"%s\", %d, %d, %d, %d, \"%s\", %d)", esdList, scalingTracks, scalingV0, ocdb, nFiles, firstFile, nEvents, firstEvent, esdFileName, (Int_t)mc));
  }
  else {
    gROOT->Macro(TString::Format("$ALIDPG_ROOT/DataProc/Common/main_runFilteringTask.C(\"%s\", %f, %f, \"%s\", %d, %d, %d, %d, \"%s\", %d)", esdList, scalingTracks, scalingV0, ocdb, nFiles, firstFile, nEvents, firstEvent, esdFileName, (Int_t)mc));
    
}
