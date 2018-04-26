/*
  Template of calibration/filtering macro using ESD:
  - requires AliESDs.root and AliESDfriend.root
  - requires OCDB access (default set to "raw://")
  - requires run number as argument to init OCDB
  - calls LoadLibraries.C, ConfigCalibTrain.C and AddTaskTPCCalib.C macros
  - output CalibObjects.root with TPC and TRD calibration objects are created

  Example:
  .L $ALICE_ROOT/ANALYSIS/macros/runCalibTrain.C
  runCalibTrain("104892");
*/

void runCalibTrain(Int_t runNumber, const char *inFileName = "AliESDs.root", const char *ocdb="raw://")
{

  //
  // macro that starts the runCalibTrain
  // needed to work with ROOT6
  //

  // needed libraries
  //  gROOT->Macro("$ALICE_PHYSICS/PWGPP/CalibMacros/CPass1/LoadLibraries.C");

  // include path (still needed for ROOT5 but not for ROOT6)
  gSystem->SetIncludePath("-I$ALICE_PHYSICS/include -I$ALICE_ROOT/include"); 

  // clean geometry task
  if (gSystem->Exec("cp $ALICE_PHYSICS/PWGPP/CalibMacros/commonMacros/CleanGeom.C ./") ||
      gROOT->LoadMacro("CleanGeom.C++")) { // comple local copy only
    printf("Failed to load/compile CleanGeom, exit\n");
    return;
  }
  
  // in case there are some specific storages (for local testing)
  if (gSystem->AccessPathName("localOCDBaccessConfig.C", kFileExists)==0) {
    Printf("Loading localOCDBaccessConfig");
    gROOT->LoadMacro("localOCDBaccessConfig.C");
  }

  // configuration of the OCDB for the calib train
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/CalibMacros/CPass1/ConfigCalibTrain.C");

  // detector tasks
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/CalibMacros/CPass1/AddTaskTPCCalib.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/CalibMacros/CPass1/AddTaskTRDCalib.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/CalibMacros/CPass1/AddTOFAnalysisTaskCalibPass0.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/CalibMacros/CPass1/AddTOFAnalysisTaskCalibTree.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/CalibMacros/CPass1/AddTaskT0Calib.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/CalibMacros/CPass1/AddTaskT0Analysis.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/CalibMacros/CPass1/AddTaskMeanVertexCalib.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/CalibMacros/CPass1/AddTaskSDDCalib.C"); 
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/AD/AddTaskADCalib.C");


  // now running the main macro
  if (gSystem->AccessPathName("main_runCalibTrain.C", kFileExists)==0) {
    Printf("Using local main_runCalibTrain.C");
    gROOT->Macro(TString::Format("main_runCalibTrain.C(%d, \"%s\", \"%s\")", runNumber, inFileName, ocdb));
  }
  else {
    gROOT->Macro(TString::Format("$ALIDPG_ROOT/DataProc/CPass1/main_runCalibTrain.C(%d, \"%s\", \"%s\")", runNumber, inFileName, ocdb));
  }
  return;
  
}
