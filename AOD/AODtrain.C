void AODtrain(Int_t merge=0){

  // macro to run the AOD train
  
  gROOT->LoadMacro("$ALICE_PHYSICS/TENDER/TenderSupplies/AddTaskTender.C");
  gROOT->LoadMacro("$ALICE_ROOT/ANALYSIS/macros/AddTaskPIDResponse.C");
  gROOT->LoadMacro("$ALICE_ROOT/ANALYSIS/macros/AddTaskPIDqa.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/PilotTrain/AddTaskCDBconnect.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/OADB/macros/AddTaskPhysicsSelection.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/macros/AddTaskFilteredTree.C"); 
  gROOT->LoadMacro("$ALICE_PHYSICS/OADB/COMMON/MULTIPLICITY/macros/AddTaskMultSelection.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/OADB/macros/AddTaskCentrality.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGLF/FORWARD/analysis2/AddTaskForwardMult.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGLF/FORWARD/analysis2/AddTaskCentralMult.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGGA/GammaConv/macros/AddTask_ConversionAODProduction.C");
  gROOT->LoadMacro("$ALICE_ROOT/ANALYSIS/ESDfilter/macros/AddTaskESDFilter.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGHF/vertexingHF/macros/AddTaskVertexingHF.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGDQ/dielectron/macros/ConfigJpsi2eeFilter.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGDQ/dielectron/macros/AddTaskJPSIFilter.C");

  //gROOT->LoadMacro("$ALICE_PHYSICS/PWGJE/macros/AddTaskJets.C");
  //gROOT->LoadMacro("$ALICE_PHYSICS/PWGJE/macros/AddTaskJetCluster.C");
  //gROOT->LoadMacro("$ALICE_PHYSICS/PWGJE/macros/AddTaskJetBackgroundSubtract.C");
  
  gROOT->Macro(TString::Format("$ALIDPG_ROOT/AOD/main_AODtrain.C(%d)", merge));
	       
}
