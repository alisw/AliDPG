Int_t CheckTOF(Int_t run){

  AliCDBManager* man = AliCDBManager::Instance();
  man->SetDefaultStorage("raw://");
  man->SetRun(run);
  AliCDBEntry* e1 = man->Get("GRP/CTP/Config");
  AliTriggerConfiguration* t = (AliTriggerConfiguration*)e1->GetObject();
  TString trigDet = t->GetTriggeringDetectors();
  TString actiDet = t->GetActiveDetectors();

  Bool_t isTOFtriggering = trigDet.Contains("TOF");
  Bool_t isTOFactive = actiDet.Contains("TOF");

  //if (isTOFtriggering && !isTOFactive){ // the information on the triggering from GRP/CTP/Config is wrong, we just check if TOF is active
  if (!isTOFactive){
    //Printf("TOF is triggering but not active, we need to load the special TriggerConfig");
    Printf("TOF is not active, we need to load the special TriggerConfig");
    return 1;
  }
  else {
    Printf("All ok for TOF!");
    return 0;
  }
}
