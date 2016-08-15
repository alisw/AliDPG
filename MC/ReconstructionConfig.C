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
  kReconstructionITSpureSA,  
  kReconstructionCustom,
  kNReconstructions
};

const Char_t *ReconstructionName[kNReconstructions] = {
  "Default",
  "Muon",
  "ITSpureSA",
  "Custom"
};

/*****************************************************************/

ReconstructionConfig(AliReconstruction &rec, EReconstruction_t tag)
{

  TString system = gSystem->Getenv("CONFIG_SYSTEM");
  
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

    // ITSpureSA
  case kReconstructionITSpureSA:
    ReconstructionDefault(rec);
    if (system.EqualTo("Pb-Pb"))
      rec.SetRecoParam("ITS", OverrideITSRecoParam());
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

/*****************************************************************/

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
    rec.SetCDBSnapshotMode("OCDBrec.root");
    //    AliCDBManager *cdbm = AliCDBManager::Instance();
    //    cdbm->SetSnapshotMode("OCDBrec.root");
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

/*****************************************************************/

AliITSRecoParam *
OverrideITSRecoParam()
{

  AliITSRecoParam * itsRecoParam = AliITSRecoParam::GetHighFluxParam();
  itsRecoParam->SetClusterErrorsParam(2);

  // special settings for ITS-SA-PURE only at all centralities
  itsRecoParam->SetSAUseAllClusters();
  itsRecoParam->SetTrackerSAOnly(kFALSE);
  //  itsRecoParam->SetMaxSPDcontrForSAToUseAllClusters(10000000);
  itsRecoParam->SetNLoopsSA(20);
  itsRecoParam->SetPhiLimitsSA(0.002,0.07);
  itsRecoParam->SetLambdaLimitsSA(0.003,0.04);

  itsRecoParam->SetImproveWithVertex(kTRUE);
  // Misalignment syst errors decided at ITS meeting 25.03.2010
  // additional error due to misal (B off)
  itsRecoParam->SetClusterMisalErrorY(0.0010,0.0010,0.0100,0.0100,0.0020,0.0020); // [cm]
  itsRecoParam->SetClusterMisalErrorZ(0.0100,0.0100,0.0100,0.0100,0.0500,0.0500); // [cm]
  // additional error due to misal (B on)
  itsRecoParam->SetClusterMisalErrorYBOn(0.0010,0.0030,0.0250,0.0250,0.0020,0.0020); // [cm]
  itsRecoParam->SetClusterMisalErrorZBOn(0.0050,0.0050,0.0050,0.0050,0.1000,0.1000); // [cm]
  //----
  
  //Vertexer Z
  itsRecoParam->SetVertexerZ();
  
  
  // tracklets
  itsRecoParam->SetTrackleterPhiWindowL2(0.07);
  itsRecoParam->SetTrackleterZetaWindowL2(0.4);
  itsRecoParam->SetTrackleterPhiWindowL1(0.10);
  itsRecoParam->SetTrackleterZetaWindowL1(0.6);
  //
  itsRecoParam->SetTrackleterPhiWindow(0.06);
  itsRecoParam->SetTrackleterThetaWindow(0.025);
  itsRecoParam->SetTrackleterScaleDThetaBySin2T(kTRUE);
  //
  // Removal of tracklets reconstructed in the SPD overlaps 
  itsRecoParam->SetTrackleterRemoveClustersFromOverlaps(kTRUE);
  
  // SDD configuration 
  itsRecoParam->SetUseSDDCorrectionMaps(kTRUE); 
  itsRecoParam->SetUseSDDClusterSizeSelection(kTRUE);
  itsRecoParam->SetMinClusterChargeSDD(30.);
  itsRecoParam->SetUseUnfoldingInClusterFinderSDD(kFALSE);
  
  itsRecoParam->SetEventSpecie(AliRecoParam::kHighMult);
  itsRecoParam->SetTitle("HighMult");
  
  //
  return itsRecoParam;
}
