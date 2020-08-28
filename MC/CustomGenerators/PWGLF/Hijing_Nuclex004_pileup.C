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
  AliGenCocktail *ctl = (AliGenCocktail*)GeneratorCocktail("Hijing_Nuclex004_PileUp");

  AliGenerator   *nu1a  = Generator_Nuclex(0xF, kFALSE, 10);
  AliGenerator   *nu1b  = Generator_Nuclex(0xF, kTRUE, 10);
  AliGenerator   *nu2a  = Generator_Nuclex(0x10, kFALSE, 40);
  AliGenerator   *nu2b  = Generator_Nuclex(0x10, kTRUE, 40);
  AliGenerator   *nu3a  = Generator_Nuclex(0xC000, kFALSE, 20);
  AliGenerator   *nu3b  = Generator_Nuclex(0xC000, kTRUE, 20);
  ctl->AddGenerator(nu1a,  "Nuclex1a", 1.);
  ctl->AddGenerator(nu1b,  "Nuclex1b", 1.);
  ctl->AddGenerator(nu2a,  "Nuclex2a", 1.);
  ctl->AddGenerator(nu2b,  "Nuclex2b", 1.);
  ctl->AddGenerator(nu3a,  "Nuclex3a", 1.);
  ctl->AddGenerator(nu3b,  "Nuclex3b", 1.);

  // this is the Hijing generator for the trigger event,
  // which could contain an impact parameter cut
  TString simulation = gSystem->Getenv("CONFIG_SIMULATION");
  if(!simulation.Contains("Embed")){
    AliGenHijing *hijingC = (AliGenHijing*)GeneratorHijing();
    
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
    ctl->AddGenerator(hijingC,   "Hijing", 1.);
  }

  return ctl;

}
