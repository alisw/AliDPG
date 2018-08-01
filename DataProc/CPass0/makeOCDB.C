/*
  macro to extract the OCDB entries

  input: CalibObjects.root
  ouput: TimeGain and TimeVdrift calibration objects for TPC and TRD

  Example:
  .L $ALICE_PHYSICS/PWGPP/CalibMacros/CPass0/makeOCDB.C
  makeOCDB("105160");

*/

void makeOCDB(Int_t runNumber, TString  targetOCDBstorage="", TString sourceOCDBstorage="raw://", Int_t detectorBitsQualityFlag = -1)
{

  // in case there are some specific storages (for local testing)
  if (gSystem->AccessPathName("localOCDBaccessConfig.C", kFileExists)==0) {

    Printf("Loading localOCDBaccessConfig");
    gROOT->LoadMacro("localOCDBaccessConfig.C");
  }

  // config calib train
  gROOT->LoadMacro("$ALIDPG_ROOT/DataProc/Common/ConfigCalibTrain.C");

  // main macro
  if (gSystem->AccessPathName("main_makeOCDB.C", kFileExists)==0) {
    Printf("Using local main_makeOCDB.C");
    gROOT->Macro(TString::Format("main_makeOCDB.C(%d, \"%s\", \"%s\", %d)", runNumber, targetOCDBstorage.Data(), sourceOCDBstorage.Data(), detectorBitsQualityFlag));
  }
  else {
    gROOT->Macro(TString::Format("$ALIDPG_ROOT/DataProc/CPass0/main_makeOCDB.C(%d, \"%s\", \"%s\", %d)", runNumber, targetOCDBstorage.Data(), sourceOCDBstorage.Data(), detectorBitsQualityFlag));
  }
}
