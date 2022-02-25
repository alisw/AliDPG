void doStep(Int_t prec=1, Int_t iMin=0, Int_t iMax=100, Float_t zMax=5., float stepsize=1.) {
  
  double rMin =iMin*stepsize;
  double rMax =iMax*stepsize;
  Int_t nThetaBins = prec*45;  // Y
  Double_t thetaMin = 85;
  Double_t thetaMax = 95;
  Int_t nPhiBins = prec*45;   //X
  Double_t phiMin = 0;
  Double_t phiMax = 360;
  
  AliLegoGenerator* gener = new AliLegoGenerator();
  AliSimulation simul;
//  simul->SetDefaultStorage("local:///alidata/msitta/ITS/MatBud/run/OCDB");
  simul.SetDefaultStorage("alien://folder=/alice/data/2015/OCDB");
  simul.SetRunNumber(225052);


  gBenchmark->Start("lego");
  simul.RunLego("Config.C",
		nThetaBins, thetaMin, thetaMax,
		nPhiBins, phiMin, phiMax,
		rMin, rMax, zMax, gener);
  
  gBenchmark->Stop("lego");
  gBenchmark->Show("lego");
};
