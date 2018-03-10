#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AliCDBManager.h"
#include "AliCDBEntry.h"
#include "AliCDBId.h"
#include <TString.h>
#include <TMap.h>
#endif

Bool_t patchOCDBsnapshot(const char* snapshot, int run, const char* cdbPath, const char* specialStorage=0)
{
  AliCDBManager* man = AliCDBManager::Instance();
  if (!man->IsDefaultStorageSet()) man->SetRaw(1);
  man->SetRun(run);
  TString spec = specialStorage;
  if (!spec.IsNull()) {
    man->SetSpecificStorage(cdbPath,spec.Data());
  }
  
  AliCDBEntry* ent = man->Get(cdbPath);
  TString pthsnp = cdbPath;
  pthsnp.ReplaceAll("/","*");
  TFile* snp = TFile::Open(snapshot,"update");
  if (!snp) {
    printf("Error: failed to open snapshot file %s\n",snapshot);
    return kFALSE;
  }
  ent->Write(pthsnp.Data(),TObject::kOverwrite);
  //
  // repair map if needed
  const char* mapName = "CDBSnapshotEntriesMap";
  TMap* map = (TMap*)snp->Get(mapName);
  if (map) {
    TPair* pair = (TPair*)map->FindObject(cdbPath);
    if (pair) delete map->Remove(pair->Key());
    if (!spec.IsNull()) {
      TObjString *key = new TObjString(cdbPath);
      TObjString *val = new TObjString(spec.Data());
      map->Add(key,val); 
    }
    map->Write(mapName,TObject::kOverwrite|TObject::kSingleKey);
  }
  // repair objects list
  const char* lstName = "CDBSnapshotIdsList";
  TList* lst = (TList*)snp->Get(lstName);
  if (lst) {
    TIter next(lst);
    AliCDBId* cdbid=0;
    while ( (cdbid=(AliCDBId*)next()) ) if (cdbid->GetPath() == cdbPath) break;
    if (cdbid) delete lst->Remove(cdbid);
    //
    cdbid = new AliCDBId(ent->GetId());
    lst->Add(cdbid);
    lst->Write(lstName,TObject::kOverwrite|TObject::kSingleKey);
  }
  //
  snp->Close();
  delete snp;
}
