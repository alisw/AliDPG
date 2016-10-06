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

OCDBConfig(Int_t tag, Int_t type)
{

  switch (tag) {
    
    // kOCDBDefault
  case kOCDBDefault:
    OCDBDefault(type);
    break;
    
    // kOCDBCustom
  case kOCDBCustom:
    if ((gROOT->LoadMacro("OCDBCustom.C")) != 0) {
      printf("ERROR: cannot find OCDBCustom.C\n");
      abort();
      return;
    }
    OCDBCustom();
    break;
    
  }

}

/*****************************************************************/

OCDBDefault(Int_t mode)
{

  Int_t run  = atoi(gSystem->Getenv("CONFIG_RUN"));  
  AliCDBManager* man = AliCDBManager::Instance();
  man->SetDefaultStorage("raw://");
  man->SetRun(run);
  
  // set detector specific paths
  DefaultSpecificStorage(man, mode);

}

/*****************************************************************/

DefaultSpecificStorage(AliCDBManager *man, Int_t mode)
{

  AliCDBEntry *cdbe = man->Get("GRP/GRP/Data");
  if (!cdbe) return NULL;
  AliGRPObject *grp = (AliGRPObject *)cdbe->GetObject();
  TDatime date = grp->GetTimeStart();
  Int_t year = date.GetYear();

  const Char_t *Raw      = Form("alien://Folder=/alice/data/%d/OCDB", year);
  const Char_t *Ideal    = "alien://Folder=/alice/simulation/2008/v4-15-Release/Ideal/";
  const Char_t *Residual = "alien://Folder=/alice/simulation/2008/v4-15-Release/Residual/";
  const Char_t *Full     = "alien://Folder=/alice/simulation/2008/v4-15-Release/Full/";
  // for AD hack 
  const Char_t *Raw2015  = "alien://Folder=/alice/data/2015/OCDB";
  
  // DEFAULT SPECIFIC OBJECTS 
  const Char_t *SpecificStorageList[][3] = {
    // path                    sim       rec
    //
    // ITS
    "ITS/Align/Data",          Ideal,    Residual, // ok
    "ITS/Calib/SPDSparseDead", Full,     Residual, // ok ?
    // TPC
    "TPC/Calib/ClusterParam",  Ideal,    Residual, // ok!
    "TPC/Calib/RecoParam",     Residual, Residual, // ok!
    "TPC/Calib/TimeGain",      Ideal,    Residual, // ok!
    "TPC/Calib/Correction",    Ideal,    Residual, // ok!
    // MUON
    "MUON/Align/Data",         Full,     NULL,     // ok!
    // ZDC
    "ZDC/Align/Data",          Ideal,    Ideal     // ok
  };
  const Int_t nSpecificStorages = sizeof(SpecificStorageList) / (3 * sizeof(Char_t *));

  // set specific storages
  for (Int_t isto = 0; isto < nSpecificStorages; isto++) {
    if (SpecificStorageList[isto][mode+1]) {
      printf("Setting specific storage: %s -> %s\n", SpecificStorageList[isto][0], SpecificStorageList[isto][mode+1]);
      man->SetSpecificStorage(SpecificStorageList[isto][0], SpecificStorageList[isto][mode+1]);
    }
  }

  // EXTRA SPECIFIC OBJECTS IF NO DISTORTIONS
  if (!RecoParamWithTPCDistortions(man)) {
    printf("Not using TPC distortions, need extra specific storages\n");
    NoDistortionSpecificStorage(man, mode);
  }
  
  // Run-1 settings
  if (year < 2015) {
    man->SetSpecificStorage("AD/Calib/QAParam", Raw2015);
    const Char_t *muonHack[2] = {Ideal, Residual};
    man->SetSpecificStorage("MUON/Align/Data", muonHack[mode]);
  }

}

/*****************************************************************/

NoDistortionSpecificStorage(AliCDBManager *man, Int_t mode) {

  const Char_t *Ideal    = "alien://Folder=/alice/simulation/2008/v4-15-Release/Ideal/";
  const Char_t *Residual = "alien://Folder=/alice/simulation/2008/v4-15-Release/Residual/";

  const Char_t *SpecificStorageList[][3] = {
    // path                    sim       rec
    //
    "TPC/Align/Data",          Ideal,    Residual, // ok!
    "TPC/Calib/TimeDrift",     Ideal,    Residual  // ok!
  };
  const Int_t nSpecificStorages = sizeof(SpecificStorageList) / (3 * sizeof(Char_t *));

  for (Int_t isto = 0; isto < nSpecificStorages; isto++) {
    if (SpecificStorageList[isto][mode+1]) {
      printf("Setting specific storage: %s -> %s\n", SpecificStorageList[isto][0], SpecificStorageList[isto][mode+1]);
      man->SetSpecificStorage(SpecificStorageList[isto][0], SpecificStorageList[isto][mode+1]);
    }
  }
  
}

/*****************************************************************/

Bool_t
RecoParamWithTPCDistortions(AliCDBManager *man) {

  // check methods exist
  if (!AliRecoParam::Class()->GetMethodAny("SuggestRunEventSpecie"))
    return kFALSE;
  if (!AliTPCRecoParam::Class()->GetMethodAny("GetUseCorrectionMap"))
    return kFALSE;
  
  // get event specie from GRP
  AliCDBEntry *grpe = man->Get("GRP/GRP/Data");
  if (!grpe) return kFALSE;
  AliGRPObject *grp = (AliGRPObject *)grpe->GetObject();
  AliRecoParam::EventSpecie_t evs = AliRecoParam::SuggestRunEventSpecie(grp->GetRunType(),
									grp->GetBeamType(),
									grp->GetLHCState());

  // get TPC RecoParam for event specie
  AliCDBEntry *pare = man->Get("TPC/Calib/RecoParam");
  if (!pare) return kFALSE;
  TObjArray *parl = (TObjArray *)pare->GetObject();
  AliTPCRecoParam *par = NULL;
  for (Int_t i = parl->GetEntriesFast(); i--;) {
    AliTPCRecoParam *p = (AliTPCRecoParam *)parl->UncheckedAt(i);
    if (!p || !(p->GetEventSpecie() & evs)) continue;
    par = p;
    break;
  }
  // check if use correction map
  if (!par->GetUseCorrectionMap())
    return kFALSE;

  return kTRUE;
  
}
