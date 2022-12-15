#if !defined(__CINT__) || defined(__MAKECINT__)
#include <Riostream.h>
#include "AliGenerator.h"
#include "AliGenParam.h"
#include "AliGenSTRANGElib.h"
#include "AliGenCocktail.h"
#include "TVirtualMCDecayer.h"
#include "AliDecayerPythia.h"
#include "AliGenPythiaPlus.h"
#endif

AliGenerator *GeneratorCustom(TString opt="a"){

  TString optList[3] = {"a", "b", "c"}; //a = v0 low gen   ,  b = v0 high gen   ,   c = cascades

  const int dim = 10;

  int injpart[6][dim] = {
    {1,2,3,4,5,6,7},          //k0s low gen
    {7,6,5,4,3,2,1},          //lam low gen
    {8,9,10,11,12,13,14,15},  //k0s high gen
    {11,10,9,8,11,10,9,8},    //lam high gen
    {1,2,3,4,5,6,7,8,9,10},   //xi
    {10,9,8,7,6,5,4,3,2,1}    //omega
  };

  int inj_config = 0;
  Int_t inj_sign = (uidConfig%2)? 1 : -1  ;
  if(opt.Contains("a")) inj_config = uidConfig % 7;
  else if(opt.Contains("b")) {
    inj_config = uidConfig % 8;
    inj_sign *= (uidConfig%16>7?-1:1); //fix a recurrent problem due to even configurations (8)
  }
  else if(opt.Contains("c")) {
    inj_config = uidConfig % 10;
    inj_sign *= (uidConfig%20>9?-1:1); //fix a recurrent problem due to even configurations (10)
  }
  else {
    return 0;
  }

  AliGenSTRANGElib *strangelib = new AliGenSTRANGElib();
  
  AliGenCocktail *ctl = (AliGenCocktail*) GeneratorCocktail("Pythia8_MultStrInj_pp5");
  AliGenPythiaPlus *py8  = (AliGenPythiaPlus*) GeneratorPythia8(kPythia8Tune_Monash2013);
  py8->SetTriggerChargedMultiplicity(15,0.5);

  AliGenParam *ik0s = 0x0;
  AliGenParam *ilam = 0x0;
  AliGenParam *ixi  = 0x0;
  AliGenParam *iom  = 0x0;

  if(!opt.Contains("c")) {
     ik0s = new AliGenParam("k0s_injector", injpart[(opt.Contains("a")?0:2)][inj_config], 310, strangelib->GetPt(AliGenSTRANGElib::kK0sPP5), strangelib->GetY(AliGenSTRANGElib::kK0sPP5) );
     ilam = new AliGenParam("lam_injector", injpart[(opt.Contains("a")?1:3)][inj_config], inj_sign*3122, strangelib->GetPt(AliGenSTRANGElib::kLambdaPP5), strangelib->GetY(AliGenSTRANGElib::kLambdaPP5) );
  }
  else{
     ixi  = new AliGenParam("xi_injector", injpart[4][inj_config], inj_sign*3312, strangelib->GetPt(AliGenSTRANGElib::kXiPP5), strangelib->GetY(AliGenSTRANGElib::kXiPP5) );
     iom  = new AliGenParam("om_injector", injpart[5][inj_config], -inj_sign*3334, strangelib->GetPt(AliGenSTRANGElib::kOmegaPP5), strangelib->GetY(AliGenSTRANGElib::kOmegaPP5) );
  }

  TVirtualMCDecayer* decayer = new AliDecayerPythia();

  if(!opt.Contains("c")) {
     ik0s->SetDecayer(decayer);
     ik0s->SetMomentumRange(0,1e6);
     ik0s->SetPtRange(0.,20.);
     ik0s->SetYRange(-0.6,0.6);
     ik0s->SetPhiRange(0., 360.);
     ik0s->SetForceDecay(kNoDecay);
     ik0s->SetTrackingFlag(1);

     ilam->SetDecayer(decayer);
     ilam->SetMomentumRange(0,1e6);
     ilam->SetPtRange(0.,20.);
     ilam->SetYRange(-0.6,0.6);
     ilam->SetPhiRange(0., 360.);
     ilam->SetForceDecay(kNoDecay);
     ilam->SetTrackingFlag(1);
  }
  else{
     ixi->SetDecayer(decayer);
     ixi->SetMomentumRange(0,1e6);
     ixi->SetPtRange(0.,20.);
     ixi->SetYRange(-0.6,0.6);
     ixi->SetPhiRange(0., 360.);
     ixi->SetForceDecay(kNoDecay);
     ixi->SetTrackingFlag(1);

     iom->SetDecayer(decayer);
     iom->SetMomentumRange(0,1e6);
     iom->SetPtRange(0.,20.);
     iom->SetYRange(-0.6,0.6);
     iom->SetPhiRange(0., 360.);
     iom->SetForceDecay(kNoDecay);
     iom->SetTrackingFlag(1);
  }

  ctl->AddGenerator(py8  , "Pythia8 (Monash2013)", 1.);

  if(!opt.Contains("c")){
    ctl->AddGenerator(ik0s , "Injector (K0s)", 1.);
    ctl->AddGenerator(ilam , "Injector (Lambda)", 1.);
  }
  else{
    ctl->AddGenerator(ixi  , "Injector (Xi)", 1.);
    ctl->AddGenerator(iom  , "Injector (Omega)", 1.);
  }

  return ctl;

}
