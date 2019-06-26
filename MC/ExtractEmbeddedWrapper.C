/*
 * AliDPG - ALICE Experiment Data Preparation Group
 * Embedded signal filtering main script
 *
 */

/*****************************************************************/
/*****************************************************************/
/*****************************************************************/

#if ROOT_VERSION_CODE >= ROOT_VERSION(6,0,0)
#include "ExtractEmbedded.C"
#endif

void ExtractEmbeddedWrapper(Bool_t keepMixed = kFALSE) 
{

  Printf("Run the extraction of embedded signal (keepMixed = %d)",keepMixed);

  gSystem->Exec("ln -s $ALIDPG_ROOT/MC/ExtractEmbedded.C ExtractEmbedded.C");
  if(gROOT->LoadMacro("ExtractEmbedded.C+") != 0) {
    Printf("ERROR: Problem when loading ExtractEmbedded.C");
    abort();
    return;
  }

  if(ExtractEmbedded(keepMixed,"AliESDs_EMB.root","AliESDs.root", "AliESDfriends_EMB.root", "AliESDfriends.root") == kFALSE){
    Printf("ERROR: Problem when running ExtractEmbedded");
    abort();
    return;
  }

  //// Remove for proper handling of AliMultSelection task
  // if(RemoveBKGFromGalice("galice_EMB.root","galice.root") == kFALSE){
  //   Printf("ERROR: Problem when running RemoveBKGFromGalice");
  //   abort();
  //   return;
  // }

  gSystem->Exec("rm ExtractEmbedded.C ExtractEmbedded_C.*");
  
}
