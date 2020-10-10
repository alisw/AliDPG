///
/// \file V0MultiplicityGenTrigger.C
/// \brief Select generated events depending on particle multiplicity on V0 acceptance within pT cuts
/// 
/// Selection of events depending on generated particle multiplicity on V0 acceptance
/// Extracted from AliAnalysisTaskGenMCRidge::GetMultiplicity()
/// Count primary particles in V0 acceptance above and below a pT cut, and accept the event
/// when multiplicity is the requested one.
///
/// Pass to dpgsim.sh: --generator Pythia6 or EPOSLHC or generators using AliGenExtFile
///                    --gentrigger DPG:V0MultiplicityGenTrigger (use default cut paramters)
///                    --gentrigger DPG:V0MultiplicityGenTrigger:xx_yy_zz ( count particles using as cut nMin = xx,  ptMin = yy,  ptMax = zz)
/// It is possible to specify only xx or xx_yy or xx_yy_zz, if you want to set zz, you need to set the others, 
/// if you want to set yy, you need to set xx
///
/// \author Luca Micheletti <luca.micheletti@cern.ch>
/// \author Gustavo Conesa Balbastre <Gustavo.Conesa.Balbastre@cern.ch>


#include "AliStack.h"
#include "AliGenerator.h"
#include "AliPWG0Helper.h"
#include <TDatabasePDG.h>
#include <TParticle.h>
#include <TSystem.h>
#include <TObjString.h>

//------
/// User trigger logic to accept the event
//------
Bool_t UserTriggerFunction(AliStack *stack)
{
  // default parameter cuts
  Int_t    nMin = 103; 
  Float_t ptMin = 0; 
  Float_t ptMax = 1.e4; 
  
  // Recover multiplicity cut from environmental variable
  
  TString param = gSystem->Getenv("CONFIG_GENTRIGGERPARAM");
  if ( param!="" )
  {
    TObjArray *oa = param.Tokenize("_");
    if ( oa->At(0) ) nMin  = atoi( ( (TObjString*) oa->At(0) )->GetString() );
    if ( oa->At(1) ) ptMin = atof( ( (TObjString*) oa->At(1) )->GetString() );
    if ( oa->At(2) ) ptMax = atof( ( (TObjString*) oa->At(2) )->GetString() );
  }
  
  printf("____________________________\n");
  printf("USER TRIGGER IMPLEMENTATION: nTrack >= %d, %2.2f <= pT < %2.2f GeV/c \n",nMin, ptMin, ptMax);
  
  Int_t nTracks  = stack->GetNtrack();
  
  Int_t ntrk = 0;
  Int_t nprim = stack->GetNprimary();
  
  for(int i=0;i<stack->GetNprimary();i++)
  {
    TParticle* track = stack->Particle(i); 
    if( !track )
      continue; 
    
    if( !AliPWG0Helper::IsPrimaryCharged(track, stack->GetNprimary()) ) 
      continue; 
    
    Int_t pdg = track->GetPdgCode();
    Double_t chp = TDatabasePDG::Instance()->GetParticle(pdg)->Charge();
    
    if( chp == 0 )
      continue; 
    
    if( !( ( track->Eta() > 2.8 && track->Eta() < 5.1 ) || ( track->Eta() > -3.7 && track->Eta() < -1.7 ) ) )
      continue;
    
    Float_t pt = track->Pt();
    if ( pt >= ptMin && pt < ptMax )
      ntrk++;
  }

  printf("n Tracks = %i n_prim = %i n_primchg|V0_acc = %i \n", nTracks, nprim, ntrk);

  // https://alice.its.cern.ch/jira/browse/ALIROOT-8533?filter=10512
  //
  // The minimum Nch in the particle level for corresponding 0-0.1% of  V0M 
  // was studied and we got the following numbers for each configurations.
  // String Shoving, g=3 : 108
  // PYTHIA 4c tune : 105
  // EPOS : 110
  // PYTHIA Monash2013 : 103
  //
  // So I think maybe we can just ask V0M in the particle level(Nch_V0M) >=103 or 100 
  // or sharp cut on each generators.
 
  
  if ( ntrk >= nMin )
  {
    printf("\t accepted!\n");
    return kTRUE;
  }
  else
  {
    printf("\t rejected!\n");
    return kFALSE;
  }
}

//------
/// Main, pass the generator and set the user trigger
//------
void GenTriggerCustom(AliGenerator * generator)
{
  Bool_t (*funcUserTrigger)(AliStack*) = UserTriggerFunction;
  
//printf("*-*-*-* USER FUNC %p\n",funcUserTrigger);
 
  generator->SetUserTrigger(funcUserTrigger);
}
