/*****************************************
When running in local mode, you need
to write a file containing, for example:

export ALIEN_JDL_LPMINTERACTIONTYPE=pp
export ALIEN_JDL_LPMANCHORYEAR=2015
export ALIEN_JDL_LPMPRODUCTIONTAG=LHC15n
export ALIEN_JDL_LPMRUNNUMBER=244628
export ALIEN_JDL_RAWPRODTYPE=PPass

then source the same file
******************************************/


// ### Settings that make sense when using the Alien plugin
//==============================================================================
Int_t       iCollision          = 0;       // 0=pp, 1=Pb-Pb
Int_t       run_flag            = 1500;    // year (1000, 2010 pp; 1100, 2011 pp; 1500, 2015)
Int_t       year                = 2015;
TString     periodName          = "LHC15n";
Int_t       run_number          = 0;
Bool_t      localRunning        = kFALSE;  // Missing environment vars will cause a crash; change it to kTRUE if running locally w/o env vars

//==============================================================================
Bool_t      doCDBconnect        = kTRUE;
Bool_t      usePhysicsSelection = kTRUE;   // use physics selection
Bool_t      useTender           = kFALSE;  // use tender wagon
Bool_t      useCentrality       = kFALSE;  // centrality
Bool_t      useV0tender         = kFALSE;  // use V0 correction in tender
Bool_t      useDBG              = kFALSE;  // activate debugging
Bool_t      useMC               = kFALSE;  // use MC info
Bool_t      useKFILTER          = kFALSE;  // use Kinematics filter
Bool_t      useTR               = kFALSE;  // use track references
Bool_t      useCORRFW           = kFALSE;  // do not change
Bool_t      useAODTAGS          = kFALSE;  // use AOD tags
Bool_t      useSysInfo          = kTRUE;   // use sys info
Bool_t      isMuonCaloPass      = kFALSE;  // setting this to kTRUE will disable some not needed analysis tasks for a muon_calo pass
Bool_t      isMuonOnly          = kFALSE;  // setting this to kTRUE will disable same tasks as isMuonCaloPass and some more

// ### Analysis modules to be included. Some may not be yet fully implemented.
//==============================================================================
Int_t       iAODhandler         = 1;       // Analysis produces an AOD or dAOD's
Int_t       iESDfilter          = 1;       // ESD to AOD filter (barrel + muon tracks)
Int_t       iMUONcopyAOD        = 1;       // Task that copies only muon events in a separate AOD (PWG3)
Int_t       iJETAN              = 0;       // Jet analysis (PWG4)
Int_t       iJETANdelta         = 0;       // Jet delta AODs
Int_t       iPWGHFvertexing     = 1;       // Vertexing HF task (PWG3)
Int_t       iPWGDQJPSIfilter    = 0;       // JPSI filtering (PWG3)
Int_t       iPWGPP              = 1;       // high pt filter task
Int_t       iPWGLFForward       = 1;       // Forward mult task (PWGLF)
Int_t       iPWGGAgammaconv     = 1;       // Gamma conversion analysis (PWG4)
Bool_t      doPIDResponse       = kTRUE;
Bool_t      doPIDqa             = kTRUE;

// ### Configuration macros used for each module
//==============================================================================
// TString configPWGHFd2h = (iCollision==0)?"$ALICE_PHYSICS/PWGHF/vertexingHF/ConfigVertexingHF.C"
//                          :"$ALICE_PHYSICS/PWGHF/vertexingHF/ConfigVertexingHF_Pb_AllCent_NoLS_PIDLc_PtDepSel_LooseIP.C";

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

const Char_t* CollisionSystemMC[kNSystem] =
  {
    "p-p",
    "Pb-Pb",
    "p-Pb",
    "Pb-p",
    "Xe-Xe"
  };

// Temporaries.
void ProcessEnvironment();
void ProcessEnvironmentMC();
void PrintSettings();
void AODmerge();
void AddAnalysisTasks(const char *cdb_location, Bool_t isMC);
Bool_t LoadCommonLibraries();
Bool_t LoadAnalysisLibraries();
Bool_t LoadLibrary(const char *);
TChain *CreateChain();
const char *cdbPath = "raw://";
TString train_name = ".";


//______________________________________________________________________________
void UpdateFlags()
{
  // Update flags according to type of pass
  if(iCollision == kPbPb || iCollision == kXeXe)
    useCentrality =kTRUE;

  if ( isMuonCaloPass || isMuonOnly )
    {
      // disable the analysis we know for sure can not work or are meaningless
      // for a muon_calo pass
      doCDBconnect       = kFALSE;
      useTender          = kFALSE;
      useV0tender        = kFALSE;
      iJETAN             = 0;
      iJETANdelta        = 0;
      iPWGHFvertexing    = 0;
      iPWGDQJPSIfilter   = 0;
      iPWGPP             = 0; 
      iPWGLFForward      = 0; 
      iPWGGAgammaconv    = 0; 
      doPIDResponse      = kFALSE;
      doPIDqa            = kFALSE;
    }
  if ( isMuonOnly )
    {
      // disable more unnecessary tasks for muon only MC
      usePhysicsSelection = kFALSE;
      useCentrality       = kFALSE;
    }
}

//______________________________________________________________________________
void main_AODtrainRawAndMC(Int_t merge=0, Bool_t isMC=kFALSE)
{
  // Main analysis train macro.

  // detect from envrionment variable if data or MC
  if (gSystem->Getenv("ALIEN_JDL_LPMPRODUCTIONTYPE")) {
    TString prodType = gSystem->Getenv("ALIEN_JDL_LPMPRODUCTIONTYPE");
    if (prodType=="MC"){
      isMC = kTRUE;
      printf("Run AOD train for MC (based on ALIEN_JDL_LPMPRODUCTIONTYPE)\n");
    }else if (prodType=="RAW"){
      isMC = kFALSE;
      printf("Run AOD train for RAW (based on ALIEN_JDL_LPMPRODUCTIONTYPE)\n");
    }
  }else{
    if(isMC) printf("Run AOD train for MC (based on macro argument)\n");
    else printf("Run AOD train for RAW (based on macro argument)\n");
  }

  if(isMC){
    useDBG=kTRUE;
    useMC=kTRUE;
    useKFILTER=kTRUE;
    useTR=kTRUE;
    useSysInfo=kFALSE;
    ProcessEnvironmentMC();
    // set OCDB source
    TString ocdbConfig = "default,snapshot";
    if (gSystem->Getenv("CONFIG_OCDB"))
      ocdbConfig = gSystem->Getenv("CONFIG_OCDB");
    if (merge != 0) {
      //
      gSystem->Setenv("CONFIG_RUN", gSystem->Getenv("ALIEN_JDL_LPMRUNNUMBER"));
      // set OCDB 
      gInterpreter->ProcessLine("OCDBDefault(1);");    
    }
    else if (ocdbConfig.Contains("alien") || ocdbConfig.Contains("cvmfs")) {
      // set OCDB 
      gInterpreter->ProcessLine("OCDBDefault(1);");  
    }
    else {
      // set OCDB snapshot mode
      AliCDBManager *cdbm = AliCDBManager::Instance();
      cdbm->SetSnapshotMode("OCDBrec.root");
    }
  }else{
    ProcessEnvironment();
  }

  UpdateFlags();

  PrintSettings();

  Bool_t needGrid=kFALSE;
  if(merge || doCDBconnect) needGrid=kTRUE;
  if(gSystem->Getenv("OCDB_PATH")) needGrid=kFALSE;

  if (needGrid) {
    TGrid::Connect("alien://");
    if (!gGrid || !gGrid->IsConnected()) {
      ::Error("AODtrainRawAndMC", "No grid connection");
      return;
    }
  }
  // Set temporary merging directory to current one
  gSystem->Setenv("TMPDIR", gSystem->pwd());
  // Set temporary compilation directory to current one
  gSystem->SetBuildDir(gSystem->pwd(), kTRUE);
  printf("==================================================================\n");
  printf("===========    RUNNING FILTERING TRAIN   ==========\n");
  printf("===========    Collision System is %s    ==========\n",CollisionSystem[iCollision]);
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
  
  // Make the analysis manager and connect event handlers
  AliAnalysisManager *mgr  = new AliAnalysisManager("Analysis Train", "Production train");
  if(!isMC) mgr->SetCacheSize(0);
  if (useSysInfo) {
    //mgr->SetNSysInfo(100);
    AliSysInfo::SetVerbose(kTRUE);
  }   
  // Create input handler (input container created automatically)
  // ESD input handler
  AliESDInputHandler *esdHandler = new AliESDInputHandler();
  mgr->SetInputEventHandler(esdHandler);       
  // Monte Carlo handler
  if (useMC) {
    AliMCEventHandler* mcHandler = new AliMCEventHandler();
    mgr->SetMCtruthEventHandler(mcHandler);
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,0,0)
    mcHandler->SetPreReadMode(static_cast<AliMCEventHandler::PreReadMode_t>(1));
#else
    mcHandler->SetPreReadMode(1);
#endif
    mcHandler->SetReadTR(useTR); 
  }   
  // AOD output container, created automatically when setting an AOD handler
  if (iAODhandler) {
    // AOD output handler
    AliAODHandler* aodHandler   = new AliAODHandler();
    aodHandler->SetOutputFileName("AliAOD.root");
    mgr->SetOutputEventHandler(aodHandler);
  }
  // Debugging if needed
  if (useDBG) mgr->SetDebugLevel(3);

  AddAnalysisTasks(cdbPath,isMC);
  if (merge) {
    AODmerge();
    mgr->InitAnalysis();
    mgr->SetGridHandler(new AliAnalysisAlien);
    TTree *tree = NULL;
    mgr->StartAnalysis("gridterminate", tree);
    return;
  }   
  // Run the analysis                                                                                                                     
  //
  TChain *chain = CreateChain();
  if (!chain) return;
                                                                                                                                                   
  TStopwatch timer;
  timer.Start();
  mgr->SetSkipTerminate(kTRUE);
  if (mgr->InitAnalysis()) {
    mgr->PrintStatus();
    mgr->StartAnalysis("local", chain);
  }
  timer.Print();
}                                                                                                                                          
                                                                                                                                            
//______________________________________________________________________________                                                           
void AddAnalysisTasks(const char *cdb_location, Bool_t isMC)
{
  // Add all analysis task wagons to the train                                                                                               
  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();                                                                     

  //
  // Tender and supplies. Needs to be called for every event.
  //
  AliAnalysisManager::SetCommonFileName("AODQA.root");
  if (useTender) {
    // IF V0 tender needed, put kTRUE below
    AliAnalysisTaskSE *tender = AddTaskTender(useV0tender);
    //      tender->SetDebugLevel(2);
  }
   
  // Clean Geometry: Ruben
  //  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/CalibMacros/commonMacros/CleanGeom.C++");
  //  CleanGeom* clgmTask = new CleanGeom("cleanGeom");
  //  mgr->AddTask(clgmTask);
  //  AliAnalysisDataContainer *dummyInp = mgr->GetCommonInputContainer();
  //  if (dummyInp) mgr->ConnectInput(clgmTask,0,dummyInp);  
 
   
  //
  // PIDResponse(JENS)
  //
  if (doPIDResponse) {
    AliAnalysisTaskPIDResponse *PIDResponse = AddTaskPIDResponse(isMC);
    //    PIDResponse->SetUserDataRecoPass(1);
    //    PIDResponse->SelectCollisionCandidates(AliVEvent::kAny);
  }  
 
  //
  // PIDqa(JENS)
  //
  if (doPIDqa) {
    AliAnalysisTaskPIDqa *PIDQA = AddTaskPIDqa();
    PIDQA->SelectCollisionCandidates(AliVEvent::kAny);
  }  
  // CDB connection
  //
  if (doCDBconnect && !useTender) {
    //
    // Run number is necessary for latest PCM task, since AliPhysics v5-09-33-01-1
    //
    //AliTaskCDBconnect *taskCDB = 0x0;
    //if(isMC) taskCDB = AddTaskCDBconnect(cdb_location, run_number);
    //else taskCDB = AddTaskCDBconnect(cdb_location, 0 /*run_number*/);
    AliTaskCDBconnect *taskCDB = AddTaskCDBconnect(cdb_location, run_number);
    if (!taskCDB) return;
    AliCDBManager *cdb = AliCDBManager::Instance();
    cdb->SetDefaultStorage(cdb_location);
    //    taskCDB->SetRunNumber(run_number);
  }    
 
  if (usePhysicsSelection) {
    // Physics selection task
    mgr->RegisterExtraFile("event_stat.root");
    AliPhysicsSelectionTask *physSelTask = AddTaskPhysicsSelection(useMC);
    mgr->AddStatisticsTask(AliVEvent::kAny);
  }
   

  //Jacek
  if (iPWGPP) {
    AddTaskFilteredTree("FilterEvents_Trees.root");
  }   
   
  // Centrality 
  if (useCentrality) {
    if ( run_flag >= 1500 )
      {
        AliMultSelectionTask *taskMult = AddTaskMultSelection();
	if(isMC) taskMult->SetAlternateOADBforEstimators(periodName);
      }
    else
      {
        // old scheme is only valid for PbPb
        if ( iCollision == kPbPb )
	  {
	    AliCentralitySelectionTask *taskCentrality = AddTaskCentrality();
	    //taskCentrality->SelectCollisionCandidates(AliVEvent::kAny);
	    if(isMC) taskCentrality->SetMCInput();
	  }
      }
  }
   
  // --- PWGLF - Forward (cholm@nbi.dk) -----------------------------
  if (iPWGLFForward && usePhysicsSelection) { 
    UShort_t pwglfForwardSys = 0; // iCollision+1; // pp:1, PbPb:2, pPb:3
    UShort_t pwglfSNN        = 0;            // GeV, 0==unknown
    Short_t  pwglfField      = 0;
    AddTaskForwardMult(useMC && useTR,        // Need track-refs 
		       pwglfForwardSys,       // Collision system
		       pwglfSNN, 
		       pwglfField);
    AddTaskCentralMult(useMC, pwglfForwardSys, pwglfSNN, pwglfField);
  }

  //PWGAgammaconv
  if (iPWGGAgammaconv) {
    Int_t dataset=iCollision;
    if( iCollision == kXeXe) dataset=kPbPb;
    if( iCollision == kPbp || iCollision == kpPb ) dataset=2;
    AliAnalysisTask *taskconv = AddTask_ConversionAODProduction(dataset, kFALSE, periodName);
    mgr->RegisterExtraFile("AliAODGammaConversion.root");
  }   

  AliAnalysisTaskESDfilter *taskesdfilter = 0x0;
  if (iESDfilter) 
    {
      //  ESD filter task configuration.
      if (iMUONcopyAOD) {
	printf("Registering delta AOD file\n");
	mgr->RegisterExtraFile("AliAOD.Muons.root");
      }

      Bool_t muonWithSPDTracklets = (iCollision==kPbPb || iCollision==kXeXe) ? kFALSE : kTRUE; // add SPD information to muon AOD only for pp
      if(isMC && isMuonOnly) muonWithSPDTracklets = kFALSE;

       /*
       * switch number of arguments in AddTaskESDFilter 
       * (R+compatibility)
       */
      switch (gROOT->GetGlobalFunction("AddTaskESDFilter")->GetNargs()) {
	
      case 12:
	taskesdfilter = 
	  AddTaskESDFilter(useKFILTER, 
			   iMUONcopyAOD,          // write Muon AOD
			   kFALSE,                // write dimuon AOD 
			   kFALSE,                // usePhysicsSelection 
			   kFALSE,                // centrality OBSOLETE
			   kTRUE,                 // enable TPS only tracks
			   kFALSE,                // disable cascades
			   kFALSE,                // disable kinks
			   run_flag,              // run flag (YY00)
			   3,                     // muonMCMode
			   kFALSE,                // useV0Filter 
			   muonWithSPDTracklets);
	break;
	
      case 13:
        taskesdfilter = 
	  AddTaskESDFilter(useKFILTER, 
			   iMUONcopyAOD,          // write Muon AOD
			   kFALSE,                // write dimuon AOD 
			   kFALSE,                // usePhysicsSelection 
			   kFALSE,                // centrality OBSOLETE
			   kTRUE,                 // enable TPS only tracks
			   kFALSE,                // disable cascades
			   kFALSE,                // disable kinks
			   run_flag,              // run flag (YY00)
			   3,                     // muonMCMode
			   kFALSE,                // useV0Filter 
			   muonWithSPDTracklets,
			   (isMuonCaloPass || isMuonOnly));
	break;
	
      case 14:

	taskesdfilter = 
	  AddTaskESDFilter(useKFILTER, 
			   iMUONcopyAOD,          // write Muon AOD
			   kFALSE,                // write dimuon AOD 
			   kFALSE,                // usePhysicsSelection 
			   kFALSE,                // centrality OBSOLETE
			   kTRUE,                 // enable TPS only tracks
			   kFALSE,                // disable cascades
			   kFALSE,                // disable kinks
			   run_flag,              // run flag (YY00)
			   3,                     // muonMCMode
			   kFALSE,                // useV0Filter 
			   muonWithSPDTracklets,
			   (isMuonCaloPass || isMuonOnly),
			   iPWGGAgammaconv);      // Add PCMV0
	break;
      }
      /*
       * end of
       * switch number of arguments in AddTaskESDFilter 
       * (R+compatibility)
       */

     if (isMuonOnly) {
       taskesdfilter->DisableCaloClusters();
       taskesdfilter->DisableCells();
       taskesdfilter->DisableCaloTrigger("PHOS");
       taskesdfilter->DisableCaloTrigger("EMCAL");
     } else AliEMCALGeometry::GetInstance("","");
    }   

  // ********** PWG3 wagons ******************************************************           
  // PWGHF vertexing
  if (iPWGHFvertexing) 
    {

      Int_t configHF=0;
      if(iCollision == kPbPb || iCollision == kXeXe) configHF=1;
      AliAnalysisTaskSEVertexingHF *taskvertexingHF = AddTaskVertexingHF(configHF,train_name,"",run_number,periodName);
      // Now we need to keep in sync with the ESD filter
      if (!taskvertexingHF) ::Warning("AnalysisTrainNew", "AliAnalysisTaskSEVertexingHF cannot run for this train conditions - EXCLUDED");
      else mgr->RegisterExtraFile("AliAOD.VertexingHF.root");
      taskvertexingHF->SelectCollisionCandidates(0);
    }

  // PWGDQ JPSI filtering (only pp)
  if (iPWGDQJPSIfilter && (iCollision==kpp)) {
    AliAnalysisTaskSE *taskJPSIfilter = AddTaskJPSIFilter();
    if (!taskJPSIfilter) ::Warning("AnalysisTrainNew", "AliAnalysisTaskDielectronFilter cannot run for this train conditions - EXCLUDED");
    else mgr->RegisterExtraFile("AliAOD.Dielectron.root");
    taskJPSIfilter->SelectCollisionCandidates(0);
  }   

  // ********** PWG4 wagons ******************************************************
  // Jet analysis

  // Configurations flags, move up?
  TString kDeltaAODJetName = "AliAOD.Jets.root"; //
  Bool_t  kIsPbPb = (iCollision==kPbPb || iCollision==kXeXe);
  TString kDefaultJetBackgroundBranch = "";
  TString kJetSubtractBranches = "";
  UInt_t kHighPtFilterMask = 272;// from esd filter
  if(isMC) kHighPtFilterMask = 768;// TODO: check why different mask was originally used for MC
  UInt_t iPhysicsSelectionFlag = 0;
  /*
  if (iJETAN) {
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
	AliAnalysisTaskJetBackgroundSubtract *taskSubtract = 0;
	taskSubtract = AddTaskJetBackgroundSubtract(kJetSubtractBranches,1,"B0","B%d");
	taskSubtract->SetBackgroundBranch(kDefaultJetBackgroundBranch.Data());
	if(kDeltaAODJetName.Length()>0)taskSubtract->SetNonStdOutputFile(kDeltaAODJetName.Data());
      }
    } 
  }
  */
}

//______________________________________________________________________________
TChain *CreateChain()
{
  // Create the input chain
  TChain* chain = new TChain("esdTree");
  if (gSystem->AccessPathName("AliESDs.root")) 
    ::Error("AnalysisTrainNew.C::CreateChain", "File: AliESDs.root not in ./data dir");
  else 
    chain->Add("AliESDs.root");
  if (chain->GetNtrees()) return chain;
  return NULL;
}   

//______________________________________________________________________________
void AODmerge()
{
  // Merging method. No staging and no terminate phase.
  TStopwatch timer;
  timer.Start();
  TString outputDir = "wn.xml";
  TString outputFiles = "EventStat_temp.root,AODQA.root,AliAOD.root,AliAOD.VertexingHF.root,AliAOD.Muons.root,AliAODGammaConversion.root,AliAOD.Jets.root,FilterEvents_Trees.root,pyxsec_hists.root";
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
    merged = AliAnalysisAlien::MergeOutput(outputFile, outputDir, 10, 0);
    if (!merged) {
      printf("ERROR: Cannot merge %s\n", outputFile.Data());
      continue;
    }
  }
  // all outputs merged, validate
  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
  mgr->InitAnalysis();
  mgr->SetGridHandler(new AliAnalysisAlien);
  TTree* tree = NULL;
  mgr->StartAnalysis("gridterminate", tree);
  ofstream out;
  out.open("outputs_valid", ios::out);
  out.close();
  timer.Print();
}

//______________________________________________________________________________
void ProcessEnvironment()
{
  //
  // Collision system configuration
  //
  iCollision = kpp;
  if(gSystem->Getenv("ALIEN_JDL_LPMINTERACTIONTYPE"))
    {
      Bool_t valid = kFALSE;
      for (Int_t icoll = 0; icoll < kNSystem; icoll++)
	if (strcmp(gSystem->Getenv("ALIEN_JDL_LPMINTERACTIONTYPE"), CollisionSystem[icoll]) == 0) 
	  {
	    iCollision = icoll;
	    valid = kTRUE;
	    break;
	  }
      if (!valid) {
	printf(">>>>> Unknown collision system configuration: %s \n", gSystem->Getenv("ALIEN_JDL_LPMINTERACTIONTYPE"));
	abort();
      }
    }
  else
    if(!localRunning)
      {
	printf(">>>>> Unknown collision system configuration ALIEN_JDL_LPMINTERACTIONTYPE \n");
	abort();
      }

  //
  // Setting the period
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
      year = atoi(gSystem->Getenv("ALIEN_JDL_LPMANCHORYEAR"));

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
  if (gSystem->Getenv("ALIEN_JDL_RAWPRODTYPE"))
    {
      TString passType = gSystem->Getenv("ALIEN_JDL_RAWPRODTYPE");
      if (passType.Contains("muon_calo"))
	isMuonCaloPass = kTRUE;
      else
	isMuonCaloPass = kFALSE;
    }
  else
    if(!localRunning)
      {
	printf(">>>>> Unknown production type ALIEN_JDL_RAWPRODTYPE \n");
	abort();
      }
}

//______________________________________________________________________________
void ProcessEnvironmentMC()
{
  //
  // Collision system configuration
  //
  iCollision = kpp;
  if (gSystem->Getenv("CONFIG_SYSTEM"))
    {
      Bool_t valid = kFALSE;
      for (Int_t icoll = 0; icoll < kNSystem; icoll++)
	if (strcmp(gSystem->Getenv("CONFIG_SYSTEM"), CollisionSystemMC[icoll]) == 0) {
	  iCollision = icoll;
	  valid = kTRUE;
	  printf(">>>>> Using collision system from CONFIG_SYSTEM as %s (iCollision =%d)\n", CollisionSystemMC[icoll],iCollision);
	  break;
	}
      if (!valid) {
	printf(">>>>> Unknown collision system configuration: %s \n", gSystem->Getenv("CONFIG_SYSTEM"));
	abort();
      }
    }

  //
  // Run number
  //
  gSystem->Exec("set");
  run_number = -1;
  if (gSystem->Getenv("CONFIG_RUN"))
    {
      run_number = atoi(gSystem->Getenv("CONFIG_RUN"));
      printf(">>>>> Using run_number from CONFIG_RUN as %d\n", run_number);
    }
  else
    if (gSystem->Getenv("ALIEN_JDL_LPMRUNNUMBER"))
      {
	run_number = atoi(gSystem->Getenv("ALIEN_JDL_LPMRUNNUMBER"));
	printf(">>>>> Using run_number from ALIEN_JDL_LPMRUNNUMBER as %d\n", run_number);
      }

  if (run_number <= 0)
    {
      printf(">>>>> Invalid run number: %d \n", run_number);
      abort();
    }

  //
  // Setting this to kTRUE will disable some not needed analysis tasks for a muon_calo pass
  //
  isMuonCaloPass = kFALSE;
  isMuonOnly = kFALSE;
  if (gSystem->Getenv("CONFIG_AOD"))
    {
      TString configstr = gSystem->Getenv("CONFIG_AOD");
      if (configstr.Contains("MuonOnly")) isMuonOnly = kTRUE;
      else if (configstr.Contains("Muon")) isMuonCaloPass = kTRUE;
    }

  //
  // Figure out the run_flag - still the "poor-man-solution" :)
  //
  run_flag = 1500;
  if (gSystem->Getenv("CONFIG_YEAR"))
    year = atoi(gSystem->Getenv("CONFIG_YEAR"));
  if (gSystem->Getenv("CONFIG_PERIOD"))
    periodName = gSystem->Getenv("CONFIG_PERIOD");
  if(year<2015)  run_flag =1100;
  if(year<=2010) {
    run_flag =1000;
    if (periodName.EqualTo("LHC10h"))
      run_flag = 1001;
  }
}

//________________________________________________________
void PrintSettings()
{
  printf("\n   **********************************\n");
  printf("   * \n");
  if(useMC) printf("   * Monte Carlo production\n");
  else printf("   * Raw data production\n");
  printf("   * System:         %d\n", iCollision);
  printf("   * Period name:    %s\n", periodName.Data());
  printf("   * Run number:     %d\n", run_number);
  printf("   * Run flag:       %d\n", run_flag);
  printf("   * Muon calo pass: %d\n", isMuonCaloPass);
  printf("   * Centrality:     %d\n", useCentrality);
  printf("   * \n");
  printf("   **********************************\n");
  gSystem->Exec("set");
  printf("   **********************************\n\n");
}
