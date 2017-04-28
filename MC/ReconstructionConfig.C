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
  kReconstructionNoSDD,  
  kReconstructionRun1TrackingPID,
  kReconstructionCustom,
  kNReconstructions
};

const Char_t *ReconstructionName[kNReconstructions] = {
  "Default",
  "Muon",
  "ITSpureSA",
  "NoSDD",
  "Run1TrackingPID",
  "Custom"
};

/*****************************************************************/

ReconstructionConfig(AliReconstruction &rec, EReconstruction_t tag)
{

  TString system = gSystem->Getenv("CONFIG_SYSTEM");
  
  // subsidiary handler for mc-to-mc embedding
  TString bgDir = gSystem->Getenv("CONFIG_BGEVDIR");
  if (!bgDir.IsNull()) { // add extra handler for underlaying event
    if (bgDir.BeginsWith("alien://") && !gGrid && !TGrid::Connect("alien://")) { 
      printf("Failed to create a grid connection\n"); 
      abort(); 
    }
    if (!bgDir.EndsWith("/")) bgDir += "/";
    AliMCEventHandler* mcHandler = new AliMCEventHandler();
    mcHandler->SetPreReadMode(1);
    mcHandler->SetReadTR(kFALSE);
    //
    AliMCEventHandler* mcHandlerBg = new AliMCEventHandler();
    mcHandlerBg->SetInputPath(bgDir.Data());
    mcHandlerBg->SetPreReadMode(1);
    mcHandlerBg->SetReadTR(kFALSE);
    mcHandler->AddSubsidiaryHandler(mcHandlerBg);
    //
    rec.SetMCEventHandler(mcHandler);
  }

  switch(tag) {
    
    // Default
  case kReconstructionDefault:
    ReconstructionDefault(rec);
    return;
    
    // Muon
  case kReconstructionMuon:
    ReconstructionDefault(rec);
    rec.SetRunReconstruction("MUON ITS VZERO T0 AD");
    return;

    // ITSpureSA
  case kReconstructionITSpureSA:
    ReconstructionDefault(rec);
    if (system.EqualTo("Pb-Pb"))
      rec.SetRecoParam("ITS", OverrideITSRecoParam_ITSpureSA_PbPb2015());
    return;
    
    // NoSDD
  case kReconstructionNoSDD:
    ReconstructionDefault(rec);
    rec.SetRecoParam("ITS", OverrideITSRecoParam_NoSDD_pPb2016());
    return;

    // Run1TrackingPID
  case kReconstructionRun1TrackingPID:
    ReconstructionDefault(rec);
    rec.SetRun1PIDforTracking(kTRUE);
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
    AliCDBManager *cdbm = AliCDBManager::Instance();
    cdbm->SetDefaultStorage("local://");
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
OverrideITSRecoParam_ITSpureSA_PbPb2015()
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


/*****************************************************************/

AliITSRecoParam *
OverrideITSRecoParam_NoSDD_pPb2016()
{
  AliITSRecoParam * itsRecoParam = AliITSRecoParam::GetLowFluxParam();
  itsRecoParam->SetClusterErrorsParam(2);

  // find independently ITS SA tracks 
  itsRecoParam->SetSAUseAllClusters();
  itsRecoParam->SetOuterStartLayerSA(2);

  itsRecoParam->SetAllowProlongationWithEmptyRoad(kTRUE);
    
  // larger seach windows for SA (in case of large misalignments)
  itsRecoParam->SetFactorSAWindowSizes(2);
    
  // Misalignment syst errors decided at ITS meeting 25.03.2010
  // Errors in Z reduced on 11.10.2010 for SPD and SDD
  // additional error due to misal (B off)
  itsRecoParam->SetClusterMisalErrorY(0.0010,0.0010,0.0300,0.0300,0.0020,0.0020); // [cm]
  itsRecoParam->SetClusterMisalErrorZ(0.0050,0.0050,0.0050,0.0050,0.1000,0.1000); // [cm]
  // additional error due to misal (B on)
  itsRecoParam->SetClusterMisalErrorYBOn(0.0010,0.0030,0.0500,0.0500,0.0020,0.0020); // [cm]
  itsRecoParam->SetClusterMisalErrorZBOn(0.0050,0.0050,0.0050,0.0050,0.1000,0.1000); // [cm]
  //----

  // SDD configuration 
  itsRecoParam->SetUseSDDCorrectionMaps(kTRUE); // changed 30.04.2010
  itsRecoParam->SetUseSDDClusterSizeSelection(kTRUE);
  itsRecoParam->SetMinClusterChargeSDD(30.);
  itsRecoParam->SetUseUnfoldingInClusterFinderSDD(kFALSE);

  // Plane Efficiency evaluation with tracklets Method
  itsRecoParam->SetIPlanePlaneEff(-1);
  itsRecoParam->SetComputePlaneEff(kTRUE,kFALSE);
  itsRecoParam->SetUseTrackletsPlaneEff(kTRUE);
  itsRecoParam->SetTrackleterPhiWindowL2(0.07);
  itsRecoParam->SetTrackleterZetaWindowL2(0.4);
  itsRecoParam->SetTrackleterPhiWindowL1(0.10);
  itsRecoParam->SetTrackleterZetaWindowL1(0.6);
  itsRecoParam->SetUpdateOncePerEventPlaneEff(kTRUE);
  itsRecoParam->SetMinContVtxPlaneEff(3);
  // itsRecoParam->SetOptTrackletsPlaneEff(kTRUE); // activate it for MC (very important) !
  // Removal of tracklets reconstructed in the SPD overlaps 
  // 
  itsRecoParam->SetTrackleterRemoveClustersFromOverlaps(kTRUE);
  itsRecoParam->SetTrackleterPhiWindow(0.08);
  itsRecoParam->SetTrackleterThetaWindow(0.025);
  itsRecoParam->SetTrackleterScaleDThetaBySin2T(kTRUE);
  //
  // V0 finder (A. Marin)
  itsRecoParam->GetESDV0Params()->SetMaxPidProbPionForb(0.9);

  //******************************************************************

  itsRecoParam->SetEventSpecie(AliRecoParam::kLowMult);
  itsRecoParam->SetTitle("LowMult");

  // SKIP SDD
  itsRecoParam->SetLayerToSkip(2);
  itsRecoParam->SetLayerToSkip(3);

  //
  return itsRecoParam;
}
