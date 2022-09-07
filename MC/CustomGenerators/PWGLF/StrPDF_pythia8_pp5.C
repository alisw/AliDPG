#if !defined(__CINT__) || defined(__MAKECINT__)
#include <Riostream.h>
#include "AliGenerator.h"
#include "AliGenParam.h"
#include "AliGenSTRANGElib.h"
#include "AliGenCocktail.h"
#include "TVirtualMCDecayer.h"
#include "AliDecayerPythia.h"
#endif

AliGenerator *GeneratorCustom(){

  Int_t inj_config = uidConfig % 7; 
  Int_t inj_sign = (uidConfig%2)? 1 : -1  ;
  
  const int dim = 7; //nijection required
  Int_t k0s_array[dim] = {4,5,6,7,8,9,10}; //index: npart injected 
  Int_t xi_array[dim] = {8,7,6,5,4,3,2}; //index: npart injected  
  
  AliGenSTRANGElib *strangelib = new AliGenSTRANGElib();
  
  AliGenCocktail *ctl = (AliGenCocktail*) GeneratorCocktail("Pythia8_MultStrInj_pp5");
  AliGenerator *py8  = GeneratorPythia8(kPythia8Tune_Monash2013);
  AliGenParam *ik0s = new AliGenParam("k0s_injector", k0s_array[inj_config], 310, strangelib->GetPt(AliGenSTRANGElib::kK0sPP5), strangelib->GetY(AliGenSTRANGElib::kK0sPP5) ); 
  AliGenParam *ixi = new AliGenParam("xi_injector", xi_array[inj_config], inj_sign * 3312, strangelib->GetPt(AliGenSTRANGElib::kXiPP5), strangelib->GetY(AliGenSTRANGElib::kXiPP5) );

  TVirtualMCDecayer* decayer = new AliDecayerPythia();
  decayer->SetForceDecay(kAll);
  decayer->Init();
  
  ik0s->SetDecayer(decayer);
  ik0s->SetMomentumRange(0,1e6);
  ik0s->SetPtRange(0.,20.);
  ik0s->SetYRange(-0.7,0.7);
  ik0s->SetPhiRange(0., 360.);
  ik0s->SetForceDecay(kAll);
  ik0s->SetTrackingFlag(1);

  ixi->SetDecayer(decayer);
  ixi->SetMomentumRange(0,1e6);
  ixi->SetPtRange(0.,20.);
  ixi->SetYRange(-0.7,0.7);
  ixi->SetPhiRange(0., 360.);
  ixi->SetForceDecay(kAll);
  ixi->SetTrackingFlag(1);


  ctl->AddGenerator(py8 , "Pythia8 (Monash2013)", 1.);
  ctl->AddGenerator(ik0s , "Injector (K0s)", 1.);
  ctl->AddGenerator(ixi , "Injector (Xi)", 1.);
  
  return ctl;

}
