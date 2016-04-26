/*
 * AliDPG - ALICE Experiment Data Preparation Group
 * Reconstruction steering script
 *
 */

/*****************************************************************/
/*****************************************************************/
/*****************************************************************/

void rec() 
{

  // run number
  Int_t runNumber = -1;
  if (gSystem->Getenv("CONFIG_RUN"))
    runNumber = atoi(gSystem->Getenv("CONFIG_RUN"));
  if (runNumber <= 0) {
    printf("Invalid run number: %d \n", runNumber);
    abort();
  }

  // reconstruction configuration
  gROOT->LoadMacro("$ALIDPG_MC/ReconstructionConfig.C");
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
  ReconstructionConfig(rec, reconstructionConfig, runNumber);

  /* run */
  rec.Run();

}
