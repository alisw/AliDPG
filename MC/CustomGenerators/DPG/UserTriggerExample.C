///
/// \file UserTriggerExample.C
/// \brief Example of user defined trigger, to be called by main CustomGenerator macro
/// 
/// Example of user trigger with simple multiplicity event selection.
/// In Root5 it must be compiled
/// 
/// Pass to  dpgsim.sh: --generator Pythia6 (EPOSLHC) --gentrigger DPG:UserTriggerExample:110
/// Here 110 is an example of the multiplicity cut to be applied.
/// If one uses --gentrigger DPG:UserTriggerExample the default parameter 120 is used.
///
/// \author Luca Micheletti <luca.micheletti@cern.ch>
/// \author Gustavo Conesa Balbastre <Gustavo.Conesa.Balbastre@cern.ch>


#include "AliStack.h"
#include "AliGenerator.h"
#include "TSystem.h"

//------
/// User trigger logic to accept the event
/// Here simple multiplicity selection
//------
Bool_t UserTriggerFunction(AliStack *stack)
{
  // Default multiplicity cut
  Int_t nMin = 120;
  
  // Recover multiplicity cut from environmental variable
  
  if (gSystem->Getenv("CONFIG_GENTRIGGERPARAM"))
  {
    nMin = atoi(gSystem->Getenv("CONFIG_GENTRIGGERPARAM"));
  }
  
  printf("____________________________\n");
  printf("USER TRIGGER IMPLEMENTATION EXAMPLE: GetNtrack() > %d\n",nMin);

  Int_t nTracks  = stack->GetNtrack();
  printf("n Tracks = %i \n",nTracks);
  
  if ( nTracks > nMin )
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
