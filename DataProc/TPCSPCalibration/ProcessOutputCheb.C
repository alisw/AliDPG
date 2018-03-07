Bool_t ProcessOutputCheb(TString filesToProcess, Int_t startRun, Int_t endRun, const char* ocdbStorage, 
			 Bool_t corr=kTRUE, Bool_t dist=kTRUE) {

  // macro that process a list of files (xml or txt) to produce then the
  // OCDB entry for the TPC SP Distortion calibration; inspired by
  // AliAnalysisAlien::MergeOutput for simplicity

  if (!gSystem->AccessPathName("CreateCorrMapObj.C")) {
    gROOT->LoadMacro("CreateCorrMapObj.C");
    ::Info("ProcessOutputCheb","using local version of ProcessOutputCheb.C");
  }
  else {
    gROOT->LoadMacro("$ALIDPG_ROOT/DataProc/TPCSPCalibration/CreateCorrMapObj.C");
    ::Info("ProcessOutputCheb","using %s","$ALIDPG_ROOT/DataProc/TPCSPCalibration/CreateCorrMapObj.C");
  }

  Bool_t retCode = gROOT->Macro(TString::Format("$ALIDPG_ROOT/DataProc/TPCSPCalibration/main_ProcessOutputCheb.C(\"%s\", %d, %d, \"%s\", %d, %d)", filesToProcess.Data(), startRun, endRun, ocdbStorage, (Int_t)corr, (Int_t)dist));
  
  return retCode;
  
}
