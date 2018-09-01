// QAtrain_duo - assumes having 2 sets of input data files using 2 suffixes:
//    AliESDs_barrel.root AliESDfriends_barrel.root ...  suffix = _barrel
//    AliESDs_outer.root AliESDfriends_outer.root ...    suffix = _outer
// HOW TO RUN (for run_no=158285):
// QA job: should call:
//    ln -fs AliESDs_barrel.root AliESDs.root
//    ln -fs AliESDfriends_barrel.root AliESDfriends.root
//    ... same for all specific reco files
//    aliroot -b -q QAtrain_duo.C\(\"_barrel\"\,158285\) producing:
//      -> EventStat_temp_barrel.root QAresults_barrel.root 400_400_0_350.stat
//
//    ln -fs AliESDs_outer.root AliESDs.root
//    ln -fs AliESDfriends_outer.root AliESDfriends.root
//    ... same for all specific reco files
//    aliroot QAtrain_duo.C\(\"_outer\"\,158285\) producing:
//      -> EventStat_temp_outer.root QAresults_outer.root 400_400_0_380.stat
//
// Each merging job:
// for stages i < 5
//    aliroot -b -q QAtrain_duo.C\(\"_barrel\"\,158285\,\"wn.xml\"\,i\)
//    aliroot -b -q QAtrain_duo.C\(\"_outer\"\,158285\,\"wn.xml\"\,i\)
//      -> same output files as the QA jobs (except .stat)
// for stage 5
//    aliroot -b -q QAtrain_duo.C\(\"_barrel\"\,158285\,\"Stage_5.xml\"\,5\)
//      -> event_stat_barrel.root trending_barrel.root 145230_145230_0_120345.stat
//    aliroot -b -q QAtrain_duo.C\(\"_outer\"\,158285\,\"Stage_5.xml\"\,5\)
//      -> event_stat_outer.root trending_outer.root 145230_145230_0_132897.stat
#include "Riostream.h"

/*****************************************
When running in local mode, you need
to write a file containing, for example

export ALIEN_JDL_LPMINTERACTIONTYPE=pp
export ALIEN_JDL_LPMANCHORYEAR=2015
export ALIEN_JDL_LPMPRODUCTIONTAG=LHC15n
export ALIEN_JDL_LPMRUNNUMBER=244628
export ALIEN_JDL_LPMRAWPASSID=2

then source it
******************************************/

enum ECOLLISIONSYSTEM_t
{
    kpp,
    kPbPb,
    kpPb,
    kPbp,
    kXeXe,
    kNSystem
};

const Char_t* CollisionSystem[kNSystem] =
{
    "pp",
    "PbPb",
    "pA",
    "Ap",
    "XeXe"
};

void AddAnalysisTasks(const char *, const char *); 
void QAmerge(const char *,const char *, Int_t);
void ProcessEnvironmentVars();
void SetDoQA(Bool_t &var, Bool_t onoff, const char* name);
Bool_t CheckEnvS(const char* var,TString& envString);
void PrintSettings();

Int_t iCollisionType = 0; // 0=pp, 1=PbPb
// Trigger mask.

UInt_t kTriggerInt;

UInt_t kTriggerMuonBarrel = AliVEvent::kMUU7 | AliVEvent::kMuonUnlikeLowPt8 | AliVEvent::kMuonUnlikeLowPt0;
UInt_t kTriggerEMC   = AliVEvent::kEMC7 | AliVEvent::kEMC8 | AliVEvent::kEMCEJE | AliVEvent::kEMCEGA;
UInt_t kTriggerHM   = AliVEvent::kHighMult;
// Main trigger mask used:
UInt_t kTriggerMask;
//UInt_t kTriggerMask = 0;

Bool_t localRunning = kFALSE; // Missing environment vars will cause a crash; change it to kTRUE if running locally w/o env vars


Bool_t doStatistics   = 1;
Bool_t doCDBconnect   = 1;
Bool_t doEventStat    = 1;
Bool_t doCentrality   = 0;
Bool_t doQAsym        = 0;
Bool_t doVZERO        = 1;   // there is a 2nd file
Bool_t doVZEROPbPb    = 0; 
Bool_t doVertex       = 1;
Bool_t doSPD          = 1;   // needs RP   
Bool_t doTPC          = 1;
Bool_t doHLT          = 1;
Bool_t doSDD          = 1;   // needs RP
Bool_t doPileup       = 1;
Bool_t doSSDdEdx      = 1;

Bool_t doTRD          = 1;
Bool_t doITS          = 1;
Bool_t doITSsaTracks  = 1; 
Bool_t doITSalign     = 1;  
Bool_t doESDTracks    = 1;
Bool_t doCALO         = 1;
Bool_t doMUONTrig     = 1;
Bool_t doMUONEff      = 1;
Bool_t doImpParRes    = 1;
Bool_t doMUON         = 1;
Bool_t doTOF          = 1;
Bool_t doHMPID        = 1;
Bool_t doT0           = 1;
Bool_t doZDC          = 1;
Bool_t doPIDResponse  = 1;
Bool_t doPIDqa        = 1; //new
Bool_t doFMD          = 1; // new
Bool_t doPHOS         = 1; // new
Bool_t doPHOSTrig     = 1; // new
Bool_t doEMCAL        = 1;
Bool_t doFBFqa        = 1; // new - not ported yet to revision
Bool_t doAD           = 1; // new AD detector

Bool_t doTaskFilteredTree        = 0;      // high pt filter task

               // Debug level
Int_t       debug_level        = 1;        // Debugging
Int_t       run_number = 0;


void main_QAtrain_duo(const char *suffix="", Int_t run = 0, 
             const char *xmlfile   = "wn.xml",
             Int_t  stage          = 0, /*0 = QA train, 1...n - merging stage*/
             const char *cdb     = "raw://")
//             const char *cdb     = "local://$ALICE_ROOT/OCDB")
{
  run_number = run;
  TString ss(suffix);
  ss.ToLower();
  Bool_t ibarrel = (ss.Contains("barrel"))?kTRUE:kFALSE;

  ProcessEnvironmentVars();
  
  kTriggerMask = kTriggerInt;
  
  printf("------ Run QAtrain_duo.C for collision type %s ------\n",CollisionSystem[iCollisionType]);

  PrintSettings();

  TString cdbString(cdb);
  if (cdbString.Contains("raw://") && !gSystem->Getenv("OCDB_PATH"))
  {
    TGrid::Connect("alien://");
    if (!gGrid || !gGrid->IsConnected()) {
      ::Error("QAtrain", "No grid connection");
      return;
    }  
  }
  // Set temporary merging directory to current one
  gSystem->Setenv("TMPDIR", gSystem->pwd());
  // Set temporary compilation directory to current one
  gSystem->SetBuildDir(gSystem->pwd(), kTRUE);
  // Load libraries
//  LoadLibraries();
  gSystem->AddIncludePath("-I$ALICE_PHYSICS/include -I$ALICE_ROOT/include");
//   gSystem->Load("libOADB");
  printf("Include path: %s\n", gSystem->GetIncludePath());
  // Create manager
  AliAnalysisManager *mgr  = new AliAnalysisManager("PilotAnalysis", "Production train");
  mgr->SetRunFromPath(run_number);
  mgr->SetCacheSize(0);
  // Input handler
  AliESDInputHandlerRP *esdHandler = new AliESDInputHandlerRP();
  esdHandler->SetReadFriends(kTRUE);
  esdHandler->SetActiveBranches("ESDfriend");
 
  mgr->SetInputEventHandler(esdHandler);
  mgr->SetDebugLevel(debug_level);
  mgr->SetFileInfoLog("fileinfo.log"); 
  
  // AnalysisTasks
  AddAnalysisTasks(suffix, cdb);
  if (stage>0) {
    QAmerge(suffix, xmlfile, stage);
    return;
  }   
  // Input chain
  TChain *chain = new TChain("esdTree");
  chain->Add("AliESDs.root");
  TStopwatch timer;
  timer.Start();
  if (mgr->InitAnalysis()) {                                                                                                              
    mgr->PrintStatus(); 
    mgr->SetSkipTerminate(kTRUE);
    //mgr->SetNSysInfo(100);
    //AliSysInfo::SetVerbose(kTRUE);
    mgr->StartAnalysis("local", chain);
  }
  timer.Print();
}

void AddAnalysisTasks(const char *suffix, const char *cdb_location)
{
  TString ss(suffix);
  ss.ToLower();
  Bool_t ibarrel = (ss.Contains("barrel"))?kTRUE:kFALSE;
  Bool_t iouter = (ss.Contains("outer"))?kTRUE:kFALSE;
  Bool_t iall = (!ibarrel)&(!iouter);
  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
  mgr->SetCommonFileName(Form("QAresults%s.root",suffix));

  // Fixes/protections for specific AliRoot Versions
  // ESD track QA can be run for AliPhysics >= v5-09-14
  // PHOS QA tasks need different arguments starting for AliPhysics >= v5-09-24
  // we expect by default to be using an AliPhysics recent than the two above
  Bool_t disableESDtrackQA=kFALSE;

  if(!gSystem->Getenv("ALIEN_JDL_PACKAGES"))
    if(gSystem->Getenv("ALIEN_PACKAGES"))
    {
      gSystem->Setenv("ALIEN_JDL_PACKAGES", gSystem->Getenv("ALIEN_PACKAGES"));
      printf("Using ALIEN_PACKAGES instead of ALIEN_JDL_PACKAGES\n");
    }

  if(gSystem->Getenv("ALIEN_JDL_PACKAGES")){
    TString packg=gSystem->Getenv("ALIEN_JDL_PACKAGES");
    Int_t pos1=packg.Index("AliPhysics");
    TString subs=packg(pos1,packg.Length()-pos1);
    Int_t pos2=subs.Index("VO_ALICE");
    if(pos2<=0) pos2=subs.Length();
    TString aliph=subs(0,pos2);
    Int_t ver,n1,n2;
    Char_t str2[20];
    sscanf(aliph.Data(),"AliPhysics::v%d-%d-%02d%s",&ver,&n1,&n2,str2);
    printf("XXXXXXXXX=> %s\n",aliph.Data());
    if(ver<5){
      disableESDtrackQA=kTRUE;
    }else if(ver==5){
      if(n1<9 || (n1==9 && n2<14)) disableESDtrackQA=kTRUE;  // < v5-09-14
    }
  }

  // Statistics task
  if (doStatistics) mgr->AddStatisticsTask(kTriggerMask);

//   Clean Geometry: Ruben
//  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/CalibMacros/commonMacros/CleanGeom.C++");
//  CleanGeom* clgmTask = new CleanGeom("cleanGeom");
//  mgr->AddTask(clgmTask);
//  AliAnalysisDataContainer *dummyInp = mgr->GetCommonInputContainer();
//  if (dummyInp) mgr->ConnectInput(clgmTask,0,dummyInp);  

  //
  // CDB connection
  // 
  if (doCDBconnect) {
    AliTaskCDBconnect *taskCDB = AddTaskCDBconnect(cdb_location, run_number);    
//    AliTaskCDBconnect *taskCDB = AddTaskCDBconnect();
    if (!taskCDB) return;
//    AliCDBManager *cdb = AliCDBManager::Instance();
//    cdb->SetDefaultStorage(cdb_location);
//    taskCDB->SetRunNumber(run_number);
  }    
  
  //
  // Event Statistics (Jan Fiete)
  // 
  if (doEventStat) {
      AliPhysicsSelectionTask* physSelTask = AddTaskPhysicsSelection(kFALSE /*MC*/);
      AliAnalysisDataContainer *cstatsout = (AliAnalysisDataContainer*)mgr->GetOutputs()->FindObject("cstatsout");
      cstatsout->SetFileName(Form("EventStat_temp%s.root", suffix));
  }
  
   //
  // PIDResponse(JENS)
  // 
  if (doPIDResponse && (ibarrel || iall)) {
    AliAnalysisTaskPIDResponse *PIDResponse = AddTaskPIDResponse();
 //   PIDResponse->SetUserDataRecoPass(1);
    PIDResponse->SelectCollisionCandidates(kTriggerMask);
  }  

  //
  // Centrality (A. Toia)
  // 
  if (doCentrality) 
  {
      AliMultSelectionTask *taskMult = AddTaskMultSelection();
      taskMult->SetAddInfo(kFALSE);
  }

  // Vertexing (A. Dainese)
  // 
  if (doVertex && (ibarrel || iall)) {
    #if ROOT_VERSION_CODE >= ROOT_VERSION(6,0,0)
    AliAnalysisTaskVertexESD* taskvertexesd =  AddTaskVertexESD(kFALSE, static_cast<AliVEvent::EOfflineTriggerTypes>(kTriggerMask)); //ROOT6
    #else
    AliAnalysisTaskVertexESD* taskvertexesd =  AddTaskVertexESD(kFALSE, kTriggerMask); // ROOT5
    #endif
    taskvertexesd->SelectCollisionCandidates(kTriggerMask);
  }  

  // TPC QA (E. Sicking)
  // 
  if (doQAsym && (ibarrel || iall)) {
  // offline trigger in AddTask
    AliAnalysisTaskSE * taskqasim = AddTaskQAsym(0, kTriggerMask, kTriggerHM, kTriggerEMC, kTriggerMuonBarrel);
  }  
  //
  // VZERO QA  (C. Cheshkov)
  // 
  if (doVZERO && (ibarrel || iall)) 
  {
    AliAnalysisTaskSE * taskv0qa = AddTaskVZEROQA(0);
    //taskv0qa->SelectCollisionCandidates();
    AliAnaVZEROQA *taskv0qatrig = AddTaskVZEROQATrig(0);
    //taskv0qatrig->SelectCollisionCandidates(AliVEvent::kINT8);
    taskv0qatrig->SelectCollisionCandidates(AliVEvent::kINT7);
  }

  if (doVZEROPbPb /*&& iCollisionType==0*/) {
    AliAnaVZEROPbPb* taskV0PbPb = AddTaskVZEROPbPb(run_number);
//    taskV0PbPb->SetClassesNames("CTRUE-,C0HWU-,CPBI2WU_B1-,CPBI2_B1-,CPBI1WU-,CPBI1-,CVHNWU-,CVHN-,CVLNWU-,CVLN-");
//    taskV0PbPb->SetClassesNames("CTRUE-,C0HWU-,CPBI2WU,CPBI2,CPBI1WU-,CPBI1-,CVHNWU,CVHN,CVLNWU,CVLN");
//    taskV0PbPb->SetClassesNames("CTRUE-,C0HWU-,CPBI2WU-,CPBI2-,CPBI2WU_B1-,CPBI2_B1-,CPBI1WU-,CPBI1-,CVHNWU-,CVHN-,CVHN_R2-,CVHNWU_R2-,CVLNWU-,CVLN-,CVLN_B2-,CVLNWU_B2-");
//    taskV0PbPb->SetClassesNames("CTRUE-,C0HWU-,CPBI2WU-,CPBI2-,CPBI2WU_B1-,CPBI2_B1-,CPBI1WU-,CPBI1-,CVHNWU-,CVHN-,CVHN_R2-,CVHNWU_R2-,CVLNWU-,CVLN-,CVLN_R1-,CVLN_B2-,CVLNWU_R1-,CVLNWU_B2-");
//    taskV0PbPb->SetClassesNames("CTRUE-,C0HWU-,CPBI2WU-,CPBI2-,CPBI2WU_B1-,CPBI2_B1-,CPBI1WU-,CPBI1-,CVHNWU-,CVHN-,CVHN_R2-,CVHNWU_R2-,CVLNWU-,CVLN-,CVLN_R1-,CVLN_B2-,CVLNWU_R1-,CVLNWU_B2-,CSEMI_R1-,CSEMIWU_R1-,CCENT_R2-,CCENTWU_R2-");
  }
  //
  // TPC (Jacek Otwinowski & Michael Knichel)
  //
  //
  // Optionally MC information can be used by setting the 1st argument to true
  // Optionally friends information can be switched off by setting the 2st argument 
  // to false
  // Optionally highMult axis can be used by setting the 3st argument to true (for PbPb)
  if (doTPC && (ibarrel || iall)) {
    AliPerformanceTask *tpcQA = 0;
    if (iCollisionType==kPbPb  || iCollisionType == kXeXe) {
       // High multiplicity Pb-Pb
       tpcQA = AddTaskPerformanceTPCdEdxQA(kFALSE, kTRUE, kTRUE);
    } else {
      // Low multiplicity (pp)
       tpcQA = AddTaskPerformanceTPCdEdxQA(kFALSE, kTRUE, kFALSE);
    }
    tpcQA->SelectCollisionCandidates(kTriggerMask);
  }

  // HLT (Alberica Toia)
  if (doHLT && (ibarrel || iall)) {
    AliPerformanceTask *hltQA = AddTaskPerformanceTPCdEdxQA(kFALSE, kTRUE, kFALSE,0,kTRUE);
    hltQA->SelectCollisionCandidates(kTriggerMask);
  }
  
  //
  // SPD (A. Mastroserio)
  // 
  if (doSPD && (ibarrel || iall)) {
    AliAnalysisTaskSPD* taskspdqa = AddTaskSPDQA();
    // Request from Annalisa
    if (iCollisionType==kPbPb  || iCollisionType == kXeXe) taskspdqa->SetHeavyIonMode();
    taskspdqa->SelectCollisionCandidates(kTriggerMask);
    taskspdqa->SetOCDBInfo(run_number, cdb_location);
  }
  
  //
  // SDD (F. Prino)
  // 
  if (doSDD && (ibarrel || iall)) {
    AliAnalysisTaskSE* tasksdd = AddSDDPoints();
    tasksdd->SelectCollisionCandidates(kTriggerMask);
  }
  //
  // SSD dEdx (Marek Chojnacki)
  // 
  if (doSSDdEdx && (ibarrel || iall)) {
    AliAnalysisTaskSE* taskssddedx = AddTaskdEdxSSDQA();
    taskssddedx->SelectCollisionCandidates(kTriggerMask);
  }

  if (doPileup && (ibarrel || iall))
  { 
    AliAnalysisTaskCheckPileup* taskpileup = AddTaskPileupChecks("QA", 3,0.8,kFALSE); 
  }   
  
  //
  // ITS
  // 
  if (doITS && (ibarrel || iall)) {
  // hardcoded non-zero trigger mask
      AliAnalysisTaskITSTrackingCheck *itsQA = 0;
      AliAnalysisTaskITSTrackingCheck *itsQACent0010 = 0;
      AliAnalysisTaskITSTrackingCheck *itsQACent3050 = 0;
      AliAnalysisTaskITSTrackingCheck *itsQACent6080 = 0;
      itsQA = AddTaskPerformanceITS(kFALSE);
      if(iCollisionType==kPbPb) {
        itsQACent0010 = AddTaskPerformanceITS(kFALSE,kFALSE,kFALSE,3500,10000);
        itsQACent3050 = AddTaskPerformanceITS(kFALSE,kFALSE,kFALSE,590,1570);
        itsQACent6080 = AddTaskPerformanceITS(kFALSE,kFALSE,kFALSE,70,310);
      }else if(iCollisionType==kXeXe) {
        itsQACent0010 = AddTaskPerformanceITS(kFALSE,kFALSE,kFALSE,2000,6000);
        itsQACent3050 = AddTaskPerformanceITS(kFALSE,kFALSE,kFALSE,350,1000);
        itsQACent6080 = AddTaskPerformanceITS(kFALSE,kFALSE,kFALSE,40,200);
      }

  }
  //
  // ITS saTracks, align (F.Prino)
  // 
  if (doITSsaTracks && (ibarrel || iall)) {
  // offline trigger in AddTask
     AliAnalysisTaskITSsaTracks *itssaTracks = AddTaskITSsaTracks(kFALSE,kFALSE);
     itssaTracks->SelectCollisionCandidates(kTriggerMask);
  }   
  if (doITSalign && (ibarrel || iall)) {
  // no offline trigger selection
     AliAnalysisTaskITSAlignQA *itsAlign = AddTaskITSAlign(0,2012);
  }   
  //
  // Global tracks + V0s QA
  //

  if(doESDTracks && (ibarrel || iall)) {
    if(disableESDtrackQA) printf("AliAnalysisTaskCheckESDTracks switched off due to AliPhysics<v5-09-14\n");
    else{
      AliAnalysisTaskCheckESDTracks* taskestr=AddTaskCheckESDTracks("QA",kTRUE,kFALSE,kFALSE);
      taskestr->SetPtBinning(160,0.,40.);
      taskestr->SelectCollisionCandidates(kTriggerMask);
    }
  }
  //
  // TRD (Alex Bercuci, M. Fasel) 
  // 
  if(doTRD && (ibarrel || iall)) {
  // no offline trigger selection
       // steer individual TRD tasks
      Bool_t 
      doCheckESD(kTRUE),  // AliTRDcheckESD
      doCheckDET(kTRUE),  // AliTRDcheckDET
      doEffic(kTRUE),     // AliTRDefficiency
      doResolution(kTRUE),// AliTRDresolution
      doCheckPID(kFALSE),  // AliTRDcheckPID
      doV0Monitor(kFALSE);// AliTRDv0Monitor
      AliTRDpwgppHelper::AddTrainPerformanceTRD(AliTRDpwgppHelper::Translate(doCheckESD, doCheckDET, doEffic, doResolution, doCheckPID, doV0Monitor));
  
  //    gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/macros/AddTrainPerformanceTRD.C");
      // steer individual TRD tasks
  //   Bool_t 
  //    doCheckESD(kTRUE),  // AliTRDcheckESD
  //    doCheckDET(kTRUE),  // AliTRDcheckDET
  //    doEffic(kTRUE),     // AliTRDefficiency
  //   doResolution(kTRUE),// AliTRDresolution
  // doCheckPID(kTRUE),  // AliTRDcheckPID
  //   doV0Monitor(kFALSE);// AliTRDv0Monitor
  //    AddTrainPerformanceTRD(Translate(doCheckESD, doCheckDET, doEffic, doResolution, doCheckPID, doV0Monitor));
  }

  //
  // ZDC (Chiara Oppedisano) 
  // 
  if(doZDC && (ibarrel || iall)) {
  // hardcoded kMB trigger mask
     AliAnalysisTaskSE *taskZDC = AddTaskZDCQA();
     taskZDC->SelectCollisionCandidates(kTriggerMask);
  }   
  //
  // Calorimetry (Gustavo Conesa)
  //

  if(doCALO) 
  {
  // - In case of MC analysis, it skips the triggered events wagon (given that
  //the normal wagon is always suffixed as "default")
  //- No need to pass the type of data
  //- No need to specify the year. In principle the year is only needed when
  //setting the size of some histograms (change in number of SM along the years),
  //I do not know how to access automatically the run number when the histograms
  // are created in the UserCreateOutput. By default I set them to the maximum
  //expected size, but I still let the possibility to check the year.

  //So the way to initialize the task in trunk is now

  //AddTaskCalorimeterQA(Bool_t kSimulation = kFALSE,
                                   //  const char *suffix="default",
                                   // TString outputFile = "", 
                                   //  Int_t year = 2012, 
                                   //  Bool_t kPrintSettings = kFALSE)

  //So, in principle only the first 2 need to be specified.

  
    AliAnalysisTaskCaloTrackCorrelation *taskCaloQA = AddTaskCalorimeterQA("default");
    taskCaloQA->SetDebugLevel(0);
    // offline mask set in AddTask to kMB
    //taskCaloQA->SelectCollisionCandidates(0);
    taskCaloQA->SelectCollisionCandidates(kTriggerMask);
    // Add a new calo task with EMC1 trigger only
    taskCaloQA = AddTaskCalorimeterQA("trigEMC");        // for 2011 data, not 2010
    taskCaloQA->SelectCollisionCandidates(kTriggerEMC);  // for 2011 data, not 2010
    taskCaloQA->SetDebugLevel(0);
  }

  //
  // Muon Trigger
  //
  
  if(doMUONTrig) {
  // no offline trigger selection
      AliAnalysisTaskTrigChEff *taskMuonTrig = AddTaskMTRchamberEfficiency();
//      taskMuonTrig->GetMuonTrackCuts()->SetCustomParamFromRun(160000,2);       
  }

  if(doMUONEff)
  {
    AliAnalysisTaskMuonTrackingEff* muonEfficiency = AddTaskMUONTrackingEfficiency(kTRUE,kFALSE,"");
    muonEfficiency->SelectCollisionCandidates(AliVEvent::kMuonUnlikeLowPt7);
  }
  
  //
  // Impact parameter resolution (xianbao.yuan@pd.infn.it, andrea.dainese@pd.infn.it)
  // 
  if (doImpParRes && (ibarrel || iall)) {
    AliAnalysisTaskSE* taskimpparres=0;
    if(iCollisionType==kpp || iCollisionType==kpPb || iCollisionType==kPbp) {
       taskimpparres= AddTaskImpParRes();
    } else {
       taskimpparres= AddTaskImpParRes(kFALSE,-1,kFALSE,kFALSE);
    }
    taskimpparres->SelectCollisionCandidates(kTriggerMask);

    AliAnalysisTaskSEImpParResSparse* taskimpparressp=0;
    if(iCollisionType==kpp || iCollisionType==kpPb || iCollisionType==kPbp) {
      taskimpparressp = AddTaskImpParResSparse(kFALSE,-1,kTRUE,kTRUE,0,1000000,1,"withSparse");
      taskimpparressp->SetUsePtWeights(1,1.);
    } else {
      taskimpparressp = AddTaskImpParResSparse(kFALSE,-1,kFALSE,kFALSE,0,1000000,1,"withSparse");
      taskimpparressp->SetUsePtWeights(3,1.);
    }
    taskimpparressp->SelectCollisionCandidates(kTriggerMask);
  }  

  //
  // MUON QA (Philippe Pillot)
  // 
  if (doMUON) {
  // trigger analysis internal
    AliAnalysisTaskMuonQA* taskmuonqa= 0;
    if (kTriggerMask) 
      taskmuonqa = AddTaskMuonQA(kTRUE);
    else 
      taskmuonqa = AddTaskMuonQA(kFALSE); 

    // taskmuonqa->GetTrackCuts()->SetCustomParamFromRun(160000,2);
  }
  //
  // TOF (Francesca Bellini)
  // 
//  if (doTOF && (ibarrel || iall)) {
//    AliAnalysisTaskTOFqa *tofQA = AddTaskTOFQA(kFALSE);
//    tofQA->SelectCollisionCandidates(kTriggerMask);
  if (doTOF && (ibarrel || iall)) {
    AliAnalysisTaskTOFqaID *tofQA = AddTaskTOFqaID(kFALSE, kTriggerMask, 0, kFALSE, "", kFALSE, 0);
 //   tofQA->SelectCollisionCandidates(kTriggerMask);

  } 
  //
  // PIDqa(JENS)
  // 
  if (doPIDqa && (ibarrel || iall)) {
    AliAnalysisTaskPIDqa *PIDQA = AddTaskPIDqa();
    PIDQA->SelectCollisionCandidates(kTriggerMask);
  }  
 
  //
  // HMPID QA (Giacomo Volpe)
  // 
  if (doHMPID && (ibarrel || iall)) {
    AliAnalysisTaskSE* taskhmpidqa= AddTaskHmpidQA(kFALSE);
      // offline mask set in AddTask to kMB
    taskhmpidqa->SelectCollisionCandidates(kTriggerMask);
  }      
  
  
  // T0 QA (Alla Mayevskaya)
  if (doT0 && (ibarrel || iall)) {
  // no offline trigger selection
    AliT0AnalysisTaskQA* taskt0qa= AddTaskT0QA();
    taskt0qa->SelectCollisionCandidates(kTriggerMask);
  }      
  // FMD QA (Christian Holm Christiansen)
  if (doFMD && (ibarrel || iall)) {
    // Parameters: usemc, usecentrality
    AliForwardQATask *forwardQA = AddTaskForwardQA(kFALSE, kFALSE);
    forwardQA->SetDebug(0);
    // No offline trigger config. needed (see #84077)
  }
   //     
  // PHOS QA (Boris Polishchuk)
  // 
  if (doPHOS) {
    AliAnalysisTaskCaloCellsQA *taskPHOSCellQA1 = 0x0;
    taskPHOSCellQA1 = AddTaskCaloCellsQA(5, 1, CALOCELLS_NULL,"PHOSCellsQA_AnyInt");
    taskPHOSCellQA1->SelectCollisionCandidates(kTriggerMask);
    taskPHOSCellQA1->GetCaloCellsQA()->SetClusterEnergyCuts(0.3,0.3,1.0);
    AliAnalysisTaskCaloCellsQA *taskPHOSCellQA2 = 0x0;
    taskPHOSCellQA2 = AddTaskCaloCellsQA(5, 1, CALOCELLS_NULL,"PHOSCellsQA_PHI7");
    taskPHOSCellQA2->SelectCollisionCandidates(AliVEvent::kPHI7);
    taskPHOSCellQA2->GetCaloCellsQA()->SetClusterEnergyCuts(0.3,0.3,1.0);
    // Pi0 QA fo PbPb
    if (iCollisionType == kPbPb || iCollisionType == kXeXe) {
      AliAnalysisTaskPHOSPbPbQA* phosPbPb = AddTaskPHOSPbPbQA(0);
    }
  }    
  //     
  // PHOS Trigger (Boris Polishchuk)
  //
  if (doPHOSTrig)
  {
    AliAnalysisTaskPHOSTriggerQA *taskPHOSTrig1 = 0x0;
    AliAnalysisTaskPHOSTriggerQA* taskPHOSTrig2 = 0x0;
    AliAnalysisTaskPHOSTriggerQA* taskPHOSTrig3 = 0x0;
    AliAnalysisTaskPHOSTriggerQA* taskPHOSTrig4 = 0x0;
    taskPHOSTrig1 = AddTaskPHOSTriggerQA(CALOCELLS_NULL,"PHOSTriggerQAResultsL0");
    taskPHOSTrig2 = AddTaskPHOSTriggerQA(CALOCELLS_NULL,"PHOSTriggerQAResultsL1High");
    taskPHOSTrig3 = AddTaskPHOSTriggerQA(CALOCELLS_NULL,"PHOSTriggerQAResultsL1Medium");
    taskPHOSTrig4 = AddTaskPHOSTriggerQA(CALOCELLS_NULL,"PHOSTriggerQAResultsL1Low");
    taskPHOSTrig2->SelectL1Threshold(0);
    taskPHOSTrig3->SelectL1Threshold(1);
    taskPHOSTrig4->SelectL1Threshold(2);
  }
  //
  // EMCAL QA (Gustavo Conesa)
  // 
  if (doEMCAL) {
    //AliAnalysisTaskEMCALTriggerQA *emctrig = AddTaskEMCALTriggerQA();
//     emctrig->GetRecoUtils()->SwitchOffBadChannelsRemoval();
    AliEmcalTriggerMakerTask *emctrigmaker = AddTaskEmcalTriggerMakerNew("EmcalTriggers");
    AliEmcalTriggerQATask *emctrig = AddTaskEmcalTriggerQA_QAtrain(run_number);
  }
  //     
  // FLOW and BF QA (C.Perez && A.Rodriguez)
  // 
  if (doFBFqa && (ibarrel || iall)) {
    AliGlobalFBFqa *qaFBFMB =  AddTaskFBFqa("qaFBFmb",kFALSE);
    qaFBFMB->SelectCollisionCandidates(AliVEvent::kMB);
    AliGlobalFBFqa *qaFBFSC =  AddTaskFBFqa("qaFBFsc",kFALSE);
    qaFBFSC->SelectCollisionCandidates(AliVEvent::kSemiCentral);
    AliGlobalFBFqa *qaFBFCE =  AddTaskFBFqa("qaFBFce",kFALSE);
    qaFBFCE->SelectCollisionCandidates(AliVEvent::kCentral);
  }
  
//Jacek
   if (gSystem->Getenv("QA_TaskFilteredTree")) doTaskFilteredTree=1;
   if (doTaskFilteredTree) {
      AddTaskFilteredTree("FilterEvents_Trees.root");
   }   
// AD detector QA
   if (doAD && (ibarrel || iall)) {
      AliAnalysisTaskADQA *taskADQA = AddTaskADQA();
   }   
}

void QAmerge(const char *suffix, const char *dir, Int_t stage)
{
// Merging method
  TStopwatch timer;
  timer.Start();
  TString outputDir = dir;
  TString outputFiles = Form("QAresults%s.root,EventStat_temp%s.root,RecoQAresults%s.root,FilterEvents_trees%s.root",suffix,suffix,suffix,suffix);
  //TString mergeExcludes = "EventStat_temp.root"; // created by AODtrain? crash in QAmerging if AOD missing?
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
    merged = AliAnalysisAlien::MergeOutput(outputFile, outputDir, 10, stage);
    if (!merged && !outputFile.Contains("RecoQAresults") && !outputFile.Contains("FilterEvents_trees")) {
       printf("ERROR: Cannot merge %s\n", outputFile.Data());
       return;
    }
  }
  // read the analysis manager from file
  TString infolog = "fileinfo.log";
  AliAnalysisAlien::MergeInfo(infolog, dir); 
  if (!outputDir.Contains("Stage")) {
    ofstream out;
    out.open("outputs_valid", ios::out);
    out.close();    
    return;
  }
  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
  mgr->SetRunFromPath(mgr->GetRunFromAlienPath(dir));
  mgr->SetSkipTerminate(kFALSE);
  if (!mgr->InitAnalysis()) return;
  mgr->PrintStatus();
  AliLog::SetGlobalLogLevel(AliLog::kError);
  TTree *tree = NULL;
  gROOT->cd();
  mgr->StartAnalysis("gridterminate", tree);
  if (strlen(suffix)) {
    if(!strstr(suffix,"AOD") && !strstr(suffix,"aod")){ // when merging the AOD QA output we should not rename trending.root
      if (gSystem->Exec(Form("mv trending.root trending%s.root", suffix)))
        ::Error("QAmerge", "File trending.root was not produced");
    }
    if (gSystem->Exec(Form("mv event_stat.root event_stat%s.root", suffix)))
      ::Error("QAmerge", "File event_stat.root was not produced");
  }   
  ofstream out;
  out.open("outputs_valid", ios::out);
  out.close();
  timer.Print();
}

void ProcessEnvironmentVars()
{
  //
  // Collision system configuration
  //
  if(gSystem->Getenv("ALIEN_JDL_LPMINTERACTIONTYPE"))
  {
    for (Int_t icoll = 0; icoll < kNSystem; icoll++)
      if (strcmp(gSystem->Getenv("ALIEN_JDL_LPMINTERACTIONTYPE"), CollisionSystem[icoll]) == 0) 
      {
        iCollisionType = icoll;
        break;
      }
      if(iCollisionType == kPbPb || iCollisionType == kXeXe)
      {
        doCentrality =kTRUE;
        doVZEROPbPb =kTRUE;
      }
  }
  else
    if(!localRunning)
    {
      printf(">>>>> Unknown collision system configuration ALIEN_JDL_LPMINTERACTIONTYPE \n");
      abort();
    }

  //
  // Run number
  //
  if (gSystem->Getenv("ALIEN_JDL_LPMRUNNUMBER"))
    run_number = atoi(gSystem->Getenv("ALIEN_JDL_LPMRUNNUMBER"));
  else
    if(!localRunning)
    {
      printf(">>>>> Unknown run number configuration ALIEN_JDL_LPMRUNNUMBER \n");
      abort();
    }
  if (run_number <= 0)
    printf(">>>>> Invalid run number: %d \n", run_number);

  //
  // TriggerInt configuration
  //
  if(gSystem->Getenv("ALIEN_JDL_LPMANCHORYEAR"))
  {
    Int_t year = atoi(gSystem->Getenv("ALIEN_JDL_LPMANCHORYEAR"));
    if(year <= 2015) 
      kTriggerInt = AliVEvent::kAnyINT;
    else // 2016
      kTriggerInt=  AliVEvent::kINT7 | AliVEvent::kINT8;
  }
  else
    if(!localRunning)
    {
      printf(">>>>> Unknown year for configuration ALIEN_JDL_LPMANCHORYEAR \n");
      abort();
    }

  int var=0;
  TString envS;
  //
  if ( !(envS=gSystem->Getenv("CDBconnect")).IsNull() && CheckEnvS("CDBconnect",envS) ) {
    doCDBconnect = atoi(envS.Data());
    printf("Set doCDBconnect=%d according to environment variables\n",doCDBconnect);
  }
  //
  if ( !(envS=gSystem->Getenv("Statistics")).IsNull() && CheckEnvS("Statistics",envS) ) {
    doStatistics = atoi(envS.Data());
    printf("Set doStatistics=%d according to environment variables\n",doStatistics);
  }
  //
  if ( !(envS=gSystem->Getenv("EventStat")).IsNull() && CheckEnvS("EventStat",envS) ) {
    doEventStat = atoi(envS.Data());
    printf("Set doEventStat=%d according to environment variables\n",doEventStat);
  }
  //
  if ( !(envS=gSystem->Getenv("Centrality")).IsNull() && CheckEnvS("Centrality",envS) ) {
    doCentrality = atoi(envS.Data());
    printf("Set doCentrality=%d according to environment variables\n",doCentrality);
  }
  //
  if ( !(envS=gSystem->Getenv("QAsym")).IsNull() && CheckEnvS("QAsym",envS) ) {
    doQAsym = atoi(envS.Data());
    printf("Set doQAsym=%d according to environment variables\n",doQAsym);
  }
  //
  if ( !(envS=gSystem->Getenv("VZERO")).IsNull() && CheckEnvS("VZERO",envS) ) {
    doVZERO = atoi(envS.Data());
    printf("Set doVZERO=%d according to environment variables\n",doVZERO);
  }
  //
  if ( !(envS=gSystem->Getenv("VZEROPbPb")).IsNull() && CheckEnvS("VZEROPbPb",envS) ) {
    doVZEROPbPb = atoi(envS.Data());
    printf("Set doVZEROPbPb=%d according to environment variables\n",doVZEROPbPb);
  }
  //
  if ( !(envS=gSystem->Getenv("Vertex")).IsNull() && CheckEnvS("Vertex",envS) ) {
    doVertex = atoi(envS.Data());
    printf("Set doVertex=%d according to environment variables\n",doVertex);
  }
  //
  if ( !(envS=gSystem->Getenv("SPD")).IsNull() && CheckEnvS("SPD",envS) ) {
    doSPD = atoi(envS.Data());
    printf("Set doSPD=%d according to environment variables\n",doSPD);
  }
  //
  if ( !(envS=gSystem->Getenv("TPC")).IsNull() && CheckEnvS("TPC",envS) ) {
    doTPC = atoi(envS.Data());
    printf("Set doTPC=%d according to environment variables\n",doTPC);
  }
  //
  if ( !(envS=gSystem->Getenv("HLT")).IsNull() && CheckEnvS("HLT",envS) ) {
    doHLT = atoi(envS.Data());
    printf("Set doHLT=%d according to environment variables\n",doHLT);
  }
  //
  if ( !(envS=gSystem->Getenv("SDD")).IsNull() && CheckEnvS("SDD",envS) ) {
    doSDD = atoi(envS.Data());
    printf("Set doSDD=%d according to environment variables\n",doSDD);
  }
  //
  if ( !(envS=gSystem->Getenv("SSDdEdx")).IsNull() && CheckEnvS("SSDdEdx",envS) ) {
    doSSDdEdx = atoi(envS.Data());
    printf("Set doSSDdEdx=%d according to environment variables\n",doSSDdEdx);
  }
  //
  if ( !(envS=gSystem->Getenv("Pileup")).IsNull() && CheckEnvS("Pileup",envS) ) {
    doPileup = atoi(envS.Data());
    printf("Set doPileup=%d according to environment variables\n",doPileup);
  }
  //
  if ( !(envS=gSystem->Getenv("TRD")).IsNull() && CheckEnvS("TRD",envS) ) {
    doTRD = atoi(envS.Data());
    printf("Set doTRD=%d according to environment variables\n",doTRD);
  }
  //
  if ( !(envS=gSystem->Getenv("ITS")).IsNull() && CheckEnvS("ITS",envS) ) {
    doITS = atoi(envS.Data());
    printf("Set doITS=%d according to environment variables\n",doITS);
  }
  //
  if ( !(envS=gSystem->Getenv("ITSsaTracks")).IsNull() && CheckEnvS("ITSsaTracks",envS) ) {
    doITSsaTracks = atoi(envS.Data());
    printf("Set doITSsaTracks=%d according to environment variables\n",doITSsaTracks);
  }
  //
  if ( !(envS=gSystem->Getenv("ITSalign")).IsNull() && CheckEnvS("ITSalign",envS) ) {
    doITSalign = atoi(envS.Data());
    printf("Set doITSalign=%d according to environment variables\n",doITSalign);
  }
  // 
  if ( !(envS=gSystem->Getenv("ESDTracks")).IsNull() && CheckEnvS("ESDTracks",envS) ) {
    doESDTracks = atoi(envS.Data());
    printf("Set doESDtracks=%d according to environment variables\n",doESDTracks);
  }
  //
  if ( !(envS=gSystem->Getenv("CALO")).IsNull() && CheckEnvS("CALO",envS) ) {
    doCALO = atoi(envS.Data());
    printf("Set doCALO=%d according to environment variables\n",doCALO);
  }
  //
  if ( !(envS=gSystem->Getenv("MUONTrig")).IsNull() && CheckEnvS("MUONTrig",envS) ) {
    doMUONTrig = atoi(envS.Data());
    printf("Set doMUONTrig=%d according to environment variables\n",doMUONTrig);
  }
  //
  if ( !(envS=gSystem->Getenv("MUONEff")).IsNull() && CheckEnvS("MUONEff",envS) ) {
    doMUONEff = atoi(envS.Data());
    printf("Set doMUONEff=%d according to environment variables\n",doMUONEff);
  }
  //
  if ( !(envS=gSystem->Getenv("ImpParRes")).IsNull() && CheckEnvS("ImpParRes",envS) ) {
    doImpParRes = atoi(envS.Data());
    printf("Set doImpParRes=%d according to environment variables\n",doImpParRes);
  }
  //
  if ( !(envS=gSystem->Getenv("MUON")).IsNull() && CheckEnvS("MUON",envS) ) {
    doMUON = atoi(envS.Data());
    printf("Set doMUON=%d according to environment variables\n",doMUON);
  }
  //
  if ( !(envS=gSystem->Getenv("TOF")).IsNull() && CheckEnvS("TOF",envS) ) {
    doTOF = atoi(envS.Data());
    printf("Set doTOF=%d according to environment variables\n",doTOF);
  }
  //
  if ( !(envS=gSystem->Getenv("HMPID")).IsNull() && CheckEnvS("HMPID",envS) ) {
    doHMPID = atoi(envS.Data());
    printf("Set doHMPID=%d according to environment variables\n",doHMPID);
  }
  //
  if ( !(envS=gSystem->Getenv("T0")).IsNull() && CheckEnvS("T0",envS) ) {
    doT0 = atoi(envS.Data());
    printf("Set doT0=%d according to environment variables\n",doT0);
  }
  //
  if ( !(envS=gSystem->Getenv("ZDC")).IsNull() && CheckEnvS("ZDC",envS) ) {
    doZDC = atoi(envS.Data());
    printf("Set doZDC=%d according to environment variables\n",doZDC);
  }
  //
  if ( !(envS=gSystem->Getenv("PIDResponse")).IsNull() && CheckEnvS("PIDResponse",envS) ) {
    doPIDResponse = atoi(envS.Data());
    printf("Set doPIDResponse=%d according to environment variables\n",doPIDResponse);
  }
  //
  if ( !(envS=gSystem->Getenv("PIDqa")).IsNull() && CheckEnvS("PIDqa",envS) ) {
    doPIDqa = atoi(envS.Data());
    printf("Set doPIDqa=%d according to environment variables\n",doPIDqa);
  }
  //
  if ( !(envS=gSystem->Getenv("FMD")).IsNull() && CheckEnvS("FMD",envS) ) {
    doFMD = atoi(envS.Data());
    printf("Set doFMD=%d according to environment variables\n",doFMD);
  }
  //
  if ( !(envS=gSystem->Getenv("PHOS")).IsNull() && CheckEnvS("PHOS",envS) ) {
    doPHOS = atoi(envS.Data());
    printf("Set doPHOS=%d according to environment variables\n",doPHOS);
  }
  //
  if ( !(envS=gSystem->Getenv("PHOSTrig")).IsNull() && CheckEnvS("PHOSTrig",envS) ) {
    doPHOSTrig = atoi(envS.Data());
    printf("Set doPHOSTrig=%d according to environment variables\n",doPHOSTrig);
  }
  //
  if ( !(envS=gSystem->Getenv("EMCAL")).IsNull() && CheckEnvS("EMCAL",envS) ) {
    doEMCAL = atoi(envS.Data());
    printf("Set doEMCAL=%d according to environment variables\n",doEMCAL);
  }
  //
  if ( !(envS=gSystem->Getenv("FBFqa")).IsNull() && CheckEnvS("FBFqa",envS) ) {
    doFBFqa = atoi(envS.Data());
    printf("Set doFBFqa=%d according to environment variables\n",doFBFqa);
  }
  //
  if ( !(envS=gSystem->Getenv("TaskFilteredTree")).IsNull() && CheckEnvS("TaskFilteredTree",envS) ) {
    doTaskFilteredTree = atoi(envS.Data());
    printf("Set doTaskFilteredTree=%d according to environment variables\n",doTaskFilteredTree);
  }
  //
  if ( !(envS=gSystem->Getenv("ADqa")).IsNull() && CheckEnvS("ADqa",envS) ) {
    doAD = atoi(envS.Data());
    printf("Set doAD=%d according to environment variables\n",doAD);
  }
  //-----------------------------------------------------------  
  // combinations
  if ( !(envS=gSystem->Getenv("QAGROUP")).IsNull() ) {
    int qaGRP = atoi(envS.Data());
    switch (qaGRP) {
    case 0: 
      SetDoQA(doStatistics,  1, "Statistics");
      //SetDoQA(doVZERO,       1, "VZERO");
            SetDoQA(doVZEROPbPb,   1, "VZEROPbPb");
      SetDoQA(doVertex,      1, "Vertex");
      SetDoQA(doPileup,      1, "Pileup");
      SetDoQA(doSPD,         1, "SPD");
      SetDoQA(doTPC,         0, "TPC");
      SetDoQA(doHLT,         0, "HLT");
      SetDoQA(doSDD,         0, "SDD");
      SetDoQA(doSSDdEdx,     0, "SSDdEdx");
      SetDoQA(doTRD,         1, "TRD");
      SetDoQA(doITS,         0, "ITS");
      SetDoQA(doITSsaTracks, 0, "ITSsaTracks");
      SetDoQA(doITSalign,    0, "ITSalign");
      SetDoQA(doESDTracks,   0, "ESDTracks");
      SetDoQA(doCALO ,       1, "CALO");
      SetDoQA(doMUONTrig,    0, "MUONTrig");
      SetDoQA(doMUONEff,     0, "MUONEff");
      SetDoQA(doImpParRes,   1, "ImpParRes");
      SetDoQA(doMUON,        0, "MUON");
      SetDoQA(doTOF,         0, "TOF");
      SetDoQA(doHMPID,       1, "HMPID");
      SetDoQA(doT0,          1, "T0");
      SetDoQA(doZDC,         1, "ZDC");
      //      SetDoQA(doPIDResponse, 1, "PIDResponse");
      SetDoQA(doPIDqa,       1, "PIDqa");
      SetDoQA(doFMD,         1, "FMD");
      SetDoQA(doPHOS,        1, "PHOS");
      SetDoQA(doPHOSTrig,    1, "PHOSTrig");
      SetDoQA(doEMCAL,       0, "EMCAL");
      SetDoQA(doFBFqa,       1, "FBFqa");
      SetDoQA(doAD,          1, "ADqa");
      break;
      
    case 1: 
      SetDoQA(doStatistics,  0, "Statistics");
      SetDoQA(doVZERO,       0, "VZERO");
      SetDoQA(doVZEROPbPb,   0, "VZEROPbPb");
      SetDoQA(doVertex,      0, "Vertex");
      SetDoQA(doPileup,      0, "Pileup");
      SetDoQA(doSPD,         0, "SPD");
      SetDoQA(doTPC,         1, "TPC");
      SetDoQA(doHLT,         1, "HLT");
      SetDoQA(doSDD,         0, "SDD");
      SetDoQA(doSSDdEdx,     0, "SSDdEdx");
      SetDoQA(doTRD,         0, "TRD");
      SetDoQA(doITS,         0, "ITS");
      SetDoQA(doITSsaTracks, 0, "ITSsaTracks");
      SetDoQA(doITSalign,    0, "ITSalign");
      SetDoQA(doESDTracks,   0, "ESDTracks");
      SetDoQA(doCALO ,       0, "CALO");
      SetDoQA(doMUONTrig,    0, "MUONTrig");
      SetDoQA(doMUONEff,     0, "MUONEff");
      SetDoQA(doImpParRes,   0, "ImpParRes");
      SetDoQA(doMUON,        0, "MUON");
      SetDoQA(doTOF,         0, "TOF");
      SetDoQA(doHMPID,       0, "HMPID");
      SetDoQA(doT0,          0, "T0");
      SetDoQA(doZDC,         0, "ZDC");
      //      SetDoQA(doPIDResponse, 0, "PIDResponse");
      SetDoQA(doPIDqa,       0, "PIDqa");
      SetDoQA(doFMD,         0, "FMD");
      SetDoQA(doPHOS,        0, "PHOS");
      SetDoQA(doPHOSTrig,    0, "PHOSTrig");
      SetDoQA(doEMCAL,       0, "EMCAL");
      SetDoQA(doFBFqa,       0, "FBFqa");
      SetDoQA(doAD,          0, "ADqa");
      break;
    case 2: 
      SetDoQA(doStatistics,  0, "Statistics");
      SetDoQA(doVZERO,       0, "VZERO");
      SetDoQA(doVZEROPbPb,   0, "VZEROPbPb");
      SetDoQA(doVertex,      0, "Vertex");
      SetDoQA(doPileup,      0, "Pileup");
      SetDoQA(doSPD,         0, "SPD");
      SetDoQA(doTPC,         0, "TPC");
      SetDoQA(doHLT,         0, "HLT");
      SetDoQA(doSDD,         0, "SDD");
      SetDoQA(doSSDdEdx,     0, "SSDdEdx");
      SetDoQA(doTRD,         0, "TRD");
      SetDoQA(doITS,         0, "ITS");
      SetDoQA(doITSsaTracks, 1, "ITSsaTracks");
      SetDoQA(doITSalign,    1, "ITSalign");
      SetDoQA(doESDTracks,   1, "ESDTracks");
      SetDoQA(doCALO ,       0, "CALO");
      SetDoQA(doMUONTrig,    0, "MUONTrig");
      SetDoQA(doMUONEff,     0, "MUONEff");
      SetDoQA(doImpParRes,   0, "ImpParRes");
      SetDoQA(doMUON,        0, "MUON");
      SetDoQA(doTOF,         0, "TOF");
      SetDoQA(doHMPID,       0, "HMPID");
      SetDoQA(doT0,          0, "T0");
      SetDoQA(doZDC,         0, "ZDC");
      //      SetDoQA(doPIDResponse, 0, "PIDResponse");
      SetDoQA(doPIDqa,       0, "PIDqa");
      SetDoQA(doFMD,         0, "FMD");
      SetDoQA(doPHOS,        0, "PHOS");
      SetDoQA(doPHOSTrig,    0, "PHOSTrig");
      SetDoQA(doEMCAL,       0, "EMCAL");
      SetDoQA(doFBFqa,       0, "FBFqa");
      SetDoQA(doAD,          0, "ADqa");
      break;
    case 3: 
      SetDoQA(doStatistics,  0, "Statistics");
      SetDoQA(doVZERO,       0, "VZERO");
      SetDoQA(doVZEROPbPb,   0, "VZEROPbPb");
      SetDoQA(doVertex,      0, "Vertex");
      SetDoQA(doPileup,      0, "Pileup");
      SetDoQA(doSPD,         0, "SPD");
      SetDoQA(doTPC,         0, "TPC");
      SetDoQA(doHLT,         0, "HLT");
      SetDoQA(doSDD,         1, "SDD");
      SetDoQA(doSSDdEdx,     1, "SSDdEdx");
      SetDoQA(doTRD,         0, "TRD");
      SetDoQA(doITS,         1, "ITS");
      SetDoQA(doITSsaTracks, 0, "ITSsaTracks");
      SetDoQA(doITSalign,    0, "ITSalign");
      SetDoQA(doESDTracks,   0, "ESDTracks");
      SetDoQA(doCALO ,       0, "CALO");
      SetDoQA(doMUONTrig,    0, "MUONTrig");
      SetDoQA(doMUONEff,     0, "MUONEff");
      SetDoQA(doImpParRes,   0, "ImpParRes");
      SetDoQA(doMUON,        0, "MUON");
      SetDoQA(doTOF,         1, "TOF");
      SetDoQA(doHMPID,       0, "HMPID");
      SetDoQA(doT0,          0, "T0");
      SetDoQA(doZDC,         0, "ZDC");
      //      SetDoQA(doPIDResponse, 0, "PIDResponse");
      SetDoQA(doPIDqa,       0, "PIDqa");
      SetDoQA(doFMD,         0, "FMD");
      SetDoQA(doPHOS,        0, "PHOS");
      SetDoQA(doPHOSTrig,    0, "PHOSTrig");
      SetDoQA(doEMCAL,       0, "EMCAL");
      SetDoQA(doFBFqa,       0, "FBFqa");
      SetDoQA(doAD,          0, "ADqa");
      break;
    case 4: 
      SetDoQA(doStatistics,  0, "Statistics");
      SetDoQA(doVZERO,       0, "VZERO");
      SetDoQA(doVZEROPbPb,   0, "VZEROPbPb");
      SetDoQA(doVertex,      0, "Vertex");
      SetDoQA(doPileup,      0, "Pileup");
      SetDoQA(doSPD,         0, "SPD");
      SetDoQA(doTPC,         0, "TPC");
      SetDoQA(doHLT,         0, "HLT");
      SetDoQA(doSDD,         0, "SDD");
      SetDoQA(doSSDdEdx,     0, "SSDdEdx");
      SetDoQA(doTRD,         0, "TRD");
      SetDoQA(doITS,         0, "ITS");
      SetDoQA(doITSsaTracks, 0, "ITSsaTracks");
      SetDoQA(doITSalign,    0, "ITSalign");
      SetDoQA(doESDTracks,   0, "ESDTracks");
      SetDoQA(doCALO ,       0, "CALO");
      SetDoQA(doMUONTrig,    1, "MUONTrig");
      SetDoQA(doMUONEff,     1, "MUONEff");
      SetDoQA(doImpParRes,   0, "ImpParRes");
      SetDoQA(doMUON,        1, "MUON");
      SetDoQA(doTOF,         0, "TOF");
      SetDoQA(doHMPID,       0, "HMPID");
      SetDoQA(doT0,          0, "T0");
      SetDoQA(doZDC,         0, "ZDC");
      //      SetDoQA(doPIDResponse, 0, "PIDResponse");
      SetDoQA(doPIDqa,       0, "PIDqa");
      SetDoQA(doFMD,         0, "FMD");
      SetDoQA(doPHOS,        0, "PHOS");
      SetDoQA(doPHOSTrig,    0, "PHOSTrig");
      SetDoQA(doEMCAL,       1, "EMCAL");
      SetDoQA(doFBFqa,       0, "FBFqa");
      SetDoQA(doAD,          0, "ADqa");
      break;
    };

    printf("Set doTaskFilteredTree=%d according to environment variables\n",doTaskFilteredTree);
  }


}

void SetDoQA(Bool_t& var, Bool_t onoff, const char* name)
{
  // set to ON only if previous value is not off
  if (var) var = onoff;
  if (name) printf("Set %-15s to %s\n",name,var ? "ON":"OFF");
}

Bool_t CheckEnvS(const char* var,TString& envString)
{
  if (envString=="0" || envString=="1") return kTRUE;
  else printf("Ignoring wrong value %s for environment variable %s\n",envString.Data(),var);
  return kFALSE;
}

void mergeQAgroups(const char* lst, const char* out="QAresults.root")
{
  TString lstS = lst;
  TString outS = out;
  if (lstS.IsNull()) exit(1);
  if (outS.IsNull()) exit(1);
  gSystem->Load("libCORRFW.so");
  AliFileMerger fm;
  fm.IterTXT(lstS.Data(),outS.Data());
  //  
}

//________________________________________________________
void PrintSettings()
{
  printf("\n   **********************************\n");
  printf("   * \n");
  printf("   * System:         %d\n", iCollisionType);
  printf("   * Run number:     %d\n", run_number);
  printf("   * Centrality:     %d\n", doCentrality);
  printf("   * TriggerInt:     %d\n", kTriggerInt);
  printf("   * TriggerMask:    %d\n", kTriggerMask);
  printf("   * \n");
  printf("   **********************************\n");
  gSystem->Exec("set");
  printf("   **********************************\n\n");
}
