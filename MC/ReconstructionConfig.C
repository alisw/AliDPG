/*
 * AliDPG - ALICE Experiment Data Preparation Group
 * Reconstruction steering script
 *
 */

/*****************************************************************/
/*****************************************************************/
/*****************************************************************/

#if (!defined(__CLING__) && !defined(__CINT__)) || defined(__ROOTCLING__) || defined(__ROOTCINT__)
#include "AliMCEventHandler.h"
#include "AliMagF.h"
#endif

enum EReconstruction_t {
  kReconstructionDefault,
  kReconstructionMuon,
  kReconstructionMuonOnly,
  kReconstructionPhosOnly,
  kReconstructionITSpureSA,
  kReconstructionNoSDD,  
  kReconstructionRun1TrackingPID,
  kReconstructionForNuclei,
  kReconstructionRun3,
  kReconstructionCustom,
  kReconstructionNoTPC,
  kReconstructionNoEMCal,
  kNReconstructions
};

const Char_t *ReconstructionName[kNReconstructions] = {
  "Default",
  "Muon",
  "MuonOnly",
  "PhosOnly",
  "ITSpureSA",
  "NoSDD",
  "Run1TrackingPID",
  "OptimisedForNuclei",
  "Run3",
  "Custom",
  "NoTPC",
  "NoEMCal"
};

void ReconstructionDefault(AliReconstruction &rec);
void ReconstructionRun3(AliReconstruction &rec);
void SetCDBRun3(int run);

AliITSRecoParam *OverrideITSRecoParam_VertexerSmearMC();
AliITSRecoParam *OverrideITSRecoParam_NoSDD_pPb2016();
AliITSRecoParam *OverrideITSRecoParam_ITSpureSA_PbPb2015();

/*****************************************************************/

AliReconstruction* gg_tmp_rec;
void ReconstructionConfig(AliReconstruction &rec, int tag_tmp)
{
  EReconstruction_t tag = (EReconstruction_t) tag_tmp;
  printf(">>>>> ReconstructionConfig: %s \n", ReconstructionName[tag]);

  /** fast magnetic field **/
  if (gSystem->Getenv("CONFIG_FASTB")) {  
    if (!AliMagF::Class()->GetMethodAny("SetFastFieldDefault")) {
      printf("ERROR: fast magnetic field is requested, but AliRoot version is not compatible\n");
      abort();
    }
    printf(">>>>> Using fast magnetic field \n");
    AliMagF::SetFastFieldDefault(kTRUE);
 }

  /** Flag to impose or revert at run-time the bugfix in AliITStrackerMI **/
  Bool_t doWithBug=kFALSE;
  if(gSystem->Getenv("ALIEN_JDL_LPMANCHORYEAR")){
    Int_t year = atoi(gSystem->Getenv("ALIEN_JDL_LPMANCHORYEAR"));
    if(year<2018){
      if(gSystem->Getenv("ALIEN_JDL_LPMANCHORPASSNAME")){
	TString passname=gSystem->Getenv("ALIEN_JDL_LPMANCHORPASSNAME");
	TString periodname=gSystem->Getenv("CONFIG_PERIOD");
	// the bug should be imposed for reconstructions done with AliRoot<v5-09-21
	// pass1 of data colelcted before 2017
	// pass4 of pp 2010
	// pass2, pass3 and pass4 of LHC15n
	// pass2_UD an lowIR passes of Pb-Pb LHC15o
	if(passname.Contains("pass1")) doWithBug=kTRUE;
	if(year==2010 && passname.Contains("pass4")) doWithBug=kTRUE;
	if(periodname=="LHC15n"){
	  if(passname=="pass4" || passname=="pass3" || passname=="pass2") doWithBug=kTRUE;
	}
	if(periodname=="LHC15o"){
	  if(passname.Contains("pass5_lowIR") || passname.Contains("pass4_lowIR") ||
	     passname.Contains("pass3_lowIR") || passname.Contains("pass2_lowIR") ||
	     passname.Contains("pass2_UD")) doWithBug=kTRUE;
	}
      }
    }
  }
  Bool_t hasBugFixed=kFALSE;
  if(gSystem->Getenv("ALIEN_JDL_PACKAGES")){
    // Bug fix introfuced in v5-09-21
    // No need to re-impose it for AliRoot<v5-09-21
    TString packg=gSystem->Getenv("ALIEN_JDL_PACKAGES");
    Int_t pos1=packg.Index("AliPhysics");
    TString subs=packg(pos1,packg.Length()-pos1);
    Int_t pos2=subs.Index("VO_ALICE");
    if(pos2<=0) pos2=subs.Length();
    TString aliph=subs(0,pos2);
    Int_t ver,n1,n2;
    Char_t str2[20];
    sscanf(aliph.Data(),"AliPhysics::v%d-%d-%02d%s",&ver,&n1,&n2,str2);
    if(ver>5)  hasBugFixed=kTRUE;
    else if(ver==5){
      if(n1>9 || (n1==9 && n2>=21)) hasBugFixed=kTRUE;
    }
  }
  if(!hasBugFixed){
    printf("AliRoot version <v5-09-21 no need to impose the fAfterV0 bug\n");
  }
  if(hasBugFixed && doWithBug){
    if (AliITStrackerMI::Class()->GetMethod("SetFixBugAfterV0","false")) {
      printf("Call SetFixBugAfterV0(kFALSE) to impose the fAfterV0 bug to match the corresponding raw data pass\n");
      gROOT->ProcessLine("AliITStrackerMI::SetFixBugAfterV0(kFALSE)");
    }else{
      printf("ERROR: imposing of V0 bug in AliITStrackerMI is requested, but AliRoot version is not compatible\n");
      abort();
    }
  }

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
    mcHandler->SetPreReadMode((AliMCEventHandler::PreReadMode_t) 1);
    mcHandler->SetReadTR(kFALSE);
    //
    AliMCEventHandler* mcHandlerBg = new AliMCEventHandler();
    mcHandlerBg->SetInputPath(bgDir.Data());
    mcHandlerBg->SetPreReadMode((AliMCEventHandler::PreReadMode_t) 1);
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

    // Muon only
  case kReconstructionMuonOnly:
    ReconstructionDefault(rec);
    rec.SetRunReconstruction("MUON ITS");
    rec.SetRunQA("MUON:ALL");
    rec.SetWriteESDfriend(kFALSE);
    rec.SetRunPlaneEff(kFALSE);
    rec.SetRecoParam("ITS",OverrideITSRecoParam_VertexerSmearMC());
    return;

    // Phos only
  case kReconstructionPhosOnly:
    ReconstructionDefault(rec);
    rec.SetRunReconstruction("PHOS");
    rec.SetRunTracking("PHOS");
    rec.SetFillESD("PHOS");
    rec.SetRunQA(":");
    rec.SetWriteESDfriend(kFALSE);
    rec.SetRunPlaneEff(kFALSE);
    rec.SetRunMultFinder(kFALSE);
    rec.SetRunVertexFinder(kFALSE);
    return;
    
    // ITSpureSA
  case kReconstructionITSpureSA:
    ReconstructionDefault(rec);
    if (system.EqualTo("Pb-Pb") || system.EqualTo("Xe-Xe"))
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

  case kReconstructionForNuclei:
    ReconstructionDefault(rec);
    const char* conf = gSystem->Getenv("ALIEN_JDL_CONFIG_HE3_PION_THRESHOLD");
    if (conf) {
      int threshold = atoi(conf);
      threshold = threshold ? threshold : 130;
      rec.SetPIDforTrackingOptimisedForNuclei(threshold);
    }
    return;

    // Run3
  case kReconstructionRun3:
    ReconstructionRun3(rec);
    return;
    
    // Custom
  case kReconstructionCustom:
    if ((gROOT->LoadMacro("ReconstructionCustom.C")) != 0) {
      printf("ERROR: cannot find ReconstructionCustom.C\n");
      abort();
      return;
    }
    gg_tmp_rec = &rec;
    gROOT->ProcessLine("ReconstructionCustom(*gg_tmp_rec);");
    return;
    
    // NoTPC
  case kReconstructionNoTPC:
    ReconstructionDefault(rec);
    rec.SetRunReconstruction("ALL -TPC -HLT");

    // NoEMCal
  case kReconstructionNoEMCal:
    ReconstructionDefault(rec);
    rec.SetRunReconstruction("ALL -EMCAL");
  }  

}

/*****************************************************************/

void ReconstructionDefault(AliReconstruction &rec)
{

  Int_t year = atoi(gSystem->Getenv("CONFIG_YEAR"));

  //
  // set OCDB source
  TString ocdbConfig = "default,snapshot";
  if (gSystem->Getenv("CONFIG_OCDB"))
    ocdbConfig = gSystem->Getenv("CONFIG_OCDB");
  if (ocdbConfig.Contains("alien") || ocdbConfig.Contains("cvmfs")) {
    // set OCDB 
    gROOT->LoadMacro("$ALIDPG_ROOT/MC/OCDBConfig.C");
    gROOT->ProcessLine("OCDBDefault(1);");
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
    rec.SetFractionFriends(0.01);
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

void ReconstructionRun3(AliReconstruction &rec)
{

  Int_t year = atoi(gSystem->Getenv("CONFIG_YEAR"));
  //
  int runNumber = atoi(gSystem->Getenv("DC_RUN"));
  SetCDBRun3(runNumber);
  
  gPluginMgr->AddHandler("AliReconstructor", "ITS",
                         "AliITSUReconstructor","ITS", "AliITSUReconstructor()");

  rec.SetRunReconstruction("ALL -MFT"); // run cluster finder
  rec.SetRunMultFinder(kFALSE);   // to be implemented - CreateMultFinder
  rec.SetRunPlaneEff(kFALSE);     // to be implemented - CreateTrackleter

  rec.SetCleanESD(kFALSE);
  rec.SetStopOnError(kFALSE);
  rec.SetWriteESDfriend();
  rec.SetWriteAlignmentData();
  rec.SetFractionFriends(0.01);
  rec.SetRunPlaneEff(kTRUE);
  rec.SetUseTrackingErrorsForAlignment("ITS");
  //
  rec.SetRunQA(":");
 
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

/*****************************************************************/

AliITSRecoParam *
OverrideITSRecoParam_VertexerSmearMC()
{
  AliITSRecoParam *itsRecoParam = AliITSRecoParam::GetLowFluxParam();
  
  // use MC vertex with smearing
  itsRecoParam->SetVertexerSmearMC();
  itsRecoParam->ReconstructOnlySPD();
  
  //
  return itsRecoParam;
}

void SetCDBRun3(int run)
{
  // set OCDB source
  TString ocdbConfig = "default,snapshot";
  AliCDBManager *cdbm = AliCDBManager::Instance();
  if (!gSystem->AccessPathName("OCDBrec.root")) {
    // set OCDB snapshot mode
    cdbm->SetSnapshotMode("OCDBrec.root");
    cdbm->SetDefaultStorage("local://");
    //    cdbm->SetSnapshotMode("OCDBsim.root");
  }
  else if (gSystem->Getenv("CONFIG_OCDBCUSTOM")) {
    gROOT->LoadMacro("OCDBCustom.C");
    gROOT->ProcessLine("OCDBDefault(0);");
  }
  else {
    if (gSystem->Getenv("CONFIG_OCDB")) ocdbConfig = gSystem->Getenv("CONFIG_OCDB");
    if (ocdbConfig.Contains("alien") || ocdbConfig.Contains("cvmfs")) {
      // set OCDB 
      gROOT->LoadMacro("$ALIDPG_ROOT/MC/OCDBRun3.C");
      gROOT->ProcessLine("OCDBDefault(1);");
    }
  }
  cdbm->SetSpecificStorage("GRP/GRP/Data", "local://./");

  AliCDBManager::Instance()->SetRun(run);
}
