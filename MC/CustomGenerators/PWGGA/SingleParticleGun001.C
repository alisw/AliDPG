AliGenerator *GeneratorCustom(TString opt = "")
{
  AliGenCocktail *ctl  = GeneratorCocktail("SingleParticleGun001");

  const Double_t ptMin=0.5;
  const Double_t ptMax=100.;

  const Double_t yMin=-0.15;
  const Double_t yMax=0.15;
  
  const Double_t phiMin=240.;
  const Double_t phiMax=330.;

  const Int_t nPDG         = 3;
  TString optPDGList[nPDG] = {"gamma","pi0", "eta"};
  Int_t PDGList[nPDG]      = {22, 111, 221};
  TString stringPDG        = "";
  Int_t iPDG               = -1;
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

  AliGenerator   *particle = GeneratorInjector(1, iPDG, ptMin, ptMax, yMin, yMax, phiMin, phiMax);
  ctl->AddGenerator(particle, Form("Injector (%s)",stringPDG.Data()), 1.);

  return ctl;
}
