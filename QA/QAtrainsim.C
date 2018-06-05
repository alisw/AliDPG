//______________________________________________________________________________
void QAtrainsim(Int_t run = 0,
             const char *xmlfile   = "wn.xml",
             Int_t  stage          = 0, /*0 = QA train, 1...n - merging stage*/
             const char *cdb     = "raw://")
{

  // Fixes/protections for specific AliRoot Versions
  // ESD track QA can be run for AliPhysics >= v5-09-14
  // PHOS QA tasks need different arguments starting for AliPhysics >= v5-09-24
  // we expect by default to be using an AliPhysics recent than the two above
  Bool_t disableESDtrackQA=kFALSE;
  Bool_t useEmptyStringForPHOS=kTRUE;
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
    if(ver<5){
      disableESDtrackQA=kTRUE;
      useEmptyStringForPHOS=kFALSE;
    }else if(ver==5){
      if(n1<9 || (n1==9 && n2<14)) disableESDtrackQA=kTRUE; // < v5-09-14
      if(n1<9 || (n1==9 && n2<24)) useEmptyStringForPHOS=kFALSE;  // < v5-09-24
    }
  }

  // loading all necessary macros
  gROOT->LoadMacro("$ALIDPG_ROOT/MC/OCDBConfig.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/PilotTrain/AddTaskCDBconnect.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/OADB/macros/AddTaskPhysicsSelection.C");
  gROOT->LoadMacro("$ALICE_ROOT/ANALYSIS/macros/AddTaskPIDResponse.C"); 
  gROOT->LoadMacro("$ALICE_PHYSICS/OADB/COMMON/MULTIPLICITY/macros/AddTaskMultSelection.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/OADB/macros/AddTaskCentrality.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/macros/AddTaskVertexESD.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/PilotTrain/AddTaskQAsym.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/PilotTrain/AddTaskVZEROQA.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/VZERO/AddTaskVZEROPbPb.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/TPC/macros/AddTaskPerformanceTPCdEdxQA.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/PilotTrain/AddTaskSPDQA.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/PilotTrain/AddSDDPoints.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/PilotTrain/AddTaskdEdxSSDQA.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/macros/AddTaskPerformanceITS.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/macros/AddTaskITSsaTracks.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/macros/AddTaskITSAlign.C");
  if(disableESDtrackQA) printf("AliAnalysisTaskCheckESDTracks switched off due to AliPhysics<v5-09-14\n");
  else{
    gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/macros/AddTaskCheckESDTracks.C");
  }
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/ZDC/AddTaskZDCQA.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGGA/CaloTrackCorrelations/macros/QA/AddTaskCalorimeterQA.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/macros/AddTaskMTRchamberEfficiency.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/MUON/dep/AddTaskMUONTrackingEfficiency.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/MUON/dep/AddTaskMuonPerformance.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/macros/AddTaskV0QA.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/macros/AddTaskImpParRes.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/PilotTrain/AddTaskMuonQA.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/TOF/AddTaskTOFqaID.C");
  gROOT->LoadMacro("$ALICE_ROOT/ANALYSIS/macros/AddTaskPIDqa.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/HMPID/AddTaskHmpidQA.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/T0/AddTaskT0QA.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGLF/FORWARD/analysis2/AddTaskForwardQA.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGGA/PHOSTasks/CaloCellQA/macros/AddTaskCaloCellsQA.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGGA/PHOSTasks/PHOS_PbPbQA/macros/AddTaskPHOSPbPb.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGGA/PHOSTasks/PHOS_TriggerQA/macros/AddTaskPHOSTriggerQA.C");
  //  gROOT->LoadMacro("$ALICE_PHYSICS/PWGGA/EMCALTasks/macros/AddTaskEMCALTriggerQA.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/EvTrkSelection/AddSingleTrackEfficiencyTaskForAutomaticQA.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/AD/AddTaskADQA.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/macros/AddTaskFBFqa.C");



  // running the main macro
  if (gSystem->AccessPathName("main_QAtrainsim.C", kFileExists)==0) {
    Printf("Using local main_QAtrainsim.C");
    gROOT->Macro(TString::Format("main_QAtrainsim.C(%d, \"%s\", %d, \"%s\", %d, %d)", run, xmlfile, stage, cdb, (Int_t)disableESDtrackQA, (Int_t)useEmptyStringForPHOS));
  }
  else {
    Printf("Using main_QAtrain_duo.C from AliDPG");    
    gROOT->Macro(TString::Format("$ALIDPG_ROOT/QA/main_QAtrainsim.C(%d, \"%s\", %d, \"%s\", %d, %d)", run, xmlfile, stage, cdb, (Int_t)disableESDtrackQA, (Int_t)useEmptyStringForPHOS));
  }  

  return;

}
