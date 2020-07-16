AliGenerator *
GeneratorCustom(TString ptParamPrompt="UserParam_PbPb5TeV_0010", Bool_t useEventbyEventCent = kFALSE)
{
  TString simulation = gSystem->Getenv("CONFIG_SIMULATION");
    
  AliGenCocktail *ctl = (AliGenCocktail*) GeneratorCocktail("Hijing_Jpsiee002");
  
  if(!simulation.Contains("Embed")){
    AliGenerator *hij = GeneratorHijing();
    ctl->AddGenerator(hij, "Hijing", 1.);
  }

  // centrality dependent injection
  Int_t ntimes = 1;
  Double_t bEvalFormula = -1.;

  TFormula *formula = new TFormula("Signals","(x<14.0)*(11.0-0.4*x-0.02*x*x) + (x>14.0)*2.0");
  if(!useEventbyEventCent){
    if(bminConfig>bmaxConfig)Fatal("Hijing_Jpsiee002","max impact parameter smaller than min impact parameter");
    if(bminConfig<0.)Fatal("Hijing_Jpsiee002","min impact parameter <0");
    if(bmaxConfig>20)Fatal("Hijing_Jpsiee002","unphysical impact parameter");
    bEvalFormula=(bmaxConfig+bminConfig)/2.;
    ntimes=TMath::Nint(formula->Eval(bEvalFormula));
    delete formula; formula=0x0;
  }


  if (uidConfig % 10 < 7) {
    AliGenerator *jpsi  = Generator_Jpsiee(ptParamPrompt, 1.0, 0.0, 0.0, 0.0); 
    ctl->AddGenerator(jpsi, "Jpsi2ee", 1., formula,ntimes);
    TFile *file = new TFile("typeHF_4.proc", "recreate");
    file->Close();
  }
  else {
    AliGenerator *bjpsi = Generator_Jpsiee("Pythia BBar", 0.0, 0.0, 0.0, 1.0, kTRUE /*useEvtGenForBdecays*/);
    ctl->AddGenerator(bjpsi, "B2Jpsi2ee", 1., formula,ntimes);
    TFile *file = new TFile("typeHF_5.proc", "recreate");
    file->Close();
  }
  return ctl;
}
