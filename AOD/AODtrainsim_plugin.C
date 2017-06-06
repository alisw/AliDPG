//===================== ANALYSIS TRAIN =========================================
// To use: copy this macro to your work directory, modify the global part to match
// your needs, then run root.
//    root[0] .L AnalysisTrain.C
// Grid full mode as below (other modes: test, offline, submit, terminate)
//    root[1] AnalysisTrainNew("grid", "full")
// CAF mode (requires root v5-23-02 + aliroot v4-16-Rev08)
//    root[2] AnalysisTrainNew("proof")
// Local mode requires AliESds.root or AliAOD.root in ./data directory
//    root[3] AnalysisTrainNew("local")
// In proof and grid modes, a token is needed and sourcing the produced environment file.
//
// If 'saveTrain' flag is set, the train will generate a directory name and run
// in this directory. A configuration file 'ConfigTrain.C' will be generated. 
// One can replay at any time the train via:
//    root[1] AnalysisTrainNew(ana_mode, plugin_mode, "train_default_<date>/ConfigTrain.C")

/*****************************************
When running in local mode, you need
to write a file containing, for example

export ALIEN_JDL_LPMINTERACTIONTYPE=pp
export ALIEN_JDL_LPMANCHORYEAR=2015
export ALIEN_JDL_LPMPRODUCTIONTAG=LHC15n
export ALIEN_JDL_LPMRUNNUMBER=244628
export ALIEN_JDL_MCPRODTYPE=muon_calo

then source it
******************************************/

//
// Settings below are going to be overwritten at runtime, from environment vars
//

TString     train_tag          = "_p-p_";   // Train special tag appended to 
Int_t       iCollision         = 0;         // 0=pp, 1=Pb-Pb
Int_t       run_flag           = 1500;      // 1000 year 2010 LHCbcde cuts, 1100 year 2011 pp cuts, 1500 year 2015 
TString     periodName         = "LHC15n";  //
Int_t       run_number         = 0;    // For tasks arguments, CDB connect
Bool_t      localRunning       = kFALSE;    // Missing environment vars will cause a crash; change it to kTRUE if running locally w/o env vars
Bool_t      isMuonCaloPass     = kFALSE;    // setting this to kTRUE will disable some not needed analysis tasks for a muon_calo pass

TString     aliphysics_version = "v5-09-08-01-1"; // *CHANGE ME IF MORE RECENT IN GRID*

//==================   TRAIN NAME   ============================================
TString     train_name         = "FILTERsim"; // local folder name

TString     visible_name;
TString     job_comment;
TString     job_tag;

/****************** JUST FOR TEST MODE **************************/
// Change production base directory here (test mode)
TString     alien_datadir      = "/alice/sim/2016/LHC16c2";
// Data pattern - change as needed for test mode
TString     data_pattern       = "*ESDs.root";
Int_t run_numbers[10] = {177580}; // Set the run range, for testing
/****************************************************************/

//==============================================================================

// ### Settings that make sense when using the Alien plugin
//==============================================================================
Int_t       runOnData          = 0;       // Set to 1 if processing real data
Bool_t      useProductionMode  = kTRUE;   // use the plugin in production mode
// Usage of par files ONLY in grid mode and ONLY if the code is not available
// in the deployed AliRoot versions. Par file search path: local dir, if not there $ALICE_ROOT.
// To refresh par files, remove the ones in the workdir, then do "make <target.par>" in 
// AliRoot.

// Work directory in GRID (DON'T CHANGE)
TString     grid_workdir       = "/alice/cern.ch/user/a/alidaq/AOD/AOD$2";
//Int_t       run_range[2]       =  {114786, 114949};  // LHC09a7   *CHANGE ME*
// AliEn output directory. If blank will become output_<train_name>
               // Output directory (DON'T CHANGE)
TString     alien_outdir       = "$1/AOD$2";
               // Input collection (production mode)
TString     data_collection    = "$1/qa1.xml";
// Output folder to write delta AOD's. Considered if not null.
TString     outputSingleFolder = "";
//TString     outputSingleFolder = "deltas";
// Number of files merged in a chunk
Int_t       maxMergeFiles      = 30;
// Number of test files
Int_t       nTestFiles         = 1;
// Files that should not be merged
TString     mergeExclude       = "AliAOD.root AliAOD.VertexingHF.root FilterEvents_Trees.root AliAOD.Jets.root AliAOD.Muons.root AliAODGammaConversion.root";
TString     mergeDirName       = "AOD$2";
// Make replicas on the storages below
TString     outputStorages      = "disk=4";
Int_t       outputReplicas      = 2;
// Number of runs per master job
Int_t       nRunsPerMaster     = 10;
// Maximum number of files per job (gives size of AOD)
Int_t       nFilesPerJob       = 20;
// Int_t       nFilesPerJob       = 1; (AOD->delta AOD production case)
// ### Settings that make sense only for local analysis
//==============================================================================
// Change local xml dataset for local interactive analysis
TString     local_xmldataset   = "";
const char *cdbPath = "raw://";

enum ECOLLISIONSYSTEM_t
{
    kpp,
    kPbPb,
    kpA,
    kNSystem
};

const Char_t* CollisionSystem[kNSystem] =
{
    "pp",
    "PbPb",
    "pA",
};

void ProcessEnvironment(); // set the system, centrality, run_flag from ALIEN
void PrintSettings();

// ### Other flags to steer the analysis
//==============================================================================
Bool_t      doCDBconnect        = kTRUE;
Bool_t      usePhysicsSelection = kTRUE; // use physics selection
Bool_t      useTender           = kFALSE; // use tender wagon
Bool_t      useCentrality       = kFALSE; // centrality delta AOD
Bool_t      useV0tender         = kFALSE;  // use V0 correction in tender
Bool_t      useMergeViaJDL      = kTRUE;  // merge via JDL
Bool_t      useFastReadOption   = kFALSE;  // use xrootd tweaks
Bool_t      useOverwriteMode    = kTRUE;  // overwrite existing collections
Bool_t      useDATE             = kFALSE; // use date in train name
Bool_t      useDBG              = kTRUE;  // activate debugging
Bool_t      useMC               = kTRUE;  // use MC info
Bool_t      useKFILTER          = kTRUE;  // use Kinematics filter
Bool_t      useTR               = kTRUE;  // use track references
Bool_t      saveTrain           = kTRUE;  // save train configuration as: 
Bool_t      saveCanvases        = kFALSE;  // save canvases created in Terminate

// ### Analysis modules to be included. Some may not be yet fully implemented.
//==============================================================================
Int_t       iAODanalysis        = 0;      // AOD based analysis 
Int_t       iAODhandler         = 1;      // Analysis produces an AOD or dAOD's
Int_t       iESDfilter          = 1;      // ESD to AOD filter (barrel + muon tracks)
Int_t       iMUONcopyAOD        = 1;      // Task that copies only muon events in a separate AOD (PWG3)
Int_t       iJETAN              = 0;      // Jet analysis (PWG4)
Int_t       iJETANdelta         = 0;      // Jet delta AODs
Int_t       iPWGHFvertexing     = 1;      // Vertexing HF task (PWG3)
Int_t       iPWGDQJPSIfilter    = 0;      // JPSI filtering (PWG3)
Int_t       iPWGHFd2h           = 1;      // D0->2 hadrons (PWG3)
Int_t       iPIDResponse        = 1;      // PID response
Int_t       iPWGLFForward       = 1;      // Forward mult task (PWGLF)
Int_t       iPWGGAgammaconv     = 1;      // Gamma conversion analysis (PWG4)
Int_t       iPWGPP              = 1;      // high pt filter task
Int_t       doPIDqa             = 1;      // 
Int_t       iMUONRefit          = 0;

// ### Configuration macros used for each module
//==============================================================================
//TString configPWGHFd2h = (iCollision==0)?"$ALICE_PHYSICS/PWGHF/vertexingHF/ConfigVertexingHF.C"
//                          :"$ALICE_PHYSICS/PWGHF/vertexingHF/ConfigVertexingHF_Pb_AllCent.C";
//                            :"$ALICE_PHYSICS/PWGHF/vertexingHF/ConfigVertexingHF_Pb_AllCent_NoLS_NoDstar_16var.C";
//                            :"$ALICE_PHYSICS/PWGHF/vertexingHF/ConfigVertexingHF_ITSUpgrade_16var.C";

//______________________________________________________________________________
void UpdateFlags()
{
  // Update flags according to type of pass
  if ( isMuonCaloPass )
  {
    // disable the analysis we know for sure can not work or are meaningless
    // for a muon_calo pass
    doCDBconnect       = kFALSE;
    iPWGHFvertexing    = 0; 
    iPWGHFd2h          = 0; 
    iPWGPP             = 0; 
    iPWGLFForward      = 0; 
    iPWGGAgammaconv    = 0; 
    doPIDResponse      = 0; 
    doPIDqa            = 0; 
  }
}

//______________________________________________________________________________
void AODtrainsim_plugin(const char *analysis_mode="grid", 
                      const char *plugin_mode="full",
                      const char *config_file="")
{
  ProcessEnvironment();
  
  UpdateFlags();
  
  PrintSettings();

  //==================   TRAIN NAME   ============================================  
  //
  // Name in train page (DON'T CHANGE)
  visible_name       = Form("FILTER%s$2_$3", train_tag.Data()); //# FIXED #
  // Add train composition and other comments
  job_comment        = "Standard AODs + deltas";
  job_tag            = Form("%s: %s", visible_name.Data(), job_comment.Data());


// Main analysis train macro. If a configuration file is provided, all parameters
// are taken from there but may be altered by CheckModuleFlags.
   if (strlen(config_file) && !LoadConfig(config_file)) return;
   TString smode(analysis_mode);
   smode.ToUpper();
   TString spmode(plugin_mode);
   spmode.ToLower();
   if (spmode == "test") useProductionMode = kFALSE;
   // Check compatibility of selected modules
   if (saveTrain)              WriteConfig();

   printf("==================================================================\n");
   printf("===========    RUNNING FILTERING TRAIN %s IN %s MODE   ==========\n", train_name.Data(),smode.Data());
   printf("==================================================================\n");
   printf("=  Configuring analysis train for:                               =\n");
   if (usePhysicsSelection)   printf("=  Physics selection                                                =\n");
   if (useTender)    printf("=  TENDER                                                        =\n");
   if (iESDfilter)   printf("=  ESD filter                                                    =\n");
   if (iMUONcopyAOD) printf("=  MUON copy AOD                                                 =\n");
   if (iJETAN)       printf("=  Jet analysis                                                  =\n");
   if (iJETANdelta)  printf("=     Jet delta AODs                                             =\n");
   if (iPWGHFvertexing) printf("=  PWGHF vertexing                                                =\n");
   if (iPWGDQJPSIfilter) printf("=  PWGDQ j/psi filter                                             =\n");
   if (iPWGHFd2h) printf("=  PWGHF D0->2 hadrons QA                                     =\n");

   //==========================================================================
   // Make the analysis manager and connect event handlers
   AliAnalysisManager *mgr  = new AliAnalysisManager("Analysis Train", "Production train");
   if (!strcmp(plugin_mode, "test")) mgr->SetNSysInfo(100);

   // Create input handler (input container created automatically)
   if (iAODanalysis) {
   // AOD input handler
      AliAODInputHandler *aodH = new AliAODInputHandler();
      if (iPWGHFd2h) aodH->AddFriend("AliAOD.VertexingHF.root");
      mgr->SetInputEventHandler(aodH);
   } else {   
   // ESD input handler
      AliESDInputHandler *esdHandler = new AliESDInputHandler();
      mgr->SetInputEventHandler(esdHandler);       
   }
   // Monte Carlo handler
   if (useMC) {
      AliMCEventHandler* mcHandler = new AliMCEventHandler();
      mgr->SetMCtruthEventHandler(mcHandler);
      mcHandler->SetPreReadMode(1);
      mcHandler->SetReadTR(useTR); 
   }   
   // AOD output container, created automatically when setting an AOD handler
   if (iAODhandler) {
      // AOD output handler
      AliAODHandler* aodHandler   = new AliAODHandler();
      aodHandler->SetOutputFileName("AliAOD.root");
      mgr->SetOutputEventHandler(aodHandler);
      if (iAODanalysis) {
         aodHandler->SetFillAOD(kFALSE);
         aodHandler->SetCreateNonStandardAOD();
      } 
   }
   // Debugging if needed
   if (useDBG) mgr->SetDebugLevel(3);
   if (saveCanvases) mgr->SetSaveCanvases(kTRUE);

   //==========================================================================
   // Create the chain. In this example it is created only from ALIEN files but
   // can be done to work in batch or grid mode as well.
   TChain *chain = CreateChain(smode, plugin_mode);
           
   //==========================================================================
   // Load the tasks configuration macros for all wagons. These files are supposed now to be
   // in the current workdir, but in AliEn they will be in the file catalog, 
   // mapped from AliRoot and pecified in the jdl input list.
    
   // For now connection to top input container and common AOD output container
   // is done in this macro, but in future these containers will be connected
   // from each task configuration macro.
                                                                                                                                           
   AddAnalysisTasks(cdbPath);                                                                                                                     
   // Run the analysis                                                                                                                     
   //                                                                                                                                      
   AliAnalysisGrid *alienHandler = CreateAlienHandler(plugin_mode);                                                                     
   AliAnalysisManager::GetAnalysisManager()->SetGridHandler(alienHandler);                                                              
                                                                                                                                           
   if (mgr->InitAnalysis()) {                                                                                                              
      mgr->PrintStatus();                                                                                                                  
      if (saveTrain || strlen(config_file)) gSystem->ChangeDirectory(train_name);                                                          
      StartAnalysis(smode, chain);                                                                                                         
   }                                                                                                                                       
}                                                                                                                                          

//______________________________________________________________________________                                                           
void AddAnalysisTasks(const char *cdb_location)
{
  // Add all analysis task wagons to the train                                                                                               
   AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();                                                                     

   //
  // PIDResponse(JENS)
  //
  if (iPIDResponse)
  {
    gROOT->LoadMacro("$ALICE_ROOT/ANALYSIS/macros/AddTaskPIDResponse.C"); 
    AliAnalysisTaskPIDResponse *PIDResponse = AddTaskPIDResponse(useMC, kFALSE, kTRUE, 2);
//    PIDResponse->SelectCollisionCandidates(AliVEvent::kAny);
  }
 //

  // CDB connection
  //
  if (doCDBconnect && !useTender) {
    gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/PilotTrain/AddTaskCDBconnect.C");
    AliTaskCDBconnect *taskCDB = AddTaskCDBconnect(cdb_location, run_number);
    if (!taskCDB) return;
//    AliCDBManager *cdb = AliCDBManager::Instance();
//    cdb->SetDefaultStorage(cdb_location);
//    taskCDB->SetRunNumber(run_number);
  }    

  // Tender and supplies. Needs to be called for every event.
  //
   if (useTender) {
      gROOT->LoadMacro("$ALICE_PHYSICS/TENDER/TenderSupplies/AddTaskTender.C");
      // IF V0 tender needed, put kTRUE below
      AliAnalysisTaskSE *tender = AddTaskTender(useV0tender);
//      tender->SetDebugLevel(2);
   }
 
   if (usePhysicsSelection) {
   // Physics selection task
      gROOT->LoadMacro("$ALICE_PHYSICS/OADB/macros/AddTaskPhysicsSelection.C");
      mgr->RegisterExtraFile("event_stat.root");
      AliPhysicsSelectionTask *physSelTask = AddTaskPhysicsSelection(useMC);
//      AliOADBPhysicsSelection * oadbDefaultPbPb = CreateOADBphysicsSelection();      
//      physSelTask->GetPhysicsSelection()->SetCustomOADBObjects(oadbDefaultPbPb,0,0);
      mgr->AddStatisticsTask(AliVEvent::kAny);
    }
   
   // Centrality (only Pb-Pb)
   if (useCentrality) 
   {
      //gROOT->LoadMacro("$ALICE_PHYSICS/OADB/macros/AddTaskCentrality.C");
      //AliCentralitySelectionTask *taskCentrality = AddTaskCentrality();
      //taskCentrality->SetMCInput();
      //taskCentrality->SelectCollisionCandidates(AliVEvent::kAny);
     gROOT->LoadMacro("$ALICE_PHYSICS/OADB/COMMON/MULTIPLICITY/macros/AddTaskMultSelection.C");
     AliMultSelectionTask *mult = AddTaskMultSelection();
   }

  //
  // PIDqa(JENS)
  //
  if (doPIDqa) {
    gROOT->LoadMacro("$ALICE_ROOT/ANALYSIS/macros/AddTaskPIDqa.C");
    AliAnalysisTaskPIDqa *PIDQA = AddTaskPIDqa();
    PIDQA->SelectCollisionCandidates(AliVEvent::kAny);
  }  

//Jacek
   if (iPWGPP) {
      gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/macros/AddTaskFilteredTree.C");
      AddTaskFilteredTree("FilterEvents_Trees.root");
   }   

// Muon refit
  if (iMUONRefit) {
    gROOT->LoadMacro("$ALICE_PHYSICS/PWG/muondep/AddTaskMuonRefit.C");
    AliAnalysisTaskMuonRefit* refit = AddTaskMuonRefit(-1., -1., kTRUE, -1., -1.);
    
    refit->ReAlign("", 6, -1, "");
    //refit->ReAlign("", 5, -1, "");
    
    refit->RemoveMonoCathodClusters(kTRUE, kFALSE);
  }
   
// --- PWGLF - Forward (cholm@nbi.dk) -----------------------------
   if (iPWGLFForward && usePhysicsSelection) { 
        gROOT->LoadMacro("$ALICE_PHYSICS/PWGLF/FORWARD/analysis2/AddTaskForwardMult.C");
     UShort_t pwglfForwardSys = 0; // iCollision+1; // pp:1, PbPb:2, pPb:3
     UShort_t pwglfSNN        = 0;            // GeV, 0==unknown
     Short_t  pwglfField      = 0;
     AddTaskForwardMult(useMC && useTR,        // Need track-refs 
			pwglfForwardSys,       // Collision system
			pwglfSNN, 
			pwglfField);
        gROOT->LoadMacro("$ALICE_PHYSICS/PWGLF/FORWARD/analysis2/AddTaskCentralMult.C");
        AddTaskCentralMult(useMC, pwglfForwardSys, pwglfSNN, pwglfField);
   }

   if (iPWGGAgammaconv) 
   {
      gROOT->LoadMacro("$ALICE_PHYSICS/PWGGA/GammaConv/macros/AddTask_ConversionAODProduction.C");
      AliAnalysisTask *taskconv = 
                AddTask_ConversionAODProduction(iCollision, 
                                                kTRUE,         // isMC
                                                periodName);

      mgr->RegisterExtraFile("AliAODGammaConversion.root");
   }
   
   if (iESDfilter && !iAODanalysis) {
      //  ESD filter task configuration.
      gROOT->LoadMacro("$ALICE_ROOT/ANALYSIS/ESDfilter/macros/AddTaskESDFilter.C");
      if (iMUONcopyAOD)
      {
         printf("Registering delta AOD file\n");
         mgr->RegisterExtraFile("AliAOD.Muons.root");
         mgr->RegisterExtraFile("AliAOD.Dimuons.root");
      }
      
      Bool_t muonWithSPDTracklets = (iCollision==1) ? kFALSE : kTRUE; // add SPD information to muon AOD only for pp
      
      AliAnalysisTaskESDfilter *taskesdfilter = 
                 AddTaskESDFilter(useKFILTER, 
                                  iMUONcopyAOD,         // write Muon AOD
                                  kFALSE,               // write dimuon AOD 
                                  kFALSE,               // usePhysicsSelection 
                                  kFALSE,               // centrality OBSOLETE
                                  kTRUE,                // enable TPS only tracks
                                  kFALSE,               // disable cascades
                                  kFALSE,               // disable kinks
                                  run_flag,             // run flag (YY00)
                                  3,                    // muonMCMode
                                  kFALSE,               // useV0Filter 
                                  muonWithSPDTracklets, // add SPD information to muon AOD only for pp
                                  isMuonCaloPass);	//
                 
      AliEMCALGeometry::GetInstance("","");
   }

// ********** PWG3 wagons ******************************************************
   // PWG3 vertexing
   // PWG3 vertexing
   if (iPWGHFvertexing) {
      gROOT->LoadMacro("$ALICE_PHYSICS/PWGHF/vertexingHF/macros/AddTaskVertexingHF.C");
      AliAnalysisTaskSEVertexingHF *taskvertexingHF = 
        AddTaskVertexingHF(iCollision,train_name,"",run_number,periodName);
      if (!taskvertexingHF) ::Warning("AnalysisTrainNew", "AliAnalysisTaskSEVertexingHF cannot run for this train conditions - EXCLUDED");
      else mgr->RegisterExtraFile("AliAOD.VertexingHF.root");
      taskvertexingHF->SelectCollisionCandidates(0);
   }   
      
   // PWG3 JPSI filtering (only pp)
   if (iPWGDQJPSIfilter && (iCollision==0)) {
      gROOT->LoadMacro("$ALICE_PHYSICS/PWGDQ/dielectron/macros/AddTaskJPSIFilter.C");
      AliAnalysisTaskSE *taskJPSIfilter = AddTaskJPSIFilter();
      if (!taskJPSIfilter) ::Warning("AnalysisTrainNew", "AliAnalysisTaskDielectronFilter cannot run for this train conditions - EXCLUDED");
      else mgr->RegisterExtraFile("AliAOD.Dielectron.root");
      taskJPSIfilter->SelectCollisionCandidates(0);
   }   

   // PWG3 D2h
   if (iPWGHFd2h) {   
     gROOT->LoadMacro("$ALICE_PHYSICS/PWGHF/vertexingHF/AddD2HTrain.C");
     //TFile::Cp(gSystem->ExpandPathName(configPWGHFd2h.Data()), "file:ConfigVertexingHF.C");
     //AddD2HTrain(kFALSE, 1,0,0,0,0,0,0,0,0,0,0);
     AddD2HTrain(kTRUE, 1,0,0,0,0,0,0,0,0,0,0);
   }
         
   // ********** PWG4 wagons ******************************************************
   // Jet analysis
   // Configurations flags, move up?
   TString kDeltaAODJetName = "AliAOD.Jets.root"; //
   Bool_t  kIsPbPb = (iCollision==0)?false:true; // can be more intlligent checking the name of the data set
   TString kDefaultJetBackgroundBranch = "";
   TString kJetSubtractBranches = "";
   UInt_t kHighPtFilterMask = 768;// from esd filter
   UInt_t iPhysicsSelectionFlag = 0;
   if (iJETAN) {
     gROOT->LoadMacro("$ALICE_PHYSICS/PWGJE/macros/AddTaskJets.C");
     // Default jet reconstructor running on ESD's
     AliAnalysisTaskJets *taskjets = AddTaskJets("AOD","UA1",0.4,kHighPtFilterMask,1.,0); // no background subtraction     
     if (!taskjets) ::Fatal("AnalysisTrainNew", "AliAnalysisTaskJets cannot run for this train conditions - EXCLUDED");
     if(kDeltaAODJetName.Length()>0) taskjets->SetNonStdOutputFile(kDeltaAODJetName.Data());
     if (iJETANdelta) {
        //            AddTaskJetsDelta("AliAOD.Jets.root"); // need to modify this accordingly in the add task jets
        mgr->RegisterExtraFile(kDeltaAODJetName.Data());
        TString cTmp("");
        if(kIsPbPb){
          // UA1 intrinsic background subtraction
          taskjets = AddTaskJets("AOD","UA1",0.4,kHighPtFilterMask,1.,2); // background subtraction
          if(kDeltaAODJetName.Length()>0)taskjets->SetNonStdOutputFile(kDeltaAODJetName.Data());
       }
       // SICONE 
       taskjets = AddTaskJets("AOD","SISCONE",0.4,kHighPtFilterMask,0.15,0); //no background subtration to be done later....                                                                                  
       if(kDeltaAODJetName.Length()>0)taskjets->SetNonStdOutputFile(kDeltaAODJetName.Data());
       cTmp = taskjets->GetNonStdBranch();
       if(cTmp.Length()>0)kJetSubtractBranches += Form("%s ",cTmp.Data());
	 
       // Add the clusters..
       gROOT->LoadMacro("$ALICE_PHYSICS/PWGJE/macros/AddTaskJetCluster.C");
       AliAnalysisTaskJetCluster *taskCl = 0;
       Float_t fCenUp = 0;
       Float_t fCenLo = 0;
       Float_t fTrackEtaWindow = 0.9;
       taskCl = AddTaskJetCluster("AOD","",kHighPtFilterMask,iPhysicsSelectionFlag,"KT",0.4,0,1, kDeltaAODJetName.Data(),0.15,fTrackEtaWindow,0); // this one is for the background and random jets, random cones with no skip                                                                                 
       taskCl->SetBackgroundCalc(kTRUE);
       taskCl->SetNRandomCones(10);
       taskCl->SetCentralityCut(fCenLo,fCenUp);
       taskCl->SetGhostEtamax(fTrackEtaWindow);
       kDefaultJetBackgroundBranch = Form("%s_%s",AliAODJetEventBackground::StdBranchName(),taskCl->GetJetOutputBranch());

       taskCl = AddTaskJetCluster("AOD","",kHighPtFilterMask,iPhysicsSelectionFlag,"ANTIKT",0.4,2,1,kDeltaAODJetName.Data(),0.15);
       taskCl->SetCentralityCut(fCenLo,fCenUp);
       if(kIsPbPb)taskCl->SetBackgroundBranch(kDefaultJetBackgroundBranch.Data());
       taskCl->SetNRandomCones(10);
       kJetSubtractBranches += Form("%s ",taskCl->GetJetOutputBranch());

       taskCl = AddTaskJetCluster("AOD","",kHighPtFilterMask,iPhysicsSelectionFlag,"ANTIKT",0.2,0,1,kDeltaAODJetName.Data(),0.15);
       taskCl->SetCentralityCut(fCenLo,fCenUp);
       if(kIsPbPb)taskCl->SetBackgroundBranch(kDefaultJetBackgroundBranch.Data());
       kJetSubtractBranches += Form("%s ",taskCl->GetJetOutputBranch());
	 
       // DO THE BACKGROUND SUBTRACTION
       if(kIsPbPb&&kJetSubtractBranches.Length()){
         gROOT->LoadMacro("$ALICE_PHYSICS/PWGJE/macros/AddTaskJetBackgroundSubtract.C");
         AliAnalysisTaskJetBackgroundSubtract *taskSubtract = 0;
         taskSubtract = AddTaskJetBackgroundSubtract(kJetSubtractBranches,1,"B0","B%d");
         taskSubtract->SetBackgroundBranch(kDefaultJetBackgroundBranch.Data());
         if(kDeltaAODJetName.Length()>0)taskSubtract->SetNonStdOutputFile(kDeltaAODJetName.Data());
       }
     } 
   }
}

//______________________________________________________________________________
void StartAnalysis(const char *mode, TChain *chain) {
// Start analysis.
   Int_t imode = -1;
   AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
   if (!strcmp(mode, "LOCAL")) imode = 0;
   if (!strcmp(mode, "GRID"))  imode = 1;
   switch (imode) {
      case 0:
         if (!chain) {
            ::Error("AnalysisTrainNew.C::StartAnalysis", "Cannot create the chain");
            return;
         }   
         mgr->StartAnalysis(mode, chain);
         break;
      case 1:
         if (!mgr->GetGridHandler()) {
            ::Error("AnalysisTrainNew.C::StartAnalysis", "Grid plugin not initialized");
            return;
         }   
         mgr->StartAnalysis("grid");
   }
}          
    
//______________________________________________________________________________
Bool_t Connect(const char *mode) {
// Connect <username> to the back-end system.
   Int_t imode = -1;
   if (!strcmp(mode, "LOCAL")) imode = 0;
   if (!strcmp(mode, "GRID"))  imode = 1;
   TString username = gSystem->Getenv("alien_API_USER");
   switch (imode) {
      case 0:
         break;
      case 1:
         ::Info("AnalysisTrainNew.C::Connect", "Connecting user <%s> to AliEn ...", 
                username.Data());
         TGrid::Connect("alien://");
         if (!gGrid || !gGrid->IsConnected()) return kFALSE;
         break;
      default:
         ::Error("AnalysisTrainNew.C::Connect", "Unknown run mode: %s", mode);
         return kFALSE;
   }
   ::Info("AnalysisTrainNew.C::Connect","Connected in %s mode", mode);
   return kTRUE;
}

//______________________________________________________________________________
TChain *CreateChain(const char *mode, const char *plugin_mode)
{
// Create the input chain
   Int_t imode = -1;
   if (!strcmp(mode, "LOCAL")) imode = 0;
   if (!strcmp(mode, "GRID"))  imode = 1;
   TChain *chain = NULL;
   // Local chain
   switch (imode) {
      case 0:
         if (iAODanalysis) {
            if (!local_xmldataset.Length()) {
               // Local AOD
               chain = new TChain("aodTree");
               if (gSystem->AccessPathName("data/AliAOD.root")) 
                  ::Error("AnalysisTrainNew.C::CreateChain", "File: AliAOD.root not in ./data dir");
               else {
                  if (!saveTrain) chain->Add("data/AliAOD.root");
                  else            chain->Add("../data/AliAOD.root");
               }   
            } else {
               // Interactive AOD
               chain = CreateChainSingle(local_xmldataset, "aodTree");
            }
         } else {      
            if (!local_xmldataset.Length()) {
               // Local ESD
               chain = new TChain("esdTree");
               if (gSystem->AccessPathName("data/AliESDs.root")) 
                  ::Error("AnalysisTrainNew.C::CreateChain", "File: AliESDs.root not in ./data dir");
               else {
                  if (!saveTrain) chain->Add("data/AliESDs.root");
                  else            chain->Add("../data/AliESDs.root");
               }   
            } else {
               // Interactive ESD
               chain = CreateChainSingle(local_xmldataset, "esdTree");
            }   
         }
         break;
      case 1:
         break;      
      default:   
   }
   if (chain && chain->GetNtrees()) return chain;
   return NULL;
}   

//______________________________________________________________________________
TChain* CreateChainSingle(const char* xmlfile, const char *treeName)
{
   printf("*******************************\n");
   printf("*** Getting the ESD Chain   ***\n");
   printf("*******************************\n");
   TAlienCollection * myCollection  = TAlienCollection::Open(xmlfile);

   if (!myCollection) {
      ::Error("AnalysisTrainNew.C::CreateChainSingle", "Cannot create an AliEn collection from %s", xmlfile) ;
      return NULL ;
   }

   TChain* chain = new TChain(treeName);
   myCollection->Reset() ;
   while ( myCollection->Next() ) chain->Add(myCollection->GetTURL("")) ;
   chain->ls();
   return chain;
}

//______________________________________________________________________________
AliAnalysisAlien* CreateAlienHandler(const char *plugin_mode)
{
// Check if user has a valid token, otherwise make one. This has limitations.
// One can always follow the standard procedure of calling alien-token-init then
   AliAnalysisAlien *plugin = new AliAnalysisAlien();
// Set the run mode (can be "full", "test", "offline", "submit" or "terminate")
   plugin->SetRunMode(plugin_mode);
   if (useProductionMode) {
      plugin->SetProductionMode();
      plugin->AddDataFile(data_collection);
   }   
      
   if (!outputSingleFolder.IsNull()) {
      plugin->SetOutputSingleFolder(outputSingleFolder);
      plugin->SetOutputToRunNo();
   }   
   plugin->SetJobTag(job_tag);
   plugin->SetNtestFiles(nTestFiles);
   plugin->SetCheckCopy(kFALSE);
   plugin->SetMergeDirName(mergeDirName);
// Set versions of used packages
   //***plugin->SetAPIVersion("V1.1x");
   //***plugin->SetROOTVersion(root_version);
   //***plugin->SetAliROOTVersion(aliroot_version);
   plugin->SetAliPhysicsVersion(aliphysics_version);
// Declare input data to be processed.
// Method 1: Create automatically XML collections using alien 'find' command.
// Define production directory LFN
   plugin->SetGridDataDir(alien_datadir);
// Set data search pattern
   plugin->SetDataPattern(data_pattern);
   if (!useProductionMode) {
      if (runOnData) {
         plugin->SetRunPrefix("000");
      }   
//   if (!iAODanalysis) plugin->SetRunRange(run_range[0], run_range[1]);
      for (Int_t i=0; i<10; i++) {
         if (run_numbers[i]==0) break;
         plugin->AddRunNumber(run_numbers[i]);
      }   
   }   
// Define alien work directory where all files will be copied. Relative to alien $HOME.
   plugin->SetGridWorkingDir(grid_workdir);
// Declare alien output directory. Relative to working directory.
   if (alien_outdir.IsNull()) alien_outdir = Form("output_%s",train_name.Data());
   plugin->SetGridOutputDir(alien_outdir);

// Add external packages
   if (iJETAN || iJETANdelta) {
      plugin->AddExternalPackage("boost::v1_43_0");
      plugin->AddExternalPackage("cgal::v3.6");
      plugin->AddExternalPackage("fastjet::v2.4.2");
   }   
   
// Declare the output file names separated by blancs.
// (can be like: file.root or file.root@ALICE::Niham::File)
   plugin->SetDefaultOutputs();
   plugin->SetMergeExcludes(mergeExclude);
   plugin->SetMaxMergeFiles(maxMergeFiles);
   plugin->SetNrunsPerMaster(nRunsPerMaster);
// Optionally define the files to be archived.
//   plugin->SetOutputArchive("log_archive.zip:stdout,stderr@ALICE::NIHAM::File root_archive.zip:AliAOD.root,AOD.tag.root@ALICE::NIHAM::File");
   
   
   // Put default output files to archive
   TString listhists = "";
   TString listaods  = "";
   AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
   TIter next(mgr->GetOutputs());
   AliAnalysisDataContainer *output;
   while ((output=(AliAnalysisDataContainer*)next())) {
      const char *filename = output->GetFileName();
      if (!(strcmp(filename, "default"))) {
         if (!mgr->GetOutputEventHandler()) continue;
         filename = mgr->GetOutputEventHandler()->GetOutputFileName();
         if (listaods.Length()) listaods += ",";
         listaods += filename;
         listaods += ",";
         listaods += "pyxsec_hists.root";
      } else {
         if (!strcmp(filename, "pyxsec_hists.root")) continue;
         if (listhists.Contains(filename)) continue;
         if (listhists.Length()) listhists += ",";
         listhists += filename;
      }
   }
   if (mgr->GetExtraFiles().Length()) {
      if (listaods.Length()) listaods += ",";
      listaods += mgr->GetExtraFiles();
      listaods.ReplaceAll(" ", ",");
   }
   if (listhists.Length()) listhists = Form("hist_archive.zip:%s@%s", listhists.Data(), outputStorages.Data());
   if (listaods.Length())  listaods  = Form("aod_archive.zip:%s@%s", listaods.Data(), outputStorages.Data());
   if (!listhists.Length() && !listaods.Length()) {
      ::Fatal("AnalysisTrainNew", "No task output !");
   }
   TString outputArchive = Form("log_archive.zip:stderr@%s", outputStorages.Data());
   if (listaods.Length()) {
      outputArchive += " ";
      outputArchive += listaods;
   }   
   if (listhists.Length()) {
      outputArchive += " ";
      outputArchive += listhists;
   }   
   
// Set friends
//   if (iAODanalysis && iPWG3d2h) 
//      plugin->SetFriendChainName("AliAOD.VertexingHF.root");
//   plugin->SetOutputArchive(outputArchive);
// Optionally set a name for the generated analysis macro (default MyAnalysis.C)
   plugin->SetAnalysisMacro(Form("%s.C", train_name.Data()));
// Optionally set a name for the generated validation script
   plugin->SetValidationScript("FILTERvalidation.sh");
// Optionally set maximum number of input files/subjob (default 100, put 0 to ignore)
   plugin->SetSplitMaxInputFileNumber(nFilesPerJob);
// Optionally set number of failed jobs that will trigger killing waiting sub-jobs.
//   plugin->SetMaxInitFailed(5);
// Optionally modify the number of replicas
   plugin->SetNumberOfReplicas(outputReplicas);
// Optionally resubmit threshold.
//   plugin->SetMasterResubmitThreshold(90);
// Optionally set time to live (default 30000 sec)
   plugin->SetTTL(70000);
// Optionally set input format (default xml-single)
   plugin->SetInputFormat("xml-single");
// Optionally modify the name of the generated JDL (default analysis.jdl)
   plugin->SetJDLName(Form("%s.jdl", train_name.Data()));
// Optionally modify the executable name (default analysis.sh)
   plugin->SetExecutable(Form("%s.sh", train_name.Data()));
// Optionally modify job price (default 1)
   plugin->SetPrice(1);      
// Merge via JDL
   plugin->SetMergeViaJDL(useMergeViaJDL);
// Use fastread option
   plugin->SetFastReadOption(useFastReadOption);
// UseOverwrite mode
   plugin->SetOverwriteMode(useOverwriteMode);   
   plugin->SetExecutableCommand("aliroot -b -q");
// Optionally modify split mode (default 'se')    
   plugin->SetSplitMode("se");
   plugin->SetNumberOfReplicas(outputReplicas);
   return plugin;
}

//______________________________________________________________________________
void WriteConfig()
{
// Write train configuration in a file. The file name has the format:
// train_[trainName]_ddMonthyyyy_time.C
   if (useDATE) {
      gSystem->Exec("date +%d%b%Y_%Hh%M > date.tmp");
      ifstream fdate("date.tmp");
      if (!fdate.is_open()) {
         ::Error("AnalysisTrainNew.C::Export","Could not generate file name");
         return;
      }
      const char date[64];
      fdate.getline(date,64);
      fdate.close();
      gSystem->Exec("rm date.tmp");
      train_name = Form("%s_%s", train_name.Data(), date);
   }   
   TString cdir = gSystem->WorkingDirectory();
   gSystem->MakeDirectory(train_name);
   gSystem->ChangeDirectory(train_name);
   ofstream out;
   out.open(Form("%sConfig.C",train_name.Data()), ios::out); 
   if (out.bad()) {
      ::Error("AnalysisTrainNew.C::Export", "Cannot open ConfigTrain.C for writing");
      return;
   }
   out << "{" << endl;
   out << "   train_name      = " << "\"" << train_name.Data() << "\";" << endl;
   //***out << "   root_version    = " << "\"" << root_version.Data() << "\";" << endl;
   //***out << "   aliroot_version = " << "\"" << aliroot_version.Data() << "\";" << endl;
   out << "   alien_datadir   = " << "\"" << alien_datadir.Data() << "\";" << endl;
   if (!alien_outdir.Length()) alien_outdir = Form("output_%s",train_name.Data());
   out << "   alien_outdir    = " << "\"" << alien_outdir.Data() << "\";" << endl;
   out << "   maxMergeFiles   = " << maxMergeFiles << ";" << endl;
   out << "   mergeExclude    = " << "\"" << mergeExclude.Data() << "\";" << endl;
   out << "   nRunsPerMaster  = " << nRunsPerMaster << ";" << endl;
   out << "   nFilesPerJob    = " << nFilesPerJob << ";" << endl;
//   for (Int_t i=0; i<10; i++) {
//      if (run_numbers[i]) 
//         out << "   run_numbers[" << i << "]  = " << run_numbers[i] << ";" << endl;
//   }
//   out << "   run_range[0]    = " << run_range[0] << ";" << endl;
//   out << "   run_range[1]    = " << run_range[1] << ";" << endl;
   out << "   usePhysicsSelection = " << usePhysicsSelection << ";" << endl;
   out << "   useTender       = " << useTender << ";" << endl;
   out << "   useMergeViaJDL  = " << useMergeViaJDL << ";" << endl;
   out << "   useOverwriteMode  = " << useOverwriteMode << ";" << endl;
   out << "   useFastReadOption = " << useFastReadOption << ";" << endl;
   out << "   useDBG          = " << useDBG << ";" << endl;
   out << "   useMC           = " << useMC << ";" << endl;
   out << "   useKFILTER      = " << useKFILTER << ";" << endl;
   out << "   useTR           = " << useTR << ";" << endl;
   out << "   saveTrain       = " << "kFALSE;" << endl << endl;
   out << "   // Analysis modules" << endl;
   out << "   iAODanalysis    = " << iAODanalysis << ";" << endl;
   out << "   iAODhandler     = " << iAODhandler << ";" << endl;
   out << "   iESDfilter      = " << iESDfilter << ";" << endl;
   out << "   iMUONcopyAOD    = " << iMUONcopyAOD << ";" << endl;
   out << "   iJETAN          = " << iJETAN << ";" << endl;
   out << "   iJETANdelta     = " << iJETANdelta << ";" << endl;
   out << "   iPWGHFvertexing  = " << iPWGHFvertexing << ";" << endl;   
   out << "   iPWGLFForward    = " << iPWGLFForward << ";" << endl;
   out << endl;
   out << "   iCollision      = \"" << CollisionSystem[iCollision] << "\";" << endl;
   out << "   periodName      = \"" << periodName.Data() << "\";" << endl;
   out << "   run_number      = " << run_number << ";" << endl;
   out << "   run_flag        = " << run_flag << ";" << endl;
   out << "   isMuonCaloPass  = " << isMuonCaloPass << ";" << endl;
   out << "   useCentrality   = " << useCentrality << ";" << endl;
   out << "// Configuration for the wagons" << endl;
   out << "}" << endl;
   ::Info("AnalysisTrainNew.C::WriteConfig", "Train configuration wrote to file %s", Form("config_%s.C", train_name.Data()));
   gSystem->ChangeDirectory(cdir);
}   

//______________________________________________________________________________
Bool_t LoadConfig(const char *filename)
{
// Read train configuration from file
   if (gSystem->AccessPathName(filename)) {
      ::Error("AnalysisTrainNew.C::LoadConfig", "Config file name not found");
      return kFALSE;
   }   
   gROOT->ProcessLine(Form(".x %s", filename));
   ::Info("AnalysisTrainNew.C::LoadConfig", "Train configuration loaded from file %s", filename);
   return kTRUE;
}

//______________________________________________________________________________
void ProcessEnvironment()
{
  //
  // Collision system configuration
  //
  if(gSystem->Getenv("ALIEN_JDL_LPMINTERACTIONTYPE"))
  {
    for (Int_t icoll = 0; icoll < kNSystem; icoll++)
      if (strcmp(gSystem->Getenv("ALIEN_JDL_LPMINTERACTIONTYPE"), CollisionSystem[icoll]) == 0) 
      {
        iCollision = icoll;

        if(icoll == kpA)
            iCollision =kpp;
      }

      if(iCollision == kPbPb)
      {
        useCentrality =kTRUE;
        train_tag = "_Pb-Pb_";
      }
      else
        train_tag = "_p-p_";
  }
  else
    if(!localRunning)
    {
      printf(">>>>> Unknown collision system configuration ALIEN_JDL_LPMINTERACTIONTYPE \n");
      abort();
    }

  //
  // Period name
  //
  if(gSystem->Getenv("ALIEN_JDL_LPMPRODUCTIONTAG"))
    periodName = gSystem->Getenv("ALIEN_JDL_LPMPRODUCTIONTAG");
  else
    if(!localRunning)
    {
      printf(">>>>> Unknown production tag configuration ALIEN_JDL_LPMPRODUCTIONTAG \n");
      abort();
    }

  //
  // Run flag configuration
  //
  if(gSystem->Getenv("ALIEN_JDL_LPMANCHORYEAR"))
  {
    Int_t year = atoi(gSystem->Getenv("ALIEN_JDL_LPMANCHORYEAR"));

    if(year<2015)
      run_flag =1100;
    else
      run_flag =1500;
    if(year<=2010) 
    {
      run_flag =1000;
      if(periodName.EqualTo("LHC10h"))
        run_flag = 1001;
    }
  }
  else
    if(!localRunning)
    {
      printf(">>>>> Unknown anchor year system configuration ALIEN_JDL_LPMANCHORYEAR \n");
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
      printf(">>>>> Unknown run number ALIEN_JDL_LPMRUNNUMBER \n");
      abort();
    }
  if (run_number <= 0)
    printf(">>>>> Invalid run number: %d \n", run_number);

  //
  // Setting this to kTRUE will disable some not needed analysis tasks for a muon_calo pass
  //
  if (gSystem->Getenv("ALIEN_JDL_MCPRODTYPE"))
  {
    TString passType = gSystem->Getenv("ALIEN_JDL_MCPRODTYPE");
    if (passType.Contains("muon_calo"))
      isMuonCaloPass = kTRUE;
    else
      isMuonCaloPass = kFALSE;
  }
  else
    if(!localRunning)
    {
      printf(">>>>> Unknown production type ALIEN_JDL_MCPRODTYPE \n");
      abort();
    }
}

//________________________________________________________
void PrintSettings()
{
  printf("\n   **********************************\n");
  printf("   * \n");
  printf("   * System:         %s\n", CollisionSystem[iCollision]);
  printf("   * Period name:    %s\n", periodName.Data());
  printf("   * Run number:     %d\n", run_number);
  printf("   * Run flag:       %d\n", run_flag);
  printf("   * Muon calo pass: %d\n", isMuonCaloPass);
  printf("   * Centrality:     %d\n", useCentrality);
  printf("   * \n");
  printf("   **********************************\n\n");
}
