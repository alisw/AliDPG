// QAtrainAOD.C
// Run few simple QA tasks to check the AOD creation

/*****************************************
When running in local mode, you need
to write a file containing, for example

export ALIEN_JDL_LPMINTERACTIONTYPE=pp

and source it
******************************************/


void QAtrainAOD(Bool_t isMC=kFALSE, Int_t iCollisionType=0){

  gROOT->LoadMacro("$ALICE_PHYSICS/OADB/macros/AddTaskPhysicsSelection.C");
  gROOT->LoadMacro("$ALICE_ROOT/ANALYSIS/macros/AddTaskPIDResponse.C");
  gROOT->LoadMacro("$ALICE_ROOT/ANALYSIS/macros/AddTaskPIDqa.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/OADB/COMMON/MULTIPLICITY/macros/AddTaskMultSelection.C");
  TString macroName="";
  macroName="$ALICE_PHYSICS/PWGPP/macros/AddTaskCheckVertexAOD.C";
  if(gSystem->Exec(TString::Format("ls %s > /dev/null 2>&1",macroName.Data()))==0){
    gROOT->LoadMacro(macroName.Data());
  }
  macroName="$ALICE_PHYSICS/PWGPP/macros/AddTaskCheckAODTracks.C";
  if(gSystem->Exec(TString::Format("ls %s > /dev/null 2>&1",macroName.Data()))==0){
    gROOT->LoadMacro(macroName.Data());
  }
  macroName="$ALICE_PHYSICS/PWGHF/vertexingHF/macros/AddTaskBasicHFQA.C";
  if(gSystem->Exec(TString::Format("ls %s > /dev/null 2>&1",macroName.Data()))==0){
    gROOT->LoadMacro(macroName.Data());
  }
  if(isMC){
    macroName="$ALICE_PHYSICS/PWGHF/vertexingHF/macros/AddTaskDmesonMCPerform.C";
    if(gSystem->Exec(TString::Format("ls %s > /dev/null 2>&1",macroName.Data()))==0){
      gROOT->LoadMacro(macroName.Data());
    }
  }

  if (gSystem->AccessPathName("main_QAtrainAOD.C", kFileExists)==0) {
    Printf("Using local main_QAtrainAOD.C");
    gROOT->Macro(TString::Format("main_QAtrainAOD.C(%d, %d)", (Int_t)isMC, iCollisionType));
  }
  else {
    Printf("Using main_QAtrainAOD.C from AliDPG"); 
    gROOT->Macro(TString::Format("$ALIDPG_ROOT/QA/main_QAtrainAOD.C(%d, %d)", (Int_t)isMC, iCollisionType));
  }
  
}
