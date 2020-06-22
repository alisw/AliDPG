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
  AliGenPileup *genpil = new AliGenPileup();
  
  AliGenHijing *hijing = (AliGenHijing*)GeneratorHijing();
  genpil->SetGenerator(hijing,mu);
  // Set the beam time structure
  // Details on the syntax in STEER/AliTriggerBCMask
  genpil->SetBCMask(bcm.Data());
  // Generate the trigger interaction
  genpil->GenerateTrigInteraction(kTRUE);
  genpil->Print();

  return genpil;

}
