//==========================================//
// Parameterized generator for single muons //
//==========================================//
// Info about this particular tunning:      //
// - PbPb@5.02TeV data (LHC18r)             //
// - CMSL7 events with physics selection    //
// - 50-90% centrality class (V0M)          //
// - Standard muon selection with pDCA      //
// - pT > 1GeV/c                            //
//==========================================//

#include <Riostream.h>
#include "TRandom.h"
#include "AliGenerator.h"
#include "AliGenParam.h"

Int_t IpMuon(TRandom *ran);
Double_t PtMuon(const Double_t *px, const Double_t */*dummy*/);
Double_t YMuon(const Double_t *py, const Double_t */*dummy*/);
Double_t V2Muon(const Double_t */*dummy*/, const Double_t */*dummy*/);

//-------------------------------------------------------------------------
AliGenerator* CreateGenerator()
{
  AliGenParam *singleMu = new AliGenParam(2,-1,PtMuon,YMuon,V2Muon,IpMuon);
  singleMu->SetMomentumRange(0., 1.e6);
  singleMu->SetPtRange(0.8, 999.);
  singleMu->SetYRange(-4.3, -2.3);
  singleMu->SetPhiRange(0., 360.);
  singleMu->SetForceDecay(kNoDecay);
  singleMu->SetTrackingFlag(1);
  return singleMu;
}

//-------------------------------------------------------------------------
Int_t IpMuon(TRandom *ran)
{
  // muon composition

  if (ran->Rndm() < 0.5 )
  {
    return 13;
  }
  else
  {
    return -13;
  }
}

//-------------------------------------------------------------------------
Double_t PtMuon(const Double_t *px, const Double_t */*dummy*/)
{
  // muon pT

  Double_t x=*px;

  Double_t p0 = 797.446;
  Double_t p1 = 0.830278,;
  Double_t p2 = 0.632177;
  Double_t p3 = 10.2202;
  Double_t p4 = -0.000614809;
  Double_t p5 = -1.70993;

  return p0 * (1. / TMath::Power(p1 + TMath::Power(x,p2), p3) + p4 * TMath::Exp(p5*x));
}

//-------------------------------------------------------------------------
Double_t YMuon(const Double_t *py, const Double_t */*dummy*/)
{
  // muon y

  Double_t y = *py;

  Double_t p0 = 1.87732;
  Double_t p1 = 0.00658212;
  Double_t p2 = -0.0988071;
  Double_t p3 = -0.000452746;
  Double_t p4 = 0.00269782;

  return p0 * (1. + p1*y + p2*y*y + p3*y*y*y + p4*y*y*y*y);
}

//-------------------------------------------------------------------------
Double_t V2Muon(const Double_t */*dummy*/, const Double_t */*dummy*/)
{
  // muon v2
  return 0.;
}
