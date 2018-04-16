/*
   rec.C to be used for pass0
   - reconstruction of raw data
   - QA information switched off
   - store all friends
   - default OCDB storage set to "raw://"

   Example:
   aliroot -b -q 'recCPass1.C("raw.root",100)'
*/

void recCPass1_OuterDet(const char *filename="raw.root",Int_t nevents=-1, const char *ocdb="raw://", Bool_t useFullITS=kFALSE)
{

  if (gSystem->AccessPathName("localOCDBaccessConfig.C", kFileExists)==0) {        
    gROOT->LoadMacro("localOCDBaccessConfig.C");
  }

  Int_t useFullITS_int = (Int_t)useFullITS; // convert to integer to be able to call the main macro
  
  // main macro
  Printf("\n\n running reco");
  gROOT->Macro(TString::Format("$ALIDPG_ROOT/DataProc/CPass1/main_recCPass1_OuterDet.C(\"%s\", %d, \"%s\", %d)", filename, nevents, ocdb, useFullITS_int));
}

