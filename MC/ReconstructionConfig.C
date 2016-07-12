/*
 * AliDPG - ALICE Experiment Data Preparation Group
 * Reconstruction steering script
 *
 */

/*****************************************************************/
/*****************************************************************/
/*****************************************************************/

enum EReconstruction_t {
  kReconstructionDefault,
  kReconstructionMuon,
  kReconstructionCustom,
  kNReconstructions
};

const Char_t *ReconstructionName[kNReconstructions] = {
  "Default",
  "Muon",
  "Custom"
};

/*****************************************************************/

ReconstructionConfig(AliReconstruction &rec, EReconstruction_t tag)
{

  switch(tag) {

    // Default
  case kReconstructionDefault:
    ReconstructionDefault(rec);
    return;
    
    // Muon
  case kReconstructionMuon:
    ReconstructionDefault(rec);
    rec.SetRunReconstruction("MUON ITS VZERO");
    return;
    
    // Custom
  case kReconstructionCustom:
    if ((gROOT->LoadMacro("ReconstructionCustom.C")) != 0) {
      printf("ERROR: cannot find ReconstructionCustom.C\n");
      abort();
      return;
    }
    ReconstructionCustom(rec);
    return;

  }  

}

ReconstructionDefault(AliReconstruction &rec)
{

  Int_t year = atoi(gSystem->Getenv("CONFIG_YEAR"));

  //
  // set OCDB source
  TString ocdbConfig = "default,snapshot";
  if (gSystem->Getenv("CONFIG_OCDB"))
    ocdbConfig = gSystem->Getenv("CONFIG_OCDB");
  if (ocdbConfig.Contains("alien")) {
    // set OCDB 
    gROOT->LoadMacro("$ALIDPG_ROOT/MC/OCDBConfig.C");
    OCDBDefault(1);
  }
  else {
    // set OCDB snapshot mode
    rec.SetCDBSnapshotMode("OCDBsim.root");
    //    AliCDBManager *cdbm = AliCDBManager::Instance();
    //    cdbm->SetSnapshotMode("OCDBsim.root");
  }

    //
    rec.SetCleanESD(kFALSE);
    rec.SetStopOnError(kFALSE);
    rec.SetWriteESDfriend();
    rec.SetWriteAlignmentData();
    rec.SetFractionFriends(.1);
    rec.SetRunPlaneEff(kTRUE);
    rec.SetUseTrackingErrorsForAlignment("ITS");
    //
    // IMPORTANT: For pPb productions anchored to 2013 runs, this setting has to be also added in the rec.C using AliRoot from v5-03-Rev-28 (announced in the Physics Board the 6th of June)
    if (year == 2013)
      rec.SetOption("TPC","PID.OADB=TSPLINE3_MC_%s_LHC13B2_FIXn1_PASS1_PPB_MEAN");
    //
    rec.SetRunQA(":");
    //
}
