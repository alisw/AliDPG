// OCDB for Run 3 simulation

void OCDBRun3(int type=0)
{
  
  AliCDBManager* man = AliCDBManager::Instance();
  
  man->SetDefaultStorage("raw://");

  // common for sim and rec
  
  man->SetSpecificStorage("MUON/Align/Data","alien://folder=/alice/simulation/2008/v4-15-Release/Ideal/");

  // removed MFT for the time being
  // man->SetSpecificStorage("MFT/Align/Data",       "alien://folder=/alice/cern.ch/user/a/auras/OCDB/");
  // man->SetSpecificStorage("MFT/Calib/RecoParam",  "alien://folder=/alice/cern.ch/user/a/auras/OCDB/");

  man->SetSpecificStorage("FIT/Align/Data", "alien://folder=/alice/cern.ch/user/p/pwg_pp/JIRA/UPG-11/OCDB/");
  man->SetSpecificStorage("FIT/Calib/RecoParam", "alien://folder=/alice/cern.ch/user/p/pwg_pp/JIRA/UPG-11/OCDB/");
  
  man->SetSpecificStorage("ITS/Align/Data", "alien://folder=/alice/cern.ch/user/p/pwg_pp/JIRA/UPG-11/OCDB/");
  man->SetSpecificStorage("ITS/Calib/SimuParam","alien://folder=/alice/cern.ch/user/p/pwg_pp/JIRA/UPG-11/OCDB/");
  man->SetSpecificStorage("ITS/Calib/RecoParam","alien://folder=/alice/cern.ch/user/p/pwg_pp/JIRA/UPG-11/OCDB/");

  man->SetSpecificStorage("ZDC/Align/Data","alien://Folder=/alice/simulation/2008/v4-15-Release/Ideal/");
  man->SetSpecificStorage("ZDC/Calib/Pedestals","alien://Folder=/alice/simulation/2008/v4-15-Release/Ideal/");
  
  man->SetSpecificStorage("TPC/Calib/RecoParam","alien://Folder=/alice/simulation/2008/v4-15-Release/Residual/");

  if (type==0) { // sim specific
    man->SetSpecificStorage("TPC/Calib/ClusterParam","alien://Folder=/alice/simulation/2008/v4-15-Release/Ideal/");
    man->SetSpecificStorage("TPC/Calib/TimeGain","alien://Folder=/alice/simulation/2008/v4-15-Release/Ideal/");
    man->SetSpecificStorage("TPC/Calib/Correction","alien://Folder=/alice/simulation/2008/v4-15-Release/Ideal/");
  }
  else { // rec specific
    man->SetSpecificStorage("TPC/Calib/ClusterParam","alien://Folder=/alice/simulation/2008/v4-15-Release/Residual/");
    man->SetSpecificStorage("TPC/Calib/TimeGain","alien://Folder=/alice/simulation/2008/v4-15-Release/Residual/");
    man->SetSpecificStorage("TPC/Calib/Correction","alien://Folder=/alice/simulation/2008/v4-15-Release/Residual/");
  }
  
  
  Int_t run  = atoi(gSystem->Getenv("CONFIG_RUN"));
  man->SetRun(run);

  

}
