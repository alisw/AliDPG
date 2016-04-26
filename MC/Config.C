/*
 * AliDPG - ALICE Experiment Data Preparation Group
 * Central configuration script
 *
 */

/*****************************************************************/
/*****************************************************************/
/*****************************************************************/

// global variables

static Int_t  runNumber       = 0;         // run number
static Int_t  neventsConfig   = 200;       // number of events
static Int_t  magnetConfig    = 0;         // magnetic field
static Int_t  detectorConfig  = 0;         // detector
static Int_t  generatorConfig = 0;         // MC generator
static Float_t energyConfig   = 0.;        // CMS energy
static Float_t crossingConfig = 0.;        // 2.8e-4 // crossing angle
static Int_t  seedConfig      = 123456789; // random seed

/*****************************************************************/

void
Config()
{

  /* initialise */
  gROOT->LoadMacro("$ALIDPG_MC/DetectorConfig.C");
  gROOT->LoadMacro("$ALIDPG_MC/GeneratorConfig.C");
  ProcessEnvironment();

  /* verbose */
  printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
  printf(">>>>>       run number: %d \n", runNumber);
  printf(">>>>> number of events: %d \n", neventsConfig);
  printf(">>>>>   magnetic field: %s \n", MagnetName[magnetConfig]);
  printf(">>>>>         detector: %s \n", DetectorName[detectorConfig]);
  printf(">>>>>     MC generator: %s \n", GeneratorName[generatorConfig]);
  printf(">>>>>       CMS energy: %f \n", energyConfig);
  printf(">>>>>   crossing angle: %f \n", crossingConfig);
  printf(">>>>>      random seed: %d \n", seedConfig);
  printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");

  /* load libraries */
  LoadLibraries();

  /* setup geant */
  new TGeant3TGeo("C++ Interface to Geant3");

  /* create galice.root */
  CreateGAlice();

  /* configure detector */
  DetectorConfig(detectorConfig, runNumber);

  /* configure MC generator */
  GeneratorConfig(generatorConfig, runNumber);
  GeneratorOptions();
}

/*****************************************************************/

Float_t
SetEnergyFromGRP()
{
  AliCDBEntry *cdbe = AliCDBManager::Instance()->Get("GRP/GRP/Data");
  AliGRPObject *grpd = dynamic_cast<AliGRPObject*>(cdbe->GetObject()); 
  return (grpd->GetBeamEnergy() * 2.);
}

/*****************************************************************/

void
ProcessEnvironment()
{

  // run number
  if (gSystem->Getenv("CONFIG_RUN"))
    runNumber = atoi(gSystem->Getenv("CONFIG_RUN"));

  // number of events configuration
  neventsConfig = 200;
  if (gSystem->Getenv("CONFIG_NEVENTS"))
    neventsConfig = atoi(gSystem->Getenv("CONFIG_NEVENTS"));

  // magnetic field configuration
  magnetConfig = kMagnetDefault;
  if (gSystem->Getenv("CONFIG_MAGNET")) {
    Bool_t valid = kFALSE;
    for (Int_t imag = 0; imag < kNMagnets; imag++)
      if (strcmp(gSystem->Getenv("CONFIG_MAGNET"), MagnetName[imag]) == 0) {
	magnetConfig = imag;
	valid = kTRUE;
	break;
      }
    if (!valid) {
      printf(">>>>> Unknown magnetic field configuration: %s \n", gSystem->Getenv("CONFIG_MAGNET"));
      abort();
    }
  }
	
  // detector configuration
  detectorConfig = kDetectorDefault;
  if (gSystem->Getenv("CONFIG_DETECTOR")) {
    Bool_t valid = kFALSE;
    for (Int_t idet = 0; idet < kNDetectors; idet++)
      if (strcmp(gSystem->Getenv("CONFIG_DETECTOR"), DetectorName[idet]) == 0) {
	detectorConfig = idet;
	valid = kTRUE;
	break;
      }
    if (!valid) {
      printf(">>>>> Unknown detector configuration: %s \n", gSystem->Getenv("CONFIG_DETECTOR"));
      abort();
    }
  }
	
  // generator configuration
  generatorConfig = kGeneratorDefault;
  if (gSystem->Getenv("CONFIG_GENERATOR")) {
    Bool_t valid = kFALSE;
    for (Int_t igen = 0; igen < kNGenerators; igen++)
      if (strcmp(gSystem->Getenv("CONFIG_GENERATOR"), GeneratorName[igen]) == 0) {
	generatorConfig = igen;
	valid = kTRUE;
	break;
      }
    if (!valid) {
      printf(">>>>> Unknown MC generator configuration: %s \n", gSystem->Getenv("CONFIG_GENERATOR"));
      abort();
    }
  }
  
  // energy configuration
  energyConfig = SetEnergyFromGRP();
  if (gSystem->Getenv("CONFIG_ENERGY"))
    energyConfig = atoi(gSystem->Getenv("CONFIG_ENERGY"));
  if (energyConfig <= 0) {
    printf(">>>>> Invalid CMS energy: %f \n", energyConfig);
    abort();
  }

  // seed configuration
  seedConfig = TDatime().Get();
  if (gSystem->Getenv("CONFIG_SEED"))
    seedConfig = atoi(gSystem->Getenv("CONFIG_SEED"));
  
}

/*****************************************************************/

void
LoadLibraries()
{
  gSystem->Load("liblhapdf");
  gSystem->Load("libEGPythia6");
  // phojet libraries have to be loaded before geant3
  if (generatorConfig == kGeneratorPhojet) {
    gSystem->Load("libDPMJET");
    gSystem->Load("libTDPMjet");
  }
  gSystem->Load("libgeant321");
  gSystem->Load("libpythia6_4_25");
  gSystem->Load("libAliPythia6");
  // pythia8 libraries
  if (generatorConfig == kGeneratorPythia8_Monash2013) {
    gSystem->Load("libpythia8.so");
    gSystem->Load("libAliPythia8.so");
  }
  // hijing libraries
  if (generatorConfig == kGeneratorHijing) {
    gSystem->Load("libHIJING");
    gSystem->Load("libTHijing");
  }

}

/*****************************************************************/

void
CreateGAlice() 
{
  //=======================================================================
  //  Create the output file
   
  AliRunLoader* rl=0x0;

  cout<<"Config.C: Creating Run Loader ..."<<endl;
  rl = AliRunLoader::Open("galice.root",
			  AliConfig::GetDefaultEventFolderName(),
			  "recreate");
  if (!rl) {
    gAlice->Fatal("Config.C","Can not instatiate the Run Loader");
    return;
  }
  rl->SetCompressionLevel(2);
  rl->SetNumberOfEventsPerFile(1000);
  gAlice->SetRunLoader(rl);
  // gAlice->SetGeometryFromFile("geometry.root");
  // gAlice->SetGeometryFromCDB();
  rl->CdGAFile();
}

/*****************************************************************/

void
GeneratorOptions()
{
  //======================//
  //    Set MC options    //
  //======================//

  enum PprTrigConf_t
  {
    kDefaultPPTrig, kDefaultPbPbTrig
  };
  
  const char * pprTrigConfName[] =
    {
      "p-p","Pb-Pb"
      //    "ocdb","ocdb"
    };

  Int_t strig = kDefaultPPTrig;
  
  // Set the trigger configuration: proton-proton
  AliSimulation::Instance()->SetTriggerConfig(pprTrigConfName[strig]);
  cout <<"Trigger configuration is set to  "<<pprTrigConfName[strig]<<endl;
  //
  gMC->SetProcess("DCAY",1);
  gMC->SetProcess("PAIR",1);
  gMC->SetProcess("COMP",1);
  gMC->SetProcess("PHOT",1);
  gMC->SetProcess("PFIS",0);
  gMC->SetProcess("DRAY",0);
  gMC->SetProcess("ANNI",1);
  gMC->SetProcess("BREM",1);
  gMC->SetProcess("MUNU",1);
  gMC->SetProcess("CKOV",1);
  gMC->SetProcess("HADR",1);
  gMC->SetProcess("LOSS",2);
  gMC->SetProcess("MULS",1);
  gMC->SetProcess("RAYL",1);
  
  Float_t cut = 1.e-3;        // 1MeV cut by default
  Float_t tofmax = 1.e10;
  
  gMC->SetCut("CUTGAM", cut);
  gMC->SetCut("CUTELE", cut);
  gMC->SetCut("CUTNEU", cut);
  gMC->SetCut("CUTHAD", cut);
  gMC->SetCut("CUTMUO", cut);
  gMC->SetCut("BCUTE",  cut); 
  gMC->SetCut("BCUTM",  cut); 
  gMC->SetCut("DCUTE",  cut); 
  gMC->SetCut("DCUTM",  cut); 
  gMC->SetCut("PPCUTM", cut);
  gMC->SetCut("TOFMAX", tofmax); 
  //
  //======================//
  // Set External decayer //
  //======================//
  TVirtualMCDecayer* decayer = new AliDecayerPythia();
  decayer->SetForceDecay(kAll);
  decayer->Init();
  gMC->SetExternalDecayer(decayer);
  //
}
