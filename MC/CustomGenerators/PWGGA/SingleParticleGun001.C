AliGenerator *GeneratorCustom(TString opt = "")
{
  AliGenCocktail *ctl  = GeneratorCocktail("SingleParticleGun001");

  Double_t ptMin=0.;
  Double_t ptMax=100.;

  const Double_t yMin=-0.15;
  const Double_t yMax=0.15;
  
  const Double_t phiMin=240.;
  const Double_t phiMax=330.;

  const Int_t nPDG         = 3;
  TString optPDGList[nPDG] = {"gamma","pi0", "eta"};
  Int_t PDGList[nPDG]      = {22, 111, 221};
  TString stringPDG        = "";
  Int_t iPDG               = -1;

  const Int_t nPt = 3;
  const TString optPtList[nPt]  = {"low","mid","high"};
  const Double_t PtMinList[nPt] = {   0.,   5.,   40.};//in GeV/c
  const Double_t PtMaxList[nPt] = {  15.,  50.,  100.};//in GeV/c

  for (Int_t iopt = 0; iopt < nPDG; iopt++){
    if (opt.Contains(optPDGList[iopt])){
      iPDG      = PDGList[iopt];
      stringPDG = optPDGList[iopt];
    }
  }

  if(iPDG==-1){
    printf("ERROR: no valid particle selected\n");
    return NULL;
  }

  for (Int_t iopt = 0; iopt < nPt; iopt++){
    if (opt.Contains(optPtList[iopt])){
      ptMin = PtMinList[iopt];
      ptMax = PtMaxList[iopt];
    }
  }

  printf("INFO: %s will be injected in %2.1f < pT < %2.1f (GeV/c)\n",stringPDG.Data(),ptMin,ptMax);

  AliGenerator   *particle = GeneratorInjector(1, iPDG, ptMin, ptMax, yMin, yMax, phiMin, phiMax);
  ctl->AddGenerator(particle, Form("Injector (%s)",stringPDG.Data()), 1.);

  return ctl;
}
