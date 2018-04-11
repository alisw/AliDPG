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

void QAtrain_duo(const char *suffix="", Int_t run = 0, 
		 const char *xmlfile   = "wn.xml",
		 Int_t  stage          = 0, /*0 = QA train, 1...n - merging stage*/
		 const char *cdb     = "raw://")
//             const char *cdb     = "local://$ALICE_ROOT/OCDB")
{

  // loading all necessary macros

  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/PilotTrain/AddTaskCDBconnect.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/OADB/macros/AddTaskPhysicsSelection.C");
  gROOT->LoadMacro("$ALICE_ROOT/ANALYSIS/macros/AddTaskPIDResponse.C"); 
  gROOT->LoadMacro("$ALICE_PHYSICS/OADB/COMMON/MULTIPLICITY/macros/AddTaskMultSelection.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/macros/AddTaskVertexESD.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/PilotTrain/AddTaskQAsym.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/PilotTrain/AddTaskVZEROQA.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/PilotTrain/AddTaskVZEROQATrig.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/VZERO/AddTaskVZEROPbPb.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/TPC/macros/AddTaskPerformanceTPCdEdxQA.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/PilotTrain/AddTaskSPDQA.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/PilotTrain/AddSDDPoints.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/PilotTrain/AddTaskdEdxSSDQA.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/macros/AddTaskPileupChecks.C"); 
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/macros/AddTaskPerformanceITS.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/macros/AddTaskITSsaTracks.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/macros/AddTaskITSAlign.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/macros/AddTaskCheckESDTracks.C");

  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/ZDC/AddTaskZDCQA.C");

  gROOT->LoadMacro("$ALICE_PHYSICS/PWGGA/CaloTrackCorrelations/macros/QA/AddTaskCalorimeterQA.C");

  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/macros/AddTaskMTRchamberEfficiency.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/MUON/dep/AddTaskMUONTrackingEfficiency.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/macros/AddTaskImpParRes.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/macros/AddTaskImpParResSparse.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/PilotTrain/AddTaskMuonQA.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/TOF/AddTaskTOFqaID.C");
  gROOT->LoadMacro("$ALICE_ROOT/ANALYSIS/macros/AddTaskPIDqa.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/HMPID/AddTaskHmpidQA.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/T0/AddTaskT0QA.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGLF/FORWARD/analysis2/AddTaskForwardQA.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGGA/PHOSTasks/CaloCellQA/macros/AddTaskCaloCellsQA.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGGA/PHOSTasks/PHOS_PbPbQA/macros/AddTaskPHOSPbPb.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGGA/PHOSTasks/PHOS_TriggerQA/macros/AddTaskPHOSTriggerQA.C");
  //gROOT->LoadMacro("$ALICE_PHYSICS/PWGGA/EMCALTasks/macros/AddTaskEMCALTriggerQA.C"); // obsolete
  gROOT->LoadMacro("$ALICE_PHYSICS/PWG/EMCAL/macros/AddTaskEmcalTriggerMakerNew.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWG/EMCAL/macros/AddTaskEmcalTriggerQA_QAtrain.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/macros/AddTaskFBFqa.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/macros/AddTaskFilteredTree.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/AD/AddTaskADQA.C");

  // loading the libraries (needed for ROOT5)
  gROOT->Macro("$ALICE_PHYSICS/PWGPP/CalibMacros/CPass1/LoadLibraries.C");
  
  // running the main macro
if (gSystem->AccessPathName("main_QAtrain_duo.C", kFileExists)==0) {
    Printf("Using local main_QAtrain_duo.C");
    gROOT->Macro(TString::Format("$ALIDPG_ROOT/QA/main_QAtrain_duo.C(\"%s\", %d, \"%s\", %d, \"%s\")", suffix, run, xmlfile, stage, cdb));
  }
  else {
    Printf("Using main_QAtrain_duo.C from AliDPG");    
    gROOT->Macro(TString::Format("$ALIDPG_ROOT/QA/main_QAtrain_duo.C(\"%s\", %d, \"%s\", %d, \"%s\")", suffix, run, xmlfile, stage, cdb));
  }  

  return;

}
