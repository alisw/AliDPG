#if !defined(__CINT__) || defined(__MAKECINT__)
#include "TString.h"
#include "AliCDBManager.h"
#include "AliCDBEntry.h"
#include "TSystem.h"
#include "AliTriggerConfiguration.h"
#include "AliTriggerBCMask.h"
#include "AliLumiTools.h"
#endif

TString GetMaskMu(int run, double& mu, double xsecBarn = 8.0)
{
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
  AliCDBEntry* entry = man->Get("GRP/CTP/Config");
  const AliTriggerConfiguration* cf = (const AliTriggerConfiguration*)entry->GetObject();
  const AliTriggerBCMask* mask = (const AliTriggerBCMask*)cf->GetMasks()[0];
  TString maskStr = mask->GetTitle();
  int nbc = mask->GetNUnmaskedBCs();
  double lumi = AliLumiTools::GetLumiFromCTP(run)->GetMean(2);
  mu = lumi*xsecBarn / 11245.;
  return maskStr;
}

AliGenerator *GeneratorCustom(){

  Int_t nrun = atoi(gSystem->Getenv("CONFIG_RUN"));
  printf("Configuration for run %d\n",nrun);
  Double_t mu=0.001; 
  TString bcm=GetMaskMu(nrun,mu);
  // swap H and L to match requirement of AliGenPileup
  bcm.ReplaceAll("H","X");
  bcm.ReplaceAll("L","H");
  bcm.ReplaceAll("X","L");
  
  //printf("   BC mask = %s\n",bcm.Data());
  printf("   mu = %f\n",mu);
  printf("   Energy = %f\n",energyConfig);
  
  // Set the pileup interaction generator
  // The second argument is the pileup rate
  // in terms of event rate per bunch crossing
  AliGenCocktail *ctl = (AliGenCocktail*)GeneratorCocktail("HijingPileup");

  // this is the Hijing generator for the trigger event,
  // which could contain an impact parameter cut
  AliGenHijing *hijingC = (AliGenHijing*)GeneratorHijing();
  ctl->AddGenerator(hijingC,   "Hijing", 1.);
  
  AliGenPileup *genpil = new AliGenPileup();
  // this is the Hijing generator for the pileup events,
  // which is configured wihtout impact parameter cuts
  AliGenHijing *hijing = (AliGenHijing*)GeneratorHijing();
  hijing->SetImpactParameterRange(0.,15.);
  genpil->SetGenerator(hijing,mu);
  // Set the beam time structure
  // Details on the syntax in STEER/AliTriggerBCMask
  genpil->SetBCMask(bcm.Data());
  // Do not generate the trigger interaction!
  genpil->GenerateTrigInteraction(kFALSE);
  genpil->Print();
  
  ctl->AddGenerator(genpil,    "Pileup", 1.);
  
  return ctl;

}
