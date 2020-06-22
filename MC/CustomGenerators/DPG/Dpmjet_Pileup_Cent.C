#if !defined(__CINT__) || defined(__MAKECINT__)
//For ROOT6
R__ADD_INCLUDE_PATH($ALIDPG_ROOT)
#include "Utils/GetTriggerBCMaskAndMu.C"
#endif

AliGenerator *GeneratorCustom(){

#if defined(__CINT__)
  // For ROOT5
  gROOT->LoadMacro("$ALIDPG_ROOT/Utils/GetTriggerBCMaskAndMu.C++");
#endif

  Double_t mu=0.001; 
  TString bcm=GetTriggerBCMaskAndMu(mu);
  
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
  AliGenCocktail *ctl = (AliGenCocktail*)GeneratorCocktail("DpmjetPileup");

  // this is the DPMJET generator for the trigger event,
  // which could contain an impact parameter cut
  AliGenerator   *dpm   = GeneratorPhojet();
  ctl->AddGenerator(dpm,  "Dpmjet", 1.);
  
  AliGenPileup *genpil = new AliGenPileup();
  // this is the Hijing generator for the pileup events,
  // which is configured wihtout impact parameter cuts
  AliGenerator *dmpPU = (AliGenerator*) GeneratorPhojet();
  //dpmPU->SetImpactParameterRange(0.,15.);
  genpil->SetGenerator(dmpPU,mu);
  // Set the beam time structure
  // Details on the syntax in STEER/AliTriggerBCMask
  genpil->SetBCMask(bcm.Data());
  // Do not generate the trigger interaction!
  genpil->GenerateTrigInteraction(kFALSE);
  genpil->Print();
  
  ctl->AddGenerator(genpil,    "Pileup", 1.);
  
  return ctl;

}
