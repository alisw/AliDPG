/*
 * AliDPG - ALICE Experiment Data Preparation Group
 * Central configuration script
 *
 */

/*****************************************************************/

void
Config_LoadLibraries()
{

  // get generator string 
  TString genstr = gSystem->Getenv("CONFIG_GENERATOR");
  // check if needs Phojet/Dpmjet
  Bool_t isDpmjet = kFALSE;
  if (genstr.Contains("dpmjet", TString::kIgnoreCase) || genstr.Contains("phojet", TString::kIgnoreCase)) {
    isDpmjet = kTRUE;
    printf(">>>>> Phojet/Dpmjet libraries receipt \n");
  }

  gSystem->Load("liblhapdf");
  gSystem->Load("libEGPythia6");
  // Phojet/DPMjet with PYTHIA 6.2.14
  if (isDpmjet) {
    gSystem->Load("libpythia6");
  }
  else { 
    gSystem->Load("libpythia6_4_25");
  }
  gSystem->Load("libAliPythia6");
  // hack to make Phojet/DPMjet work
  if (isDpmjet) {
    gSystem->Load("libDPMJET");
    gSystem->Load("libTDPMjet");    
  } 
  TString isFluka = gSystem->Getenv("CONFIG_FLUKA");
  if (isFluka!="on") gSystem->Load("libgeant321");
  
  if (gSystem->Getenv("CONFIG_DETECTOR")) {
    if (strcmp(gSystem->Getenv("CONFIG_DETECTOR"), "FOCAL") == 0) {
      printf(">>>>> Load FOCAL libs \n");
      gSystem->Load("libFOCALbase");
      gSystem->Load("libFOCALgen");
      gSystem->Load("libFOCALsim");
    }
  }
}

