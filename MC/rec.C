/*
 * AliDPG - ALICE Experiment Data Preparation Group
 * Reconstruction steering script
 *
 */

/*****************************************************************/
/*****************************************************************/
/*****************************************************************/

#if ROOT_VERSION_CODE >= ROOT_VERSION(6,0,0)
#include "ReconstructionConfig.C"
#endif

void rec() 
{

  // reconstruction configuration
#if ROOT_VERSION_CODE < ROOT_VERSION(6,0,0)
  // in root5 the ROOT_VERSION_CODE is defined only in ACLic mode
#else  
  gROOT->LoadMacro("$ALIDPG_ROOT/MC/ReconstructionConfig.C");
#endif
  
  Int_t reconstructionConfig = kReconstructionDefault;
  if (gSystem->Getenv("CONFIG_RECONSTRUCTION")) {
    Bool_t valid = kFALSE;
    for (Int_t irec = 0; irec < kNReconstructions; irec++)
      if (strcmp(gSystem->Getenv("CONFIG_RECONSTRUCTION"), ReconstructionName[irec]) == 0) {
        reconstructionConfig = irec;
        valid = kTRUE;
        break;
      }
    if (!valid) {
      printf(">>>>> Unknown reconstruction configuration: %s \n", gSystem->Getenv("CONFIG_RECONSTRUCTION"));
      abort();
    }
  }

  /* initialisation */
  AliReconstruction rec;
 
  /* configuration */
  ReconstructionConfig(rec, reconstructionConfig);

  /* run */
  rec.Run();

}
