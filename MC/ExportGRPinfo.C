AliGRPObject *
ExportGRPinfo(Int_t run)
{
  // set OCDB source
  AliCDBManager *cdbm = AliCDBManager::Instance();
  TString ocdbConfig = "default,snapshot";
  if (gSystem->Getenv("CONFIG_OCDB"))
    ocdbConfig = gSystem->Getenv("CONFIG_OCDB");
  if (ocdbConfig.Contains("alien") || ocdbConfig.Contains("cvmfs")) {
    // set OCDB 
    gROOT->LoadMacro("$ALIDPG_ROOT/MC/OCDBConfig.C");
    OCDBDefault(0);
  }
  else {
    // set OCDB snapshot mode
    cdbm->SetSnapshotMode("OCDBsim.root");
  }

  //
  // obtain GRP
  cdbm->SetRun(run);
  AliCDBEntry *cdbe = cdbm->Get("GRP/GRP/Data");
  if (!cdbe) return NULL;
  AliGRPObject *grp = (AliGRPObject *)cdbe->GetObject();
  if (!grp) abort();
  //
  // DETECTOR MASK
  UInt_t mask = grp->GetDetectorMask();
  printf("export CONFIG_DETECTORMASK=0x%x\n", mask);
  //
  // YEAR
  TDatime date = grp->GetTimeStart();
  printf("export CONFIG_YEAR=%d\n", date.GetYear());
  //
  // PERIOD
  printf("export CONFIG_PERIOD=%s\n", grp->GetLHCPeriod().Data());
  //
  // BEAMTYPE, SYSTEM, TRIGGER, ENERGY
  printf("export CONFIG_BEAMTYPE=%s\n", grp->GetBeamType().Data());
  if (grp->GetBeamType().EqualTo("A-A")) {
    printf("export CONFIG_SYSTEM=Pb-Pb\n");
    printf("export CONFIG_TRIGGER=Pb-Pb\n");
    printf("export CONFIG_ENERGY=%.0f\n", grp->GetBeamEnergy() * 2.);
  }
  if (grp->GetBeamType().EqualTo("p-p")) {
    printf("export CONFIG_SYSTEM=p-p\n");
    printf("export CONFIG_TRIGGER=p-p\n");
    printf("export CONFIG_ENERGY=%.0f\n", grp->GetBeamEnergy() * 2.);
  }
  if (grp->GetBeamType().EqualTo("p-A")) {
    printf("export CONFIG_SYSTEM=Pb-p\n");
    printf("export CONFIG_TRIGGER=p-p\n");
    printf("export CONFIG_ENERGY=%.0f\n", grp->GetBeamEnergy() * 2. * TMath::Sqrt(82./208.));
  }
  if (grp->GetBeamType().EqualTo("A-p")) {
    printf("export CONFIG_SYSTEM=p-Pb\n");
    printf("export CONFIG_TRIGGER=p-p\n");
    printf("export CONFIG_ENERGY=%.0f\n", grp->GetBeamEnergy() * 2. * TMath::Sqrt(82./208.));
  }
  //
  // ENERGY - must be fixed 
  //
  // return grp
  return grp;
}
