AliGenerator* GeneratorCustom(TString opt = "")
{
  // return one of the parameterized MUON generators according to the parameter opt
  // if opt = "custom" it uses the external parameterization macro MuonGeneratorCustom.C
  // otherwise opt must match one of the options listed below
  // the parameterization macro must implement the function AliGenerator* CreateGenerator();

  // lists of available options and corresponding macros (to be updated each time a new macro is committed)
  // the third list is to activate whatever special setup this option might needed. Currently implemented:
  // - EvtGen --> load the libraries needed to use EvtGen (needed for J/psi radiative decay, ...)
  const Int_t nParametrizations = 1;
  TString optNames[nParametrizations] = {
    "single_pp13TeV_1"
  };
  TString optMacros[nParametrizations] = {
    "Muon_GenParamSingle_pp13TeV_1.C"
  };
  TString optSetups[nParametrizations] = {
    ""
  };

  TString macro = "";
  TString setup = "";
  if (opt == "custom") {
    macro += "MuonGeneratorCustom.C";
  } else {
    for (Int_t iPar = 0; iPar < nParametrizations; iPar++) {
      if (opt == optNames[iPar]) {
        macro += "$ALIDPG_ROOT/MC/CustomGenerators/PWGDQ/";
        macro += optMacros[iPar];
        macro += "+";
        setup += optSetups[iPar];
        break;
      }
    }
  }

  if(macro.IsNull()) {
    Printf("=============================================");
    Printf("Error: Parametrization %s not known -> Abort",opt.Data());
    Printf("=============================================");
    abort();
    return NULL;
  }

  Printf("=============================================");
  Printf("Info:  Selected parametrization %s",opt.Data());
  Printf("=============================================");

  if (setup.Contains("EvtGen")) {
    // load libraries to use EvtGen
    gSystem->Load("libPhotos");
    gSystem->Load("libEvtGen");
    gSystem->Load("libTEvtGen");
  }

  // set external decayer (needed for AliGenParam)
  TVirtualMCDecayer* decayer = new AliDecayerPythia();
  decayer->SetForceDecay(kAll);
  decayer->Init();
  gMC->SetExternalDecayer(decayer);

  // compile the macro (needed to use precompiled functions)
  //gSystem->AddIncludePath("-I$ALICE_ROOT/include -I$ALICE_PHYSICS/include");
  if (gROOT->LoadMacro(macro.Data()) != 0) {
    Printf("ERROR: cannot find %s\n", macro.Data());
    abort();
    return NULL;
  }

  return reinterpret_cast<AliGenerator*>(gROOT->ProcessLineSync("CreateGenerator()"));
}
