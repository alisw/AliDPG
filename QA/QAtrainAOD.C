// QAtrainAOD.C
// Run few simple QA tasks to check the AOD creation

/*****************************************
When running in local mode, you need
to write a file containing, for example

export ALIEN_JDL_LPMINTERACTIONTYPE=pp

and source it
******************************************/

enum ECOLLISIONSYSTEM_t
{
    kpp,
    kPbPb,
    kpA,
    kAp,
    kNSystem
};

const Char_t* CollisionSystem[kNSystem] =
{
    "pp",
    "PbPb",
    "pA",
    "Ap"
};

void QAtrainAOD(Bool_t isMC=kFALSE, Int_t iCollisionType=0){

  

  if(gSystem->Getenv("ALIEN_JDL_LPMINTERACTIONTYPE")){
    for (Int_t icoll = 0; icoll < kNSystem; icoll++)
      if (strcmp(gSystem->Getenv("ALIEN_JDL_LPMINTERACTIONTYPE"), CollisionSystem[icoll]) == 0){
        iCollisionType = icoll;
	break;
      }
  }
  printf("--------------------------------------\n");
  if(isMC) printf("Run the AOD QA train for Monte Carlo\n");
  else printf("Run the AOD QA train for data\n");
  printf("Collision System is %s\n",CollisionSystem[icoll]);
  printf("--------------------------------------\n");
 
  // Create manager
  AliAnalysisManager *mgr  = new AliAnalysisManager("QAtrainAOD", "AOD QA train");
  mgr->SetCacheSize(0);
  mgr->SetCommonFileName("QAresults_AOD.root");
  // Input handler
  AliESDInputHandlerRP *esdHandler = new AliESDInputHandlerRP();
  AliAODInputHandler* aodHandler = new AliAODInputHandler();
  mgr->SetInputEventHandler(aodHandler);
  if(isMC){
    AliMCEventHandler* MChandler = new AliMCEventHandler;
    handler->SetReadTR(kFALSE);
    mgr->SetMCtruthEventHandler(MChandler);      
  }
  TString macroName="";

  // Physics selection
  gROOT->LoadMacro("$ALICE_PHYSICS/OADB/macros/AddTaskPhysicsSelection.C");
  AliPhysicsSelectionTask* physSelTask = AddTaskPhysicsSelection(isMC);
  AliAnalysisDataContainer *cstatsout = (AliAnalysisDataContainer*)mgr->GetOutputs()->FindObject("cstatsout");
  cstatsout->SetFileName("EventStatAOD_temp.root");

  // PID response
  gROOT->LoadMacro("$ALICE_ROOT/ANALYSIS/macros/AddTaskPIDResponse.C");
  AliAnalysisTaskSE *setupTask = AddTaskPIDResponse(isMC);
  
  // PIDqa
  gROOT->LoadMacro("$ALICE_ROOT/ANALYSIS/macros/AddTaskPIDqa.C");
  AliAnalysisTaskPIDqa *PIDQA = AddTaskPIDqa();
  PIDQA->SelectCollisionCandidates(AliVEvent::kAny);

  // MultSelection
  gROOT->LoadMacro("$ALICE_PHYSICS/OADB/COMMON/MULTIPLICITY/macros/AddTaskMultSelection.C");
  AliMultSelectionTask *taskMult = AddTaskMultSelection();

  // Vertex QA
  macroName="$ALICE_PHYSICS/PWGPP/macros/AddTaskCheckVertexAOD.C";
  if(gSystem->Exec(Form("ls %s > /dev/null 2>&1",macroName.Data()))==0){
    gROOT->LoadMacro(macroName.Data());
    Double_t maxMult=500.;
    if(iCollisionType==kPbPb) maxMult=10000.;
    AliAnalysisTaskCheckVertexAOD *taskVert =  AddTaskCheckVertexAOD("QA",maxMult,AliVEvent::kAnyINT,isMC);
  }else{
    printf("Macro %s not found -> task will not be executed\n",macroName.Data());
  }

  // Track QA
  macroName="$ALICE_PHYSICS/PWGPP/macros/AddTaskCheckAODTracks.C";
  if(gSystem->Exec(Form("ls %s > /dev/null 2>&1",macroName.Data()))==0){
    gROOT->LoadMacro(macroName.Data());
    Bool_t usMCtruthForPID=isMC;
    AliAnalysisTaskCheckAODTracks* taskAODtr = AddTaskCheckAODTracks("QA",isMC,usMCtruthForPID);
    if(iCollisionType==kPbPb) taskAODtr->SetUpperMultiplicity(10000.)
  }else{
    printf("Macro %s not found -> task will not be executed\n",macroName.Data());
  }


  // HF deltaAOD QA
  macroName="$ALICE_PHYSICS/PWGHF/vertexingHF/macros/AddTaskBasicHFQA.C";
  if(gSystem->Exec(Form("ls %s > /dev/null 2>&1",macroName.Data()))==0){
    gROOT->LoadMacro(macroName.Data());
    AliAnalysisTaskSEHFQA* dqaHF = AddTaskBasicHFQA(AliAnalysisTaskSEHFQA::kD0toKpi,isMC,"QA");
  }else{
    printf("Macro %s not found -> task will not be executed\n",macroName.Data());
  }

  if(isMC){
    macroName="$ALICE_PHYSICS/PWGHF/vertexingHF/macros/AddTaskDmesonMCPerform.C";
    if(gSystem->Exec(Form("ls %s > /dev/null 2>&1",macroName.Data()))==0){
      gROOT->LoadMacro(macroName.Data());
      AliAnalysisTaskDmesonMCPerform* dMCp = AddTaskDmesonMCPerform("QA");
    }else{
      printf("Macro %s not found -> task will not be executed\n",macroName.Data());
    }
  }

  TChain *chainAOD = new TChain("aodTree");
  TChain *chainAODfriend = new TChain("aodTree");
  chainAOD->Add("AliAOD.root");
  chainAODfriend->Add("AliAOD.VertexingHF.root"); 
  chainAOD->AddFriend(chainAODfriend);

  TStopwatch timer;
  timer.Start();
  if (mgr->InitAnalysis()) {                      
    mgr->PrintStatus(); 
    mgr->SetSkipTerminate(kTRUE);
    mgr->StartAnalysis("local", chainAOD);
  }
  gSystem->Exec("rm TreeOfAODTracks.root");
  timer.Print();
}
