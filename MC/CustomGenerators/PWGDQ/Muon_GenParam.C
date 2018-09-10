#if ROOT_VERSION_CODE >= ROOT_VERSION(6,0,0)
#include "Muon_GenParamCustomSingle.C"
#endif

AliGenerator* GeneratorCustom(TString opt = "par_2016_1")
{
  // single muon generation with ad-hoc functions
  
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,0,0)
  // in root5 the ROOT_VERSION_CODE is defined only in ACLic mode
#else
  // compile the macro (needed to use precompiled functions)
  //gSystem->AddIncludePath("-I$ALICE_ROOT/include -I$ALICE_PHYSICS/include");
  if (gROOT->LoadMacro("$ALIDPG_ROOT/MC/CustomGenerators/PWGDQ/Muon_GenParamCustomSingle_2016_1.C+") != 0) {
    printf("ERROR: cannot find Muon_GenParamCustomSingle_2016_1\n");
    abort();
    return;
  }
#endif
  
  // set external decayer (needed for AliGenParam)
  TVirtualMCDecayer* decayer = new AliDecayerPythia();
  decayer->SetForceDecay(kAll);
  decayer->Init();
  gMC->SetExternalDecayer(decayer);
  
  const Int_t nParametrizations = 1;
  TString parList[nParametrizations] = {"par_2016_1"};
  Int_t iParametrization = -1;
  for (Int_t iPar = 0; iPar < nParametrizations; iPar++){
    if (opt.Contains(parList[iPar]))
      iParametrization = iPar;
  }

  if(iParametrization!=-1){
    Printf("=============================================");
    Printf("Info:  Selected parametrization %s",opt.Data());
    Printf("=============================================");
    
    if(iParametrization==0)
      return Muon_GenParamCustomSingle_2016_1();

  }
  else{
    Printf("=============================================");
    Printf("Error: Parametrization %s not known -> Abort",opt.Data());
    Printf("=============================================");
    return NULL;
  }
}

