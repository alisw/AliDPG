AliGRPObject *
ExportGRPinfo(Int_t run)
{
  //
  // obtain GRP
  AliCDBManager *cdb = AliCDBManager::Instance();
  cdb->SetDefaultStorage("raw://");
  cdb->SetRun(run);
  AliCDBEntry *cdbe = cdb->Get("GRP/GRP/Data");
  if (!cdbe) return NULL;
  AliGRPObject *grp = (AliGRPObject *)cdbe->GetObject();
  if (!grp) abort();
  //
  // YEAR
  TDatime date = grp->GetTimeStart();
  printf("export CONFIG_YEAR=%d\n", date.GetYear());
  //
  // PERIOD
  printf("export CONFIG_PERIOD=%s\n", grp->GetLHCPeriod().Data());
  //
  // BEAMTYPE
  printf("export CONFIG_BEAMTYPE=%s\n", grp->GetBeamType().Data());
  if (grp->GetBeamType().EqualTo("A-A")) {
    printf("export CONFIG_SYSTEM=Pb-Pb\n");
    printf("export CONFIG_TRIGGER=Pb-Pb\n");
  }
  if (grp->GetBeamType().EqualTo("p-p")) {
    printf("export CONFIG_SYSTEM=p-p\n");
    printf("export CONFIG_TRIGGER=p-p\n");
  }
  //
  // return grp
  return grp;
}
