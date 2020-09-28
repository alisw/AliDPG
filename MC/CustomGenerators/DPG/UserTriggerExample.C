///
/// \file UserTrigger.C
/// \brief Example of user defined trigger, to be called by main CustomGenerator macro
/// 
/// Example of user trigger with simple multiplicity event selection.
/// In Root5 it must be compiled
///
/// \author Luca Micheletti <luca.micheletti@cern.ch>
/// \author Gustavo Conesa Balbastre <Gustavo.Conesa.Balbastre@cern.ch>


#include "AliStack.h"
#include "AliGenerator.h"

//------
/// User trigger logic to accept the event
/// Here simple multiplicity selection
//------
Bool_t UserTriggerFunction(AliStack *stack)
{
  printf("____________________________\n");
  printf("USER TRIGGER IMPLEMENTATION \n");

  Int_t nTracks  = stack->GetNtrack();
  printf("n Tracks = %i \n",nTracks);
  
  if ( nTracks > 120 )
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
void UserTrigger(AliGenerator * generator)
{
  Bool_t (*funcUserTrigger)(AliStack*) = UserTriggerFunction;
  
//printf("*-*-*-* USER FUNC %p\n",funcUserTrigger);
 
  generator->SetUserTrigger(funcUserTrigger);
}
