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
  
  //printf("   BC mask = %s\n",bcm.Data());
  //printf("   mu = %f\n",mu);
  //printf("   Energy = %f\n",energyConfig);
  
  // Set the pileup interaction generator
  // The second argument is the pileup rate
  // in terms of event rate per bunch crossing
  AliGenCocktail *ctl = (AliGenCocktail*)GeneratorCocktail("EPOS_Pileup");

  // this is the EPOS generator for the trigger event,
  // which could contain an impact parameter cut
  AliGenerator   *epos   = GeneratorEPOSLHC();
  ctl->AddGenerator(epos,  "EPOS-LHC", 1.);
  
  AliGenPileup *genpil = new AliGenPileup();
  // this is the EPOS-LHC generator for the pileup events,
  // which is configured wihtout impact parameter cuts
  AliGenerator * eposPU = GeneratorEPOSLHC(kTRUE);
  //eposPU->SetImpactParameterRange(0.,15.);
  genpil->SetGenerator(eposPU,mu);
  // Set the beam time structure
  // Details on the syntax in STEER/AliTriggerBCMask
  genpil->SetBCMask(bcm.Data());
  // Do not generate the trigger interaction!
  genpil->GenerateTrigInteraction(kFALSE);
  genpil->Print();
  
  ctl->AddGenerator(genpil,    "Pileup", 1.);
  
  return ctl;

}
