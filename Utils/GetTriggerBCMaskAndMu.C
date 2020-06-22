#if !defined(__CINT__) || defined(__MAKECINT__)
#include "TString.h"
#include "AliCDBManager.h"
#include "AliCDBEntry.h"
#include "TSystem.h"
#include "AliTriggerConfiguration.h"
#include "AliTriggerBCMask.h"
#include "AliLumiTools.h"
#endif

///
/// \file GetTriggerBCMaskAndMu.C
/// \brief Recover the run trigger BC mask and pile-up mu value from OCDB
/// \author Ruben Shahoyan (main) and Gustavo Conesa (copy from older version of Hijing_PileUp.C::GetMaskMu())
///

//
/// \param mu value calculated here
/// \param run number, if negative set via $CONFIG_RUN
/// \return string with bc mask
//
TString GetTriggerBCMaskAndMu( double& mu, int run = -1 )
{
  if ( run <= 0  )
  {
    if (gSystem->Getenv("CONFIG_RUN"))
      run = atoi(gSystem->Getenv("CONFIG_RUN"));
  }
  
  AliCDBManager* man = AliCDBManager::Instance();
  if (gSystem->AccessPathName("OCDB.root", kFileExists)==0) {     
    man->SetDefaultStorage("local://");
    man->SetRaw(kFALSE);
    man->SetSnapshotMode("OCDB.root");
  }
  else if (!man->IsDefaultStorageSet()) {
    man->SetDefaultStorage("raw://");
  }
  
  man->SetRun(run);
  
  //
  // Get the mask
  //
  AliCDBEntry* entry = man->Get("GRP/CTP/Config");
  const AliTriggerConfiguration* cf = (const AliTriggerConfiguration*)entry->GetObject();
  
  const AliTriggerBCMask* mask = (const AliTriggerBCMask*)cf->GetMasks()[0];
  
  TString maskStr = mask->GetTitle();
  
  printf("GetTriggerBCMaskAndMu() -- mask %s\n",maskStr.Data());

  //
  // Get mu
  //
  mu = AliLumiTools::GetMuEstimate();
  
  // alternative way of extracting the mu:
  double rateInHzPerMillibarn = AliLumiTools::GetLumiFromCTP(run)->GetMean(2);  
  double xsect                = AliLumiTools::GetXSectEstimate();
  int    nbc                  = mask->GetNUnmaskedBCs();
  double muAlt = rateInHzPerMillibarn*xsect/11245./nbc;
  
  printf("GetTriggerBCMaskAndMu() -- run %d, %.2f Hz/mb, "
         "Xsection %.2f mb -> %.2f Hz, mu_per_bunch = %e, alternative mu %e\n", 
         run, rateInHzPerMillibarn, 
         xsect, rateInHzPerMillibarn*xsect, mu, muAlt);
  
  return maskStr;
}
