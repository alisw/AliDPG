/// \ingroup MC/CustomGenerators/DPG
/// \brief   Performance generator (ATO-245)
AliGenerator * GeneratorCustom() {
  AliGenCocktail *ctl    = GeneratorCocktail("Hijing+Generator for performance (tracking,PID) studies");
  AliGenerator   *hij    = GeneratorHijing();
  AliGenerator   *genJet = PerformanceGenerator();
  ctl->AddGenerator(hij,    "Hijing", 1.);
  ctl->AddGenerator(genJet,"Generator for performance (tracking,PID) studies",1);
  return ctl;
}


/// Make performance generator with flat 1/pt and flat pdg () input jets
/// Jet multiplicity smeared (Poisson) with mean value nJets
/// * nJets  defaults 1 (pp), 2(pPb), 10 PbPb
///   * can be overwritten by env variable PerformanceGenerator_nJets
/// \return Performance Generator
AliGenerator* PerformanceGenerator() {
  Printf("======= GeneratorCustom ======");
  gSystem->Load("libEVGEN");
  Int_t nJets =1;
  if (systemConfig.EqualTo("Pb-Pb")) {
    nJets=10;
  }
  if (systemConfig.EqualTo("p-Pb") || systemConfig.EqualTo("Pb-p") ) {
    nJets=2;
  }
  // default value for injected fraction of jets
  if (gSystem->Getenv("PerformanceGenerator_nJets")) testHighPtFraction= atof(gSystem->Getenv("PerformanceGenerator_nJets"));
  //
  AliGenPerformance *genPerformance= new AliGenPerformance("AliGenPerformance", AliGenPerformance::kStream| AliGenPerformance::kStreamEvent); //
  genPerformance->SetNJets(nJets);
  printf("AliGenPerformance:   Mean number of jets\t%f\n",nJets);
  TF1* f1pt = new TF1("f1pt","1-10*x",0.0003,0.1);             //
  TF1* fPDG = new TF1("f1pt","x",1,6);                         // flat pdg distribution
  genPerformance->SetFunctions(f1pt,0,0,0,fPDG);
  return genPerformance;
}