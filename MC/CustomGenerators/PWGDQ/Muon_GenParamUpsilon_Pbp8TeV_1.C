//--------------------------------------------------------------------------------------------------//
//                     MC production for muon Upsilon analysis in Pbp at 8.16TeV                    //
//                                                                                                  //
// MC is for a pure signals of Upsilon(1S), Upsilon(2S) and Upsilon(3S), with y and pT input shapes //
// extracted by fitting the Upsilon yields. ( The input shapes are the same for 1S, 2S and 3S )     //
// Production should be done on the run-by-run basis. The production is proportional to number of   //
// CMUL7-B-NOPF-MUFAST for a given run.                                                             //
//--------------------------------------------------------------------------------------------------//

#include <Riostream.h>
#include "TRandom.h"
#include "AliGenerator.h"
#include "AliGenCocktail.h"
#include "AliGenEvtGen.h"
#include "AliGenParam.h"

AliGenerator* Upsilon1SGenerator(AliGenEvtGen *evtGener);
AliGenerator* Upsilon2SGenerator(AliGenEvtGen *evtGener);
AliGenerator* Upsilon3SGenerator(AliGenEvtGen *evtGener);
Int_t IpUpsilon1S(TRandom */*dummy*/);
Int_t IpUpsilon2S(TRandom */*dummy*/);
Int_t IpUpsilon3S(TRandom */*dummy*/);
Double_t PtUpsilon(const Double_t *x, const Double_t */*dummy*/);
Double_t YUpsilon(const Double_t *x, const Double_t */*dummy*/);
Double_t V2Upsilon(const Double_t */*dummy*/, const Double_t */*dummy*/);

//-------------------------------------------------------------------------
AliGenerator* CreateGenerator()
{
  // create a cocktail of Upsilon(1S), (2S) and (3S) generators with radiative decays

  cout << "Setting generator genCocktail" << endl;
  AliGenCocktail *gener = new AliGenCocktail();

  // evtGen (for radiative decays)
  AliGenEvtGen *evtGener = new AliGenEvtGen();
  cout << "evtGener for All " << evtGener << endl;
  evtGener->SetForceDecay(kDiMuon);
  evtGener->SetParticleSwitchedOff(AliGenEvtGen::kAllPart); // can be used by both J/psi and Upsilon

  // add Upsilon(1S) generator to cocktail
  AliGenerator *upsilon1SGener = Upsilon1SGenerator(evtGener);
  gener->AddGenerator(upsilon1SGener,"Upsilon1S",4.0);

  // add Upsilon(2S) generator to cocktail
  AliGenerator *upsilon2SGener = Upsilon2SGenerator(evtGener);
  gener->AddGenerator(upsilon2SGener,"Upsilon2S",2.0);

  // add Upsilon(3S) generator to cocktail
  AliGenerator *upsilon3SGener = Upsilon3SGenerator(evtGener);
  gener->AddGenerator(upsilon3SGener,"Upsilon3S",0.5);

  gener->SetRandomise(kTRUE);
  return gener;
}

//-------------------------------------------------------------------------
AliGenerator* Upsilon1SGenerator(AliGenEvtGen *evtGener)
{
  // Upsilon(1S) generator with radiative decays

  AliGenCocktail *gener = new AliGenCocktail();
  gener->UsePerEventRates();

  // Particle generator
  AliGenParam* upsilonGener = new AliGenParam(1,-1,PtUpsilon,YUpsilon,V2Upsilon,IpUpsilon1S);
  upsilonGener->SetMomentumRange(0., 1.e6);
  upsilonGener->SetPtRange(0., 999.);
  upsilonGener->SetYRange(-4.2, -2.3);
  upsilonGener->SetPhiRange(0., 360.);
  upsilonGener->SetTrackingFlag(1);
  upsilonGener->SetForceDecay(kNoDecay);  // Upsilon particles decay are switched-off for Pythia

  // add Upsilon(1S) generator to cocktail
  gener->AddGenerator(upsilonGener,"Upsilon1S",1);
  // add EvtGen generator to cocktail
  gener->AddGenerator(evtGener,"EvtGen",1);

  return gener;
}

//-------------------------------------------------------------------------
AliGenerator* Upsilon2SGenerator(AliGenEvtGen *evtGener)
{
  // Upsilon(2S) generator with radiative decays

  AliGenCocktail *gener = new AliGenCocktail();
  gener->UsePerEventRates();

  // Particle generator
  AliGenParam* upsilonGener = new AliGenParam(1,-1,PtUpsilon,YUpsilon,V2Upsilon,IpUpsilon2S);
  upsilonGener->SetMomentumRange(0., 1.e6);
  upsilonGener->SetPtRange(0., 999.);
  upsilonGener->SetYRange(-4.2, -2.3);
  upsilonGener->SetPhiRange(0., 360.);
  upsilonGener->SetTrackingFlag(1);
  upsilonGener->SetForceDecay(kNoDecay);  // Upsilon particles decay are switched-off for Pythia

  // add Upsilon(2S) generator to cocktail
  gener->AddGenerator(upsilonGener,"Upsilon2S",1);
  // add EvtGen generator to cocktail
  gener->AddGenerator(evtGener,"EvtGen",1);

  return gener;
}

//-------------------------------------------------------------------------
AliGenerator* Upsilon3SGenerator(AliGenEvtGen *evtGener)
{
  // Upsilon(3S) generator with radiative decays

  AliGenCocktail *gener = new AliGenCocktail();
  gener->UsePerEventRates();

  // Particle generator
  AliGenParam* upsilonGener = new AliGenParam(1,-1,PtUpsilon,YUpsilon,V2Upsilon,IpUpsilon3S);
  upsilonGener->SetMomentumRange(0., 1.e6);
  upsilonGener->SetPtRange(0., 999.);
  upsilonGener->SetYRange(-4.2, -2.3);
  upsilonGener->SetPhiRange(0., 360.);
  upsilonGener->SetTrackingFlag(1);
  upsilonGener->SetForceDecay(kNoDecay);  // Upsilon particles decay are switched-off for Pythia

  // add Upsilon(3S) generator to cocktail
  gener->AddGenerator(upsilonGener,"Upsilon3S",1);
  // add EvtGen generator to cocktail
  gener->AddGenerator(evtGener,"EvtGen",1);

  return gener;
}

//-------------------------------------------------------------------------
Int_t IpUpsilon1S(TRandom */*dummy*/)
{
  return 553;
}

//-------------------------------------------------------------------------
Int_t IpUpsilon2S(TRandom */*dummy*/)
{
  return 100553;
}

//-------------------------------------------------------------------------
Int_t IpUpsilon3S(TRandom */*dummy*/)
{
  return 200553;
}

//-------------------------------------------------------------------------
Double_t PtUpsilon(const Double_t *x, const Double_t */*dummy*/)
{
  // Upsilon pT tuned on accEff corrected Upsilon(1S) yields in Pbp data at 8.16TeV

  Double_t pT = *x;

  Double_t p0 = 2.46580e+02;
  Double_t p1 = 5.24198e+00;
  Double_t p2 = 2.71097e+00;
  Double_t p3 = 1.72578e+00;

  return p0 * pT / TMath::Power(1. + TMath::Power(pT/p1,p2), p3);
}

//-------------------------------------------------------------------------
Double_t YUpsilon(const Double_t *x, const Double_t */*dummy*/)
{
  // Upsilon y tuned on accEff corrected Upsilon(1S) yields in Pbp data at 8.16TeV

  Double_t y = *x;

  Double_t p0 = 2.51505e+03;
  Double_t p1 = -3.85886e-02;

  return p0 * (1. + p1*y*y);
}

//-------------------------------------------------------------------------
Double_t V2Upsilon(const Double_t */*dummy*/, const Double_t */*dummy*/)
{
  return 0.;
}
