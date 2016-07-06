/*
 * AliDPG - ALICE Experiment Data Preparation Group
 * OCDB configuration script
 *
 */

enum EOCDB_t {
  kOCDBDefault,
  kOCDBCustom,
  kNOCDBs
};

const Char_t *OCDBName[kNOCDBs] = {
  "Default",
  "Custom"
};

/*****************************************************************/

OCDBConfig(Int_t tag, Int_t run, Int_t type)
{

  switch (tag) {
    
    // kOCDBDefault
  case kOCDBDefault:
    OCDBDefault(run, type);
    break;
    
    // kOCDBCustom
  case kOCDBCustom:
    if ((gROOT->LoadMacro("OCDBCustom.C")) != 0) {
      printf("ERROR: cannot find OCDBCustom.C\n");
      abort();
      return;
    }
    OCDBCustom(run);
    break;
    
  }

}

/*****************************************************************/

OCDBDefault(Int_t run, Int_t mode)
{

  gROOT->LoadMacro("$ALIDPG_ROOT/MC/Utils.C");
  Int_t year = RunToYear(run);
  
  AliCDBManager* man = AliCDBManager::Instance();
  man->SetDefaultStorage(Form("alien://Folder=/alice/data/%d/OCDB", year));
  man->SetRun(run);
  
  //
  // set detector specific paths according to LHC Run
  if (year < 2015)
    DefaultSpecificStorageRun1(man, run, mode);
  else
    DefaultSpecificStorageRun2(man, run, mode);

}

/*****************************************************************/

DefaultSpecificStorageRun1(AliCDBManager *man, Int_t run, Int_t mode)
{
  
  gROOT->LoadMacro("$ALIDPG_ROOT/MC/Utils.C");
  Int_t year = RunToYear(run);

  const Char_t *Raw      = Form("alien://Folder=/alice/data/%d/OCDB", year);
  const Char_t *Ideal    = "alien://Folder=/alice/simulation/2008/v4-15-Release/Ideal/";
  const Char_t *Residual = "alien://Folder=/alice/simulation/2008/v4-15-Release/Residual/";
  const Char_t *Full     = "alien://Folder=/alice/simulation/2008/v4-15-Release/Full/";
  // for AD hack 
  const Char_t *Raw2015  = "alien://Folder=/alice/data/2015/OCDB";
  
  // DEFAULT RUN-1 SPECIFIC OBJECTS 
  const Char_t *SpecificStorageList[][3] = {
    // path                    sim       rec
    //
    // ITS
    "ITS/Align/Data",          Ideal,    Residual, // ok
    "ITS/Calib/SPDSparseDead", Full,     Residual, // ok ?
    // TPC
    "TPC/Calib/Parameters",    Residual, Residual, // do we need this ?
    "TPC/Calib/ClusterParam",  Residual, Residual, // Ideal, Residual ?
    "TPC/Calib/RecoParam",     Residual, Residual, // ok ?
    "TPC/Calib/TimeGain",      Ideal,    Residual, // ok
    "TPC/Calib/Correction",    Ideal,    Residual, // ok
    "TPC/Align/Data",          Ideal,    Residual, // ok
    "TPC/Calib/TimeDrift",     Ideal,    Residual, // ok
    "TPC/Calib/AltroConfig",   Ideal,    Residual, // do we need this ?
    // MUON
    "MUON/Align/Data",         Ideal,    Residual, // ok
    // ZDC
    "ZDC/Align/Data",          Ideal,    Ideal,    // ok ?
    "ZDC/Calib/Pedestals",     Ideal,    Ideal,    // do we need this ?
    // AD
    "AD/Calib/QAParam",        Raw2015,  Raw2015   // temporary HACK
  };
  const Int_t nSpecificStorages = sizeof(SpecificStorageList) / (3 * sizeof(Char_t *));

  // set specific storages
  for (Int_t isto = 0; isto < nSpecificStorages; isto++) {
    if (SpecificStorageList[mode][isto]) {
      printf("Setting specific storage: %s -> %s\n", SpecificStorageList[isto][0], SpecificStorageList[isto][mode+1]);
      man->SetSpecificStorage(SpecificStorageList[isto][0], SpecificStorageList[isto][mode+1]);
    }
  }

}

/*****************************************************************/

DefaultSpecificStorageRun2(AliCDBManager *man, Int_t run, Int_t mode)
{
  
  gROOT->LoadMacro("$ALIDPG_ROOT/MC/Utils.C");
  Int_t year = RunToYear(run);

  const Char_t *Raw      = Form("alien://Folder=/alice/data/%d/OCDB", year);
  const Char_t *Ideal    = "alien://Folder=/alice/simulation/2008/v4-15-Release/Ideal/";
  const Char_t *Residual = "alien://Folder=/alice/simulation/2008/v4-15-Release/Residual/";
  const Char_t *Full     = "alien://Folder=/alice/simulation/2008/v4-15-Release/Full/";
  
  // DEFAULT RUN-1 SPECIFIC OBJECTS 
  const Char_t *SpecificStorageList[][3] = {
    // path                    sim       rec
    //
    // ITS
    "ITS/Align/Data",          Ideal,    Residual,
    "ITS/Calib/SPDSparseDead", Full,     Residual,
    // TPC
    "TPC/Calib/Parameters",    Residual, Residual,
    "TPC/Calib/ClusterParam",  Residual, Residual,
    "TPC/Calib/RecoParam",     Full,     Full,      // this needs to be moved to Residual
    "TPC/Calib/TimeGain",      Ideal,    Residual,
    "TPC/Calib/Correction",    Ideal,    Residual,
    "TPC/Align/Data",          Ideal,    Residual,
    "TPC/Calib/TimeDrift",     Ideal,    Residual,
    // MUON
    "MUON/Align/Data",         Ideal,    Residual,
    // ZDC
    "ZDC/Align/Data",          Ideal,    Ideal,
    "ZDC/Calib/Pedestals",     Ideal,    Ideal
  };
  const Int_t nSpecificStorages = sizeof(SpecificStorageList) / (3 * sizeof(Char_t *));

  // set specific storages
  for (Int_t isto = 0; isto < nSpecificStorages; isto++) {
    if (SpecificStorageList[mode][isto]) {
      printf("Setting specific storage: %s -> %s\n", SpecificStorageList[isto][0], SpecificStorageList[isto][mode+1]);
      man->SetSpecificStorage(SpecificStorageList[isto][0], SpecificStorageList[isto][mode+1]);
    }
  }

}

