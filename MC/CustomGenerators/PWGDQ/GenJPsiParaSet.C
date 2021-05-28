/**********************************************************************
 Created on : 21/05/2019
 Purpose    : paraset for prompt JPsi
**********************************************************************/

#if !defined(__CINT__) || defined(__MAKECINT__)
#include "TRandom.h"
#include "AliGenParam.h"
#endif 

//           generator functions
//


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Double_t  V2Zero_M( const Double_t* /*px*/, const Double_t */*dummy*/ )
{

  return 0.0;

}
 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------
Int_t IpEtac_M(TRandom *)
{
  // J/Psi composition
  return 441;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------
Int_t IpJpsi_M(TRandom *)
{
  // J/Psi composition
  return 443;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Double_t PtJpsiPbPb5020_0020(const Double_t *px, const Double_t *dummy)
{
  // J/Psi pT
  // PbPb 5.02 TeV, 0-20 %
  //
  const Double_t kC    = 4.28158e-02;
  const Double_t kpt0  = 4.87205e+00;
  const Double_t kn    = 5.84596e+00;

  Double_t pt          = px[0];
  
  return kC * pt /TMath::Power((1. + (pt/kpt0)*(pt/kpt0)),kn);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Double_t PtJpsiPbPb5020_0010(const Double_t *px, const Double_t *dummy)
{
  // J/Psi pT
  // PbPb 5.02 TeV, 0-10 % (2018)
  //
  const Double_t kC    = 4.93446e-02;
  const Double_t kpt0  = 3.19847e+00;
  const Double_t kn    = 3.33812e+00;

  Double_t pt          = px[0];

  return kC * pt /TMath::Power((1. + (pt/kpt0)*(pt/kpt0)),kn);
}


Double_t PtJpsiPbPb5020_2040(const Double_t *px, const Double_t *dummy)
{
  // J/Psi pT
  // PbPb 5.02 TeV, 20-40 %
  //
  const Double_t kC    = 1.43606e-02;
  const Double_t kpt0  = 3.20879e+00;
  const Double_t kn    = 3.22259e+00;
  Double_t pt          = px[0];

  return kC * pt /TMath::Power((1. + (pt/kpt0)*(pt/kpt0)),kn);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Double_t PtJpsiPbPb5020_3050(const Double_t *px, const Double_t *dummy)
{
  // J/Psi pT
  // PbPb 5.02 TeV, 0-10 % (2018)
  //
  const Double_t kC    = 5.06325e-03;
  const Double_t kpt0  = 3.22806e+00;
  const Double_t kn    = 2.95146e+00;

  Double_t pt          = px[0];

  return kC * pt /TMath::Power((1. + (pt/kpt0)*(pt/kpt0)),kn);
}


Double_t PtJpsiPbPb5020_4090(const Double_t *px, const Double_t *dummy)
{
  // J/Psi pT
  // PbPb 5.02 TeV, 40-90 %
  //
  const Double_t kC    = 1.61589e-03;
  const Double_t kpt0  = 3.39896e+00;
  const Double_t kn    = 3.04499e+00;
  Double_t pt          = px[0];
  
  return kC * pt /TMath::Power((1. + (pt/kpt0)*(pt/kpt0)),kn);
}

Double_t PtJpsipp_13TeV(const Double_t *px, const Double_t *dummy)
{
  // prompt J/Psi pT
  // pp, 13TeV
  //
  const Double_t kC    = 2.28550e+00;
  const Double_t kpt0  = 3.73619e+00;
  const Double_t kn    = 2.81708e+00;
  Double_t pt          = px[0];

  return kC * pt /TMath::Power((1. + (pt/kpt0)*(pt/kpt0)),kn);
}

Double_t YJpsiFlat( const Double_t */*x*/, const Double_t */*dummy*/ )
{
  return 1.;
}
//---------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//           generators
//
AliGenParam* GenJPsiParaSet(TString option)
{
  printf("\nProcessing config setup : JPsiParaSet \n\n");
  printf("Parameter option %s ",option.Data());

  
  AliGenParam *genJpsi = 0x0;
  if(option == "UserParam_PbPb5TeV_minbias")    genJpsi = new AliGenParam(1,-1, PtJpsiPbPb5020_0010, YJpsiFlat, V2Zero_M, IpJpsi_M);
  else if(option == "UserParam_PbPb5TeV_0010")  genJpsi = new AliGenParam(1,-1, PtJpsiPbPb5020_0010, YJpsiFlat, V2Zero_M, IpJpsi_M);
  else if(option == "UserParam_PbPb5TeV_0020")  genJpsi = new AliGenParam(1,-1, PtJpsiPbPb5020_0020, YJpsiFlat, V2Zero_M, IpJpsi_M);
  else if(option == "UserParam_PbPb5TeV_2040")  genJpsi = new AliGenParam(1,-1, PtJpsiPbPb5020_2040, YJpsiFlat, V2Zero_M, IpJpsi_M);
  else if(option == "UserParam_PbPb5TeV_3050")  genJpsi = new AliGenParam(1,-1, PtJpsiPbPb5020_3050, YJpsiFlat, V2Zero_M, IpJpsi_M);
  else if(option == "UserParam_PbPb5TeV_4090")  genJpsi = new AliGenParam(1,-1, PtJpsiPbPb5020_4090, YJpsiFlat, V2Zero_M, IpJpsi_M);
  else if(option == "UserParam_pp13TeV")        genJpsi = new AliGenParam(1,-1, PtJpsipp_13TeV,      YJpsiFlat, V2Zero_M, IpJpsi_M);
  else if(option == "UserParam_pp13TeV_EtaC")   genJpsi = new AliGenParam(1,-1, PtJpsipp_13TeV,      YJpsiFlat, V2Zero_M, IpEtac_M);
  return genJpsi;
}
