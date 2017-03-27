AliGenerator *
GeneratorCustom()
{
  AliGenCocktail *ctl = GeneratorCocktail("EPOSLHC_Jpsiee002");
  AliGenerator *epos = GeneratorEPOSLHC();
  ctl->AddGenerator(epos, "EPOS-LHC", 1.);
  if (uidConfig % 10 < 7)
    { // (params, jpsifrac, lowfrac, highfrac, bfrac) 
      AliGenerator *jpsi = Generator_Jpsiee("pPb 5.03", 0.7, 0.0, 0.3, 0.0); 
      ctl->AddGenerator(jpsi, "Jpsi2ee", 1., NULL, 3); 
      TFile *file = new TFile("typeHF_4.proc", "recreate"); 
      file->Close(); 
    }
  else
    { 
      AliGenerator *bjpsi = Generator_Jpsiee("Pythia BBar", 0.0, 0.0, 0.0, 1.0); 
      ctl->AddGenerator(bjpsi, "B2Jpsi2ee", 1., NULL, 1); 
      TFile *file = new TFile("typeHF_5.proc", "recreate"); 
      file->Close();
    }

  return ctl;
}
