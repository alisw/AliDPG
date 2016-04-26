#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AliCDBManager.h"
#include <TString.h>
#include <TROOT.h>
#include <TSystem.h>
#endif


const Char_t *Ideal = "alien://Folder=/alice/simulation/2008/v4-15-Release/Ideal/";
const Char_t *Residual = "alien://Folder=/alice/simulation/2008/v4-15-Release/Residual/";
const Char_t *Full = "alien://Folder=/alice/simulation/2008/v4-15-Release/Full/";

//const Char_t *Raw  = "alien://Folder=/alice/data/2015/OCDB";
const Char_t *Raw  = "local:///cvmfs/alice.cern.ch/calibration/data/2015/OCDB";

//____________________________________

void ConfigOCDB(Int_t run, Bool_t sim)
{

  AliCDBManager* man = AliCDBManager::Instance();
  man->SetDefaultStorage(Raw);
  man->SetRun(run);
  
  //
  // set detector specific paths
  if (sim) 
    SetSpecificStorageSim(man, run);
  else 
    SetSpecificStorageRec(man, run);

  // create snapshot
  gROOT->LoadMacro("$ALIDPG_MC/CreateSnapshot.C");
  if (sim)
    CreateSnapshot("OCDBsim.root");
  else
    CreateSnapshot("OCDBrec.root");

}

void
SetSpecificStorageSim(AliCDBManager *man, Int_t run)
{

  // ITS
  man->SetSpecificStorage("ITS/Align/Data", Ideal);
  man->SetSpecificStorage("ITS/Calib/SPDSparseDead", Full); // only for PbPb ?
  
  // TPC
  man->SetSpecificStorage("TPC/Calib/Parameters",   Residual);
  man->SetSpecificStorage("TPC/Calib/ClusterParam", Residual);
  man->SetSpecificStorage("TPC/Calib/RecoParam",    Full);
  man->SetSpecificStorage("TPC/Calib/TimeGain",     Ideal);
  man->SetSpecificStorage("TPC/Calib/Correction",   Ideal);
  man->SetSpecificStorage("TPC/Align/Data",         Ideal);
  man->SetSpecificStorage("TPC/Calib/TimeDrift",    Ideal);

  // MUON
  man->SetSpecificStorage("MUON/Align/Data", Ideal);

  // ZDC
  man->SetSpecificStorage("ZDC/Align/Data", Ideal);
  man->SetSpecificStorage("ZDC/Calib/Pedestals", Ideal); // only for PbPb ?

}

void
SetSpecificStorageRec(AliCDBManager *man, Int_t run)
{

  // ITS
  man->SetSpecificStorage("ITS/Align/Data", Residual);
  man->SetSpecificStorage("ITS/Calib/SPDSparseDead", Residual); // only for PbPb ?
  
  // TPC
  man->SetSpecificStorage("TPC/Calib/Parameters",   Residual);
  man->SetSpecificStorage("TPC/Calib/ClusterParam", Residual);
  man->SetSpecificStorage("TPC/Calib/RecoParam",    Full);
  man->SetSpecificStorage("TPC/Calib/TimeGain",     Residual);
  man->SetSpecificStorage("TPC/Calib/Correction",   Residual);
  man->SetSpecificStorage("TPC/Align/Data",         Residual);
  man->SetSpecificStorage("TPC/Calib/TimeDrift",    Residual);

  // MUON
  man->SetSpecificStorage("MUON/Align/Data", Residual);

  // ZDC
  man->SetSpecificStorage("ZDC/Align/Data", Ideal);
  man->SetSpecificStorage("ZDC/Calib/Pedestals", Ideal); // only for PbPb ?

}

