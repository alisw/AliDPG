void OCDBconfig()
{
  AliCDBManager* man = AliCDBManager::Instance();
  man->SetDefaultStorage("raw://");
  //man->SetDefaultStorage("local:///cvmfs/alice.cern.ch/calibration/data/2015/OCDB");
  man->SetSpecificStorage("TPC/Calib/RecoParam", "alien://folder=/alice/simulation/2008/v4-15-Release/Residual");  
  //
  // ITS (2 objects)

  //  man->SetSpecificStorage("ITS/Align/Data",     "alien://folder=/alice/simulation/2008/v4-15-Release/Residual");
  //  man->SetSpecificStorage("ITS/Calib/SPDSparseDead", "alien://folder=/alice/simulation/2008/v4-15-Release/Residual");  

  // MUON objects (1 object)

  man->SetSpecificStorage("MUON/Align/Data","alien://folder=/alice/simulation/2008/v4-15-Release/Residual"); 

  // TPC (7 objects)

  //
  // TPC (2 new objects for Run2 MC) 

  // Parameters should be used from the raw OCDB at some point
  //  man->SetSpecificStorage("TPC/Calib/Parameters",     "alien://Folder=/alice/cern.ch/user/t/tpcdrop/JIRA/OCDB/PWGPP-218/2015/tuned");//"alien://Folder=/alice/simulation/2008/v4-15-Release/Residual/");
  // ClusterParam needs to be tuned for each group of MCs
  //  man->SetSpecificStorage("TPC/Calib/ClusterParam",   "alien://Folder=/alice/simulation/2008/v4-15-Release/Residual/");

}
