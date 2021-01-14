#if !defined(__CINT__) || defined(__MAKECINT__)
//For ROOT6
R__ADD_INCLUDE_PATH($ALIDPG_ROOT)
#include "Utils/GetTriggerBCMaskAndMu.C"
#endif

AliGenerator *GeneratorCustom(){

  // This is the pp MB Pythia8 pileup generator for pileup background for embedded productions
  // The trigger event is the signal that will be added on top, generated with other macro
  
#if defined(__CINT__)
  // For ROOT5
  gROOT->LoadMacro("$ALIDPG_ROOT/Utils/GetTriggerBCMaskAndMu.C++");
#endif
  
  Double_t mu = 0.001; 
  TString bcm = GetTriggerBCMaskAndMu(mu);
  
  //printf("   BC mask = %s\n",bcm.Data());
  //printf("   mu = %f\n",mu);
  //printf("   Energy = %f\n",energyConfig);
  
  AliGenPileup *genpil = new AliGenPileup();
  // this is the Pythia8 generator for the pileup events,
  // which is configured wihtout impact parameter cuts
  AliGenerator * pythia = GeneratorPythia8();
  genpil->SetGenerator(pythia, mu);
  // Set the beam time structure
  // Details on the syntax in STEER/AliTriggerBCMask
  genpil->SetBCMask(bcm.Data());
  // Do not generate the trigger interaction! This will go in the signal
  genpil->GenerateTrigInteraction(kFALSE);
  genpil->Print();
  
  return genpil;

}
