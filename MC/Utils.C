/*
 * AliDPG - ALICE Experiment Data Preparation Group
 * Utility functions
 *
 */

/*****************************************************************/
/*****************************************************************/
/*****************************************************************/

AliGRPObject *
GetGRP(Int_t run)
{
  AliCDBManager *cdb = AliCDBManager::Instance();
  cdb->SetDefaultStorage("raw://");
  cdb->SetRun(run);
  AliCDBEntry *cdbe = cdb->Get("GRP/GRP/Data");
  if (!cdbe) return NULL;
  AliGRPObject *grp = (AliGRPObject *)cdbe->GetObject();
  return grp;
}

/*****************************************************************/

TString
RunToPeriod(Int_t run)
{
  AliGRPObject *grp = GetGRP(run);
  if (!grp) return "";
  return grp->GetLHCPeriod();
}

/*****************************************************************/

Int_t
RunToYear(Int_t run)
{
  AliGRPObject *grp = GetGRP(run);
  if (!grp) return 0;
  TDatime date = grp->GetTimeStart();
  return date.GetYear();
}
