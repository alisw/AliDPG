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

ReconstructionConfig(AliReconstruction &rec, EReconstruction_t tag, Int_t run)
{

  switch(tag) {

    // Default
  case kReconstructionDefault:
    ReconstructionDefault(rec, run);
    return;
    
    // Muon
  case kReconstructionMuon:
    ReconstructionDefault(rec, run);
    rec.SetRunReconstruction("MUON ITS VZERO");
    return;
    
    // Custom
  case kReconstructionCustom:
    if ((gROOT->LoadMacro("ReconstructionCustom.C")) != 0) {
      printf("ERROR: cannot find ReconstructionCustom.C\n");
      abort();
      return;
    }
    ReconstructionCustom(rec, run);
    return;

  }  

}

ReconstructionDefault(AliReconstruction &rec, Int_t run)
{
  gROOT->LoadMacro("$ALIDPG_ROOT/MC/Utils.C");
  Int_t year = RunToYear(run);
    //
    //    // set OCDB snapshot mode
    //    AliCDBManager *man = AliCDBManager::Instance();
    //    man->SetDefaultStorage("alien://Folder=/alice/data/2015/OCDB");
    //    man->SetRun(run);
    //    man->SetSnapshotMode("OCDBrec.root");
    rec.SetCDBSnapshotMode("OCDBrec.root");
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
