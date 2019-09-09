//--------------------------------------------------------------------------------------------------//
//                     MC production for muon J/psi analysis in PbPb at 5.02 TeV                    //
//                                                                                                  //
// MC is for a pure signals of J/psi , with y and pT input shapes tuned on PbPb collected in 2015   //
// Production should be done on the run-by-run basis. The production is proportional to number of   //
// CMUL7-B-NOPF-MUFAST for a given run.                                                             //
//--------------------------------------------------------------------------------------------------//


#if !defined(__CINT__) || defined(__MAKECINT__)
#include <Riostream.h>
#include "TRandom.h"
#include "AliGenerator.h"
#include "AliGenLib.h"
#include "AliGenParam.h"
#include "AliGenCocktail.h"
#include "AliGenEvtGen.h"

#endif

static Int_t IpJPsiPbPb( TRandom *ran);
static Double_t PtJPsiPbPb( const Double_t *px, const Double_t */*dummy*/ );
static Double_t YJPsiPbPb( const Double_t *py, const Double_t */*dummy*/ );
static Double_t V2JPsiPbPb( const Double_t *pv, const Double_t */*dummy*/ );

//-------------------------------------------------------------------------
AliGenerator* CreateGenerator()
{
	AliGenCocktail *gener = new AliGenCocktail();
	gener->UsePerEventRates();

  AliGenParam *JPsiGener = new AliGenParam(1,-1,PtJPsiPbPb,YJPsiPbPb,V2JPsiPbPb,IpJPsiPbPb);
  JPsiGener->SetMomentumRange(0,1e6);
  JPsiGener->SetPtRange(0,999.);
  JPsiGener->SetYRange(-4.2, -2.3);
  JPsiGener->SetPhiRange(0., 360.);

	JPsiGener->SetTrackingFlag(1);
	JPsiGener->SetForceDecay(kNoDecay);  // JPsi particles decay are switched-off for Pythia

	// evtGen (for radiative decays)
	AliGenEvtGen *evtGener = new AliGenEvtGen();
	evtGener->SetForceDecay(kDiMuon);
	evtGener->SetParticleSwitchedOff(AliGenEvtGen::kCharmPart);

	// add JPsi generator to cocktail
	gener->AddGenerator(JPsiGener,"JPsi",1);
	// add EvtGen generator to cocktail
	gener->AddGenerator(evtGener,"EvtGen",1);

  return gener;
}

//-------------------------------------------------------------------------
Int_t IpJPsiPbPb( TRandom *)
{
  return 443;
}

//-------------------------------------------------------------------------
Double_t PtJPsiPbPb( const Double_t *px, const Double_t */*dummy*/ )
{
  // jpsi pT in PbPb, tuned on data (2015) -> Castillo embedding https://alice.its.cern.ch/jira/browse/ALIROOT-8174?jql=text%20~%20%22LHC19a2%22
	Double_t x=*px;
	Float_t p0,p1,p2,p3;
	p0 = 1.00715e6;
	p1 = 3.50274;
	p2 = 1.93403;
	p3 = 3.96363;
	return p0 *x / TMath::Power( 1. + TMath::Power(x/p1,p2), p3 );
}

//-------------------------------------------------------------------------
Double_t YJPsiPbPb( const Double_t *py, const Double_t */*dummy*/ )
{
  // jpsi y in PbPb, tuned on data (2015) -> Castillo embedding https://alice.its.cern.ch/jira/browse/ALIROOT-8174?jql=text%20~%20%22LHC19a2%22
	Double_t y = *py;
	Float_t p0,p1,p2;
	p0 = 1.09886e6;
	p1 = 0;
	p2 = 2.12568;
	return p0*TMath::Exp(-(1./2.)*TMath::Power(((y-p1)/p2),2));
}

//-------------------------------------------------------------------------
Double_t V2JPsiPbPb( const Double_t */*dummy*/, const Double_t */*dummy*/ )
{
  //jpsi v2
  return 0.;
}
