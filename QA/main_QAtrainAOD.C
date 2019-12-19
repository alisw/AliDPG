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

void main_QAtrainAOD(Int_t isMC=0, Int_t iCollisionType=0){

  

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
  printf("Collision System is %s\n",CollisionSystem[iCollisionType]);
  printf("--------------------------------------\n");
 
  UInt_t kTriggerMask =  AliVEvent::kINT7 | AliVEvent::kINT8;
  if(gSystem->Getenv("ALIEN_JDL_LPMANCHORYEAR"))
  {
    Int_t year = atoi(gSystem->Getenv("ALIEN_JDL_LPMANCHORYEAR"));
    if(year <= 2015) 
      kTriggerMask = AliVEvent::kAnyINT;
  }
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
    MChandler->SetReadTR(kFALSE);
    mgr->SetMCtruthEventHandler(MChandler);      
  }
  TString macroName="";

  // Physics selection
  AliPhysicsSelectionTask* physSelTask = AddTaskPhysicsSelection(isMC);
  mgr->AddStatisticsTask(kTriggerMask);
  AliAnalysisDataContainer *cstatsout = (AliAnalysisDataContainer*)mgr->GetOutputs()->FindObject("cstatsout");
  cstatsout->SetFileName("EventStat_temp_AOD.root");

  // PID response
  AliAnalysisTaskPIDResponse *setupTask = AddTaskPIDResponse(isMC);
  //, 1, 1, "1", 0, "", 1, 1, 1); // the folloing to be used e.g. when running locally on AODs from a "pass1" - the settings are needed to load the appropriate splines
  
  // PIDqa
  AliAnalysisTaskPIDqa *PIDQA = AddTaskPIDqa();
  PIDQA->SelectCollisionCandidates(AliVEvent::kAny);

  // MultSelection
  AliMultSelectionTask *taskMult = AddTaskMultSelection();
  if(isMC){
    if (gSystem->Getenv("CONFIG_PERIOD")){
      TString periodName = gSystem->Getenv("CONFIG_PERIOD");
      taskMult->SetAlternateOADBforEstimators(periodName);
    }
  }
  // Vertex QA
  macroName="$ALICE_PHYSICS/PWGPP/macros/AddTaskCheckVertexAOD.C";
  if(gSystem->Exec(TString::Format("ls %s > /dev/null 2>&1",macroName.Data()))==0){
    Double_t maxMult=500.;
    if(iCollisionType==kPbPb || iCollisionType==kXeXe) maxMult=10000.;
    AliAnalysisTaskCheckVertexAOD *taskVert =  reinterpret_cast<AliAnalysisTaskCheckVertexAOD *>(gInterpreter->ProcessLine(TString::Format("AddTaskCheckVertexAOD(\"QA\",%f, %d, %d)", maxMult, AliVEvent::kAnyINT, (Int_t)isMC)));
  }else{
    printf("Macro %s not found -> task will not be executed\n",macroName.Data());
  }

  // Track QA
  macroName="$ALICE_PHYSICS/PWGPP/macros/AddTaskCheckAODTracks.C";
  if(gSystem->Exec(TString::Format("ls %s > /dev/null 2>&1",macroName.Data()))==0){
    Bool_t usMCtruthForPID=isMC;
    AliAnalysisTaskCheckAODTracks* taskAODtr = reinterpret_cast<AliAnalysisTaskCheckAODTracks*>(gInterpreter->ProcessLine(TString::Format("AddTaskCheckAODTracks(\"QA\", %d, %d)", (Int_t)isMC, (Int_t)usMCtruthForPID)));
    if(iCollisionType==kPbPb || iCollisionType==kXeXe){
      taskAODtr->SetUpperMultiplicity(10000.);
      taskAODtr->SetUsePileupCut(kFALSE);
    }
  }else{
    printf("Macro %s not found -> task will not be executed\n",macroName.Data());
  }


  // HF deltaAOD QA
  macroName="$ALICE_PHYSICS/PWGHF/vertexingHF/macros/AddTaskBasicHFQA.C";
  if(gSystem->Exec(TString::Format("ls %s > /dev/null 2>&1",macroName.Data()))==0){
    AliAnalysisTaskSEHFQA* dqaHF = reinterpret_cast<AliAnalysisTaskSEHFQA*>(gInterpreter->ProcessLine(TString::Format("AddTaskBasicHFQA(AliAnalysisTaskSEHFQA::kD0toKpi, %d, \"QA\")", (Int_t)isMC)));
  }else{
    printf("Macro %s not found -> task will not be executed\n",macroName.Data());
  }

  if(isMC){
    macroName="$ALICE_PHYSICS/PWGHF/vertexingHF/macros/AddTaskDmesonMCPerform.C";
    if(gSystem->Exec(TString::Format("ls %s > /dev/null 2>&1",macroName.Data()))==0){
      AliAnalysisTaskDmesonMCPerform* dMCp = reinterpret_cast<AliAnalysisTaskDmesonMCPerform*>(gInterpreter->ProcessLine(TString::Format("AddTaskDmesonMCPerform(\"QA\")")));
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
