//-------------------------------------------------------------------------------------------------//
//              MC production for dimuonmuon J/psi and psi(2s) analysis in pp at 13 TeV            //
//                                                                                                 //
// MC is for a pure signals of J/psi , with y and pT input shapes tuned on pp collected in         //
// 2015 (https://alice-notes.web.cern.ch/node/497)    											   //
// Production should be done on the run-by-run basis. The production is proportional to number of  //
// CMUL7-B-NOPF-MUFAST for a given run.                                                            //
//-------------------------------------------------------------------------------------------------//

#include <Riostream.h>
#include "TRandom.h"
#include "AliGenerator.h"
#include "AliGenCocktail.h"
#include "AliGenEvtGen.h"
#include "AliGenParam.h"

Int_t IpJPsipp13TeV(TRandom *ran);
Int_t IpPsi2spp13TeV(TRandom *ran);

Double_t PtJPsipp13TeV(const Double_t *px, const Double_t * /*dummy*/);
Double_t YJPsipp13TeV(const Double_t *py, const Double_t * /*dummy*/);
Double_t V2JPsipp13TeV(const Double_t *pv, const Double_t * /*dummy*/);

AliGenerator *CreateJpsiGenerator(AliGenEvtGen *evtGener);
AliGenerator *CreatePsi2sGenerator(AliGenEvtGen *evtGener);

//-------------------------------------------------------------------------//
AliGenerator *CreateGenerator()
{
	// create a cocktail of J/psi and psi(2s) generators with radiative decays

	cout << "Setting generator genCocktail" << endl;
	AliGenCocktail *gener = new AliGenCocktail();

	// evtGen (for radiative decays)
	AliGenEvtGen *evtGener = new AliGenEvtGen();
	cout << "evtGener for All " << evtGener << endl;
	evtGener->SetForceDecay(kDiMuon);
	evtGener->SetParticleSwitchedOff(AliGenEvtGen::kAllPart); // can be used by both J/psi and psi(2s)

	// add J/psi generator to cocktail (2/3 weight)
	AliGenerator *jpsiGenerator = CreateJpsiGenerator(evtGener);
	gener->AddGenerator(jpsiGenerator, "Jpsi", 4.0);

	// add psi(2S) generator to cocktail (1/3 weight)
	AliGenerator *psi2sGenerator = CreatePsi2sGenerator(evtGener);
	gener->AddGenerator(psi2sGenerator, "Psi2S", 2.0);

	gener->SetRandomise(kTRUE);
	return gener;
}
//-------------------------------------------------------------------------//

//-------------------------------------------------------------------------//
AliGenerator *CreateJpsiGenerator(AliGenEvtGen *evtGener)
{
	AliGenCocktail *gener = new AliGenCocktail();
	gener->UsePerEventRates();

	AliGenParam *JPsiGener = new AliGenParam(1, -1, PtJPsipp13TeV, YJPsipp13TeV, V2JPsipp13TeV, IpJPsipp13TeV);
	JPsiGener->SetMomentumRange(0., 1.e6);
	JPsiGener->SetPtRange(0., 999.);
	JPsiGener->SetYRange(-4.2, -2.3);
	JPsiGener->SetPhiRange(0., 360.);

	JPsiGener->SetTrackingFlag(1);
	JPsiGener->SetForceDecay(kNoDecay); // JPsi particles decay are switched-off for Pythia

	// add JPsi generator to cocktail
	gener->AddGenerator(JPsiGener, "JPsi", 1);
	// add EvtGen generator to cocktail
	gener->AddGenerator(evtGener, "EvtGen", 1);

	return gener;
}
//-------------------------------------------------------------------------//


//-------------------------------------------------------------------------//
AliGenerator *CreatePsi2sGenerator(AliGenEvtGen *evtGener)
{
	AliGenCocktail *gener = new AliGenCocktail();
	gener->UsePerEventRates();

	//Same as for the J/psi (except the id)
	AliGenParam *Psi2sGener = new AliGenParam(1, -1, PtJPsipp13TeV, YJPsipp13TeV, V2JPsipp13TeV, IpPsi2spp13TeV);
	Psi2sGener->SetMomentumRange(0., 1.e6);
	Psi2sGener->SetPtRange(0., 999.);
	Psi2sGener->SetYRange(-4.2, -2.3);
	Psi2sGener->SetPhiRange(0., 360.);

	Psi2sGener->SetTrackingFlag(1);
	Psi2sGener->SetForceDecay(kNoDecay); // Psi2s particles decay are switched-off for Pythia

	// add JPsi generator to cocktail
	gener->AddGenerator(Psi2sGener, "Psi2s", 1);
	// add EvtGen generator to cocktail
	gener->AddGenerator(evtGener, "EvtGen", 1);

	return gener;
}
//-------------------------------------------------------------------------//


//-------------------------------------------------------------------------//
Int_t IpJPsipp13TeV(TRandom *)
{
	return 443;
}
//-------------------------------------------------------------------------//

//-------------------------------------------------------------------------//
Int_t IpPsi2spp13TeV(TRandom *)
{
	return 100443;
}
//-------------------------------------------------------------------------//


//-------------------------------------------------------------------------//
Double_t PtJPsipp13TeV(const Double_t *px, const Double_t * /*dummy*/)
{
	// jpsi pT in pp at 13 TeV, tuned on data (2015)
	Double_t x = *px;
	Float_t p0, p1, p2, p3;
	p0 = 1;
	p1 = 4.75208;
	p2 = 1.69247;
	p3 = 4.49224;
	return p0 * x / TMath::Power(1. + TMath::Power(x / p1, p2), p3);
}
//-------------------------------------------------------------------------//

//-------------------------------------------------------------------------//
Double_t YJPsipp13TeV(const Double_t *py, const Double_t * /*dummy*/)
{
	// jpsi y in pp at 13 TeV, tuned on data (2015)
	Double_t y = *py;
	Float_t p0, p1, p2;
	p0 = 1;
	p1 = 0;
	p2 = 2.98887;
	return p0 * TMath::Exp(-(1. / 2.) * TMath::Power(((y - p1) / p2), 2));
}
//-------------------------------------------------------------------------//

//-------------------------------------------------------------------------//
Double_t V2JPsipp13TeV(const Double_t * /*dummy*/, const Double_t * /*dummy*/)
{
	//jpsi v2
	return 0.;
}
//-------------------------------------------------------------------------//
