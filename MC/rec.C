/*
 * AliDPG - ALICE Experiment Data Preparation Group
 * Reconstruction steering script
 *
 */

/*****************************************************************/
/*****************************************************************/
/*****************************************************************/

#include "ReconstructionConfig.C"

void rec() 
{

  // reconstruction configuration
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
