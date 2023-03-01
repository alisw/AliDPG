Double_t EtaToTheta(Double_t eta)
{
  return 2*TMath::ATan(TMath::Exp(-eta))*180/TMath::Pi();
}

// Generators for FOCAL simulations
enum GenTypes {
  gun=0, 
  gunJpsi,
  box,
  boxMomUniform,
  boxMonocromatic,
  boxWithDecay,
  boxWithPP,
  pythia, 
  pythia_MBtrig, 
  pythia_dirgamma_trig,
  pythia_gammajet_trig,
  pythia_particle_trig,
  ntuple, 
  hijing, 
  hijingAP, 
  PA_cocktail_dirgam, 
  PA_cocktail_MBtrig, 
  PA_cocktail_EPOS_MBtrig, 
  PA_cocktail_EPOS_dirgam,
  jpsiAndPythiaMB,
  pi0WithPileup,
  hydjet,
  kNGenTypes
};

TString gGenTypeNames[kNGenTypes] = {
  "gun", 
  "gunJpsi",
  "box", 
  "boxMomUniform",
  "boxMonocromatic",
  "boxWithDecay",
  "boxWithPP",
  "pythia", 
  "pythia_MBtrig", 
  "pythia_dirgamma_trig",
  "pythia_gammajet_trig",
  "pythia_particle_trig",
  "ntuple",
  "hijing",
  "hijingAP",
  "PA_cocktail_dirgam",
  "PA_cocktail_MBtrig",
  "PA_cocktail_EPOS_MBtrig",
  "PA_cocktail_EPOS_dirgam",
  "jpsiAndPythiaMB",
  "pi0WithPileup",
  "hydjet"
};


AliGenerator* GeneratorCustom(TString opt = "") {
        
  Int_t generatorType = gun;
  for (Int_t iopt = 0; iopt < kNGenTypes; iopt++) {
    if (opt.EqualTo(gGenTypeNames[iopt])) {
      generatorType = iopt;
    }
  }
    
  //switch between the different generators
  AliGenerator * generator = 0x0;
  switch(generatorType) {
          
    case gun:
    // Example for Fixed Particle Gun             
    {
      float pmom = 500.0; // GeV/c      
      if (gSystem->Getenv("CONFIG_PMOM")) {
        pmom = atof(gSystem->Getenv("CONFIG_PMOM"));
      }
      
      float eta = 4.5;
      if (gSystem->Getenv("CONFIG_ETA")) {
        eta = atof(gSystem->Getenv("CONFIG_ETA"));
      }
      
      int pdg = kGamma;
      if (gSystem->Getenv("CONFIG_PDG")) {
        pdg = atoi(gSystem->Getenv("CONFIG_PDG"));
      }
            
      AliGenFixed *gener = new AliGenFixed(1);
      gener->SetMomentum(pmom);
      gener->SetPhi(0.);
      gener->SetTheta(EtaToTheta(eta));
      gener->SetOrigin(0,0,0);        //vertex position
      gener->SetPart(pdg);
      generator = gener;
    }
    break;

    case gunJpsi:
    {
      float ptmin = 0.0;      
      if (gSystem->Getenv("CONFIG_PTMIN")) {
        ptmin = atof(gSystem->Getenv("CONFIG_PTMIN"));
      }
      float ptmax = 10.0;      
      if (gSystem->Getenv("CONFIG_PTMAX")) {
        ptmax = atof(gSystem->Getenv("CONFIG_PTMAX"));
      }
      float ymin = 3.0;      
      if (gSystem->Getenv("CONFIG_YMIN")) {
        ymin = atof(gSystem->Getenv("CONFIG_YMIN"));
      }
      float ymax = 6.0;      
      if (gSystem->Getenv("CONFIG_YMAX")) {
        ymax = atof(gSystem->Getenv("CONFIG_YMAX"));
      }
      float phimin = 0.0;      
      if (gSystem->Getenv("CONFIG_PHIMIN")) {
        phimin = atof(gSystem->Getenv("CONFIG_PHIMIN"));
      }
      float phimax = 360.0;      
      if (gSystem->Getenv("CONFIG_PHIMAX")) {
        phimax = atof(gSystem->Getenv("CONFIG_PHIMAX"));
      }
            
      // load libraries to use Evtgen
      gSystem->Load("libPhotos");
      //gSystem->Load("libEvtGenBase");
      //gSystem->Load("libEvtGenModels");
      gSystem->Load("libEvtGen");
      gSystem->Load("libEvtGenExternal");
      gSystem->Load("libTEvtGen");  
      //
      // set external decayer
      TVirtualMCDecayer* decayer = new AliDecayerPythia();
      decayer->SetForceDecay(kAll);
      decayer->Init();
      gMC->SetExternalDecayer(decayer);      
            
      AliGenCocktail *gener = new AliGenCocktail();
      gener->UsePerEventRates();
      // flat low pT
      AliGenParam *jpsi = new AliGenParam(1, AliGenMUONlib::kJpsi, "Flat", "Jpsi");  // flat pt distribution
      jpsi->SetPtRange(ptmin, ptmax);
      jpsi->SetYRange(ymin, ymax);
      jpsi->SetPhiRange(phimin, phimax);
      jpsi->SetForceDecay(kNoDecay);
      
      gener->AddGenerator(jpsi, "jpsi", 1.0);
      
      AliGenEvtGen *gene = new AliGenEvtGen();
      gene->SetForceDecay(kBJpsiDiElectron);
      gene->SetParticleSwitchedOff(AliGenEvtGen::kCharmPart);
      gener->AddGenerator(gene, "EvtGen", 1.);
      
      generator = gener;
    }
    break;
    
    case box:  
    // Example for Moving Particle Gun  
    {
      float ptmin = 0.0;      
      if (gSystem->Getenv("CONFIG_PTMIN")) {
        ptmin = atof(gSystem->Getenv("CONFIG_PTMIN"));
      }
      float ptmax = 10.0;      
      if (gSystem->Getenv("CONFIG_PTMAX")) {
        ptmax = atof(gSystem->Getenv("CONFIG_PTMAX"));
      }
      float etamin = 3.0;      
      if (gSystem->Getenv("CONFIG_ETAMIN")) {
        etamin = atof(gSystem->Getenv("CONFIG_ETAMIN"));
      }
      float etamax = 6.0;      
      if (gSystem->Getenv("CONFIG_ETAMAX")) {
        etamax = atof(gSystem->Getenv("CONFIG_ETAMAX"));
      }
      float phimin = 0.0;      
      if (gSystem->Getenv("CONFIG_PHIMIN")) {
        phimin = atof(gSystem->Getenv("CONFIG_PHIMIN"));
      }
      float phimax = 360.0;      
      if (gSystem->Getenv("CONFIG_PHIMAX")) {
        phimax = atof(gSystem->Getenv("CONFIG_PHIMAX"));
      }
      int pdg = kGamma;
      if (gSystem->Getenv("CONFIG_PDG")) {
        pdg = atoi(gSystem->Getenv("CONFIG_PDG"));
      }
            
      AliGenBox *gener = new AliGenBox(1);
      gener->SetPtRange(ptmin, ptmax);
      gener->SetPhiRange(phimin, phimax);  // full polar angle around beam axis
      gener->SetEtaRange(etamin, etamax);
      gener->SetOrigin(0,0,0);   
      //vertex position
      gener->SetSigma(0,0,0);         //Sigma in (X,Y,Z) (cm) on IP position
      gener->SetPart(pdg);
      generator = gener;
    }
    break;
    
    case boxMomUniform:  
    // Example for Moving Particle Gun  
    {
      float pmin = 0.0;      
      if (gSystem->Getenv("CONFIG_PTMIN")) {          // NOTE: we use the ptmin parameter to transmit the pmin (full momentum)
        pmin = atof(gSystem->Getenv("CONFIG_PTMIN"));
      }
      float pmax = 2000.0;      
      if (gSystem->Getenv("CONFIG_PTMAX")) {          // NOTE: we use the ptmax parameter to transmit the pmax (full momentum)
        pmax = atof(gSystem->Getenv("CONFIG_PTMAX"));
      }
      float etamin = 3.0;      
      if (gSystem->Getenv("CONFIG_ETAMIN")) {
        etamin = atof(gSystem->Getenv("CONFIG_ETAMIN"));
      }
      float etamax = 6.0;      
      if (gSystem->Getenv("CONFIG_ETAMAX")) {
        etamax = atof(gSystem->Getenv("CONFIG_ETAMAX"));
      }
      float phimin = 0.0;      
      if (gSystem->Getenv("CONFIG_PHIMIN")) {
        phimin = atof(gSystem->Getenv("CONFIG_PHIMIN"));
      }
      float phimax = 360.0;      
      if (gSystem->Getenv("CONFIG_PHIMAX")) {
        phimax = atof(gSystem->Getenv("CONFIG_PHIMAX"));
      }
      int pdg = kGamma;
      if (gSystem->Getenv("CONFIG_PDG")) {
        pdg = atoi(gSystem->Getenv("CONFIG_PDG"));
      }
            
      AliGenBox *gener = new AliGenBox(1);
      gener->SetMomentumRange(pmin, pmax);
      gener->SetPhiRange(phimin, phimax);  // full polar angle around beam axis
      gener->SetEtaRange(etamin, etamax);
      gener->SetOrigin(0,0,0);   
      //vertex position
      gener->SetSigma(0,0,0);         //Sigma in (X,Y,Z) (cm) on IP position
      gener->SetPart(pdg);
      generator = gener;
    }
    break;
    
    case boxMonocromatic:  
    // Example for Moving Particle Gun  
    {
      float pmom = 500.0;      
      if (gSystem->Getenv("CONFIG_PMOM")) {
        pmom = atof(gSystem->Getenv("CONFIG_PMOM"));
      }
      float etamin = 3.0;      
      if (gSystem->Getenv("CONFIG_ETAMIN")) {
        etamin = atof(gSystem->Getenv("CONFIG_ETAMIN"));
      }
      float etamax = 6.0;      
      if (gSystem->Getenv("CONFIG_ETAMAX")) {
        etamax = atof(gSystem->Getenv("CONFIG_ETAMAX"));
      }
      float phimin = 0.0;      
      if (gSystem->Getenv("CONFIG_PHIMIN")) {
        phimin = atof(gSystem->Getenv("CONFIG_PHIMIN"));
      }
      float phimax = 360.0;      
      if (gSystem->Getenv("CONFIG_PHIMAX")) {
        phimax = atof(gSystem->Getenv("CONFIG_PHIMAX"));
      }
      int pdg = kGamma;
      if (gSystem->Getenv("CONFIG_PDG")) {
        pdg = atoi(gSystem->Getenv("CONFIG_PDG"));
      }
            
      AliGenBox *gener = new AliGenBox(1);
      gener->SetMomentumRange(0.9999*pmom, 1.0001*pmom);
      gener->SetPhiRange(phimin, phimax);  // full polar angle around beam axis
      gener->SetEtaRange(etamin, etamax);
      gener->SetOrigin(0,0,0);   
      //vertex position
      gener->SetSigma(0,0,0);         //Sigma in (X,Y,Z) (cm) on IP position
      gener->SetPart(pdg);
      generator = gener;
    }
    break;
    
    case boxWithDecay:  
    // Example for Moving Particle Gun (with decays enabled) 
    {
      float ptmin = 0.0;      
      if (gSystem->Getenv("CONFIG_PTMIN")) {
        ptmin = atof(gSystem->Getenv("CONFIG_PTMIN"));
      }
      float ptmax = 10.0;      
      if (gSystem->Getenv("CONFIG_PTMAX")) {
        ptmax = atof(gSystem->Getenv("CONFIG_PTMAX"));
      }
      float ymin = 3.0;      
      if (gSystem->Getenv("CONFIG_YMIN")) {
        ymin = atof(gSystem->Getenv("CONFIG_YMIN"));
      }
      float ymax = 6.0;      
      if (gSystem->Getenv("CONFIG_YMAX")) {
        ymax = atof(gSystem->Getenv("CONFIG_YMAX"));
      }
      float phimin = 0.0;      
      if (gSystem->Getenv("CONFIG_PHIMIN")) {
        phimin = atof(gSystem->Getenv("CONFIG_PHIMIN"));
      }
      float phimax = 360.0;      
      if (gSystem->Getenv("CONFIG_PHIMAX")) {
        phimax = atof(gSystem->Getenv("CONFIG_PHIMAX"));
      }
      int pdg = 23;   // default: Z0
      if (gSystem->Getenv("CONFIG_PDG")) {
        pdg = atoi(gSystem->Getenv("CONFIG_PDG"));
      }
      
      // set external decayer
      TVirtualMCDecayer* decayer = new AliDecayerPythia();
      decayer->SetForceDecay(kAll);
      decayer->Init();
      gMC->SetExternalDecayer(decayer);      
      
      AliGenCocktail *gener = new AliGenCocktail();
      gener->UsePerEventRates();
      
      AliGenBox *genBox = new AliGenBox(1);
      genBox->SetPtRange(ptmin, ptmax);
      genBox->SetPhiRange(phimin, phimax);  // full polar angle around beam axis
      genBox->SetYRange(ymin, ymax);
      genBox->SetOrigin(0,0,0);   
      //vertex position
      genBox->SetSigma(0,0,0);         //Sigma in (X,Y,Z) (cm) on IP position
      genBox->SetPart(pdg);
      gener->AddGenerator(genBox, "box", 1.);      
      
      AliGenEvtGen *gene = new AliGenEvtGen();
      // NOTE: the original decay table is in AliRoot/TEvtGen/EvtGen/DecayTable/DIELECTRON.DEC
      //       If the decay you are after is there, then you can comment out the following line.
      //       Alternatively, make sure the decay file you use below is in the working directory (or copied to 
      //         the grid working note if you run on the grid)
      char* decayTable = "DIELECTRON.DEC";
      gene->SetUserDecayTable(decayTable);
      if (pdg==23) {
        gene->SetForceDecay(kZDiElectron);
        gene->SetParticleSwitchedOff(AliGenEvtGen::kAllPart);
      }
      if (pdg==443) {
        gene->SetForceDecay(kDiElectron);
        gene->SetParticleSwitchedOff(AliGenEvtGen::kCharmPart);      
      }
      if (pdg==553 || pdg==100553 || pdg==200553) {
        gene->SetForceDecay(kDiElectron);
        gene->SetParticleSwitchedOff(AliGenEvtGen::kCharmPart);      
      }
      gene->Init();
      gener->AddGenerator(gene, "EvtGen", 1.);
      
      generator = gener;
    }
    break;

    case boxWithPP:
    {
      float ptmin = 0.0;      
      if (gSystem->Getenv("CONFIG_PTMIN")) {
        ptmin = atof(gSystem->Getenv("CONFIG_PTMIN"));
      }
      float ptmax = 10.0;      
      if (gSystem->Getenv("CONFIG_PTMAX")) {
        ptmax = atof(gSystem->Getenv("CONFIG_PTMAX"));
      }
      float etamin = 3.0;      
      if (gSystem->Getenv("CONFIG_ETAMIN")) {
        etamin = atof(gSystem->Getenv("CONFIG_ETAMIN"));
      }
      float etamax = 6.0;      
      if (gSystem->Getenv("CONFIG_ETAMAX")) {
        etamax = atof(gSystem->Getenv("CONFIG_ETAMAX"));
      }
      float phimin = 0.0;      
      if (gSystem->Getenv("CONFIG_PHIMIN")) {
        phimin = atof(gSystem->Getenv("CONFIG_PHIMIN"));
      }
      float phimax = 360.0;      
      if (gSystem->Getenv("CONFIG_PHIMAX")) {
        phimax = atof(gSystem->Getenv("CONFIG_PHIMAX"));
      }
      int nbkg = 1;      
      if (gSystem->Getenv("CONFIG_NBKG")) {
        nbkg = atoi(gSystem->Getenv("CONFIG_NBKG"));
      }
      int pdg = kGamma;
      if (gSystem->Getenv("CONFIG_PDG")) {
        pdg = atoi(gSystem->Getenv("CONFIG_PDG"));
      }
      float energy = 14000;  // GeV, used for the pythia MB event
      if (gSystem->Getenv("CONFIG_ENERGY")) {
        energy = atof(gSystem->Getenv("CONFIG_ENERGY"));
      }
      
      // set external decayer
      TVirtualMCDecayer* decayer = new AliDecayerPythia();
      decayer->SetForceDecay(kAll);
      decayer->Init();
      gMC->SetExternalDecayer(decayer);
      
      // Create the cocktail generator
      AliGenCocktail *cocktail = new AliGenCocktail();
      cocktail->UsePerEventRates();
      
      AliGenBox *boxGen = new AliGenBox(1);
      boxGen->SetPtRange(ptmin, ptmax);
      boxGen->SetPhiRange(phimin, phimax);  // full polar angle around beam axis
      boxGen->SetEtaRange(etamin, etamax);
      boxGen->SetOrigin(0,0,0);   
      //vertex position
      boxGen->SetSigma(0,0,0);         //Sigma in (X,Y,Z) (cm) on IP position
      boxGen->SetPart(pdg);
      cocktail->AddGenerator(boxGen, "Box Generator", 1.0);
      
      AliGenPythia *pythiaMB = new AliGenPythia(-1); 
      pythiaMB->SetMomentumRange(0,999999); 
      pythiaMB->SetThetaRange(0., 45.); 
      pythiaMB->SetYRange(-12,12); 
      pythiaMB->SetPtRange(0,1000); 
      pythiaMB->SetProcess(kPyMb); // Min. bias events 
      pythiaMB->SetEnergyCMS(energy); // LHC energy 
      //pythiaMB->SetOrigin(0, 0, 0); // Vertex position 
      pythiaMB->SetSigma(0, 0, 5.3); // Sigma in (X,Y,Z) (cm) on IP position 
      pythiaMB->SetCutVertexZ(1.); // Truncate at 1 sigma 
      pythiaMB->SetVertexSmear(kPerEvent); // Smear per event 
      pythiaMB->SetTrackingFlag(1); // Particle transport       
      
      if (nbkg>0) {
        cocktail->AddGenerator(pythiaMB, "PythiaMB", 1.0, 0, nbkg);
      }
      
      AliGenEvtGen *gene = new AliGenEvtGen();
      char* decayTable = "CUSTOMDECAYS.DEC";
      gene->SetUserDecayTable(decayTable);
      if (pdg==23) {
        gene->SetForceDecay(kZDiElectron);
        gene->SetParticleSwitchedOff(AliGenEvtGen::kAllPart);
      }
      if (pdg==443) {
        gene->SetForceDecay(kDiElectron);
        gene->SetParticleSwitchedOff(AliGenEvtGen::kCharmPart);      
      }
      if (pdg==553 || pdg==100553 || pdg==200553) {
        gene->SetForceDecay(kDiElectron);
        gene->SetParticleSwitchedOff(AliGenEvtGen::kCharmPart);      
      }
      if (pdg==221 || pdg==223) {
        gene->SetParticleSwitchedOff(AliGenEvtGen::kAllPart);        
      }
      gene->Init();
      cocktail->AddGenerator(gene, "EvtGen", 1.);
      
      generator = cocktail;      
    };
    break;
    
    case pythia:
    // Example for Pythia            
    {
      float energy = 14000;  // GeV
      if (gSystem->Getenv("CONFIG_ENERGY")) {
        energy = atof(gSystem->Getenv("CONFIG_ENERGY"));
      }
            
      AliGenPythia *gener = new AliGenPythia(-1); 
      gener->SetMomentumRange(0,999999); 
      gener->SetThetaRange(0., 45.); 
      gener->SetYRange(-12,12); 
      gener->SetPtRange(0,1000); 
      gener->SetProcess(kPyMb); // Min. bias events 
      gener->SetEnergyCMS(energy); // LHC energy 
      //gener->SetOrigin(0, 0, 0); // Vertex position 
      gener->SetSigma(0, 0, 5.3); // Sigma in (X,Y,Z) (cm) on IP position 
      gener->SetCutVertexZ(1.); // Truncate at 1 sigma 
      gener->SetVertexSmear(kPerEvent); // Smear per event 
      gener->SetTrackingFlag(1); // Particle transport 
      generator = gener;
    }
    break;

    case pythia_MBtrig:
    case pythia_dirgamma_trig:
    // Example for Pythia            
    {
      float energy = 14000;  // GeV
      if (gSystem->Getenv("CONFIG_ENERGY")) {
        energy = atof(gSystem->Getenv("CONFIG_ENERGY"));
      }      
            
      AliGenPythiaFOCAL *gener = new AliGenPythiaFOCAL(-1); 
      gener->SetMomentumRange(0,999999); 
      //gener->SetThetaRange(0., 45.); 
      gener->SetYRange(-12,12); 
      gener->SetPtRange(0,1000); 
      gener->SetEnergyCMS(energy); // LHC energy 
      //gener->SetOrigin(0, 0, 0); // Vertex position 
      gener->SetSigma(0, 0, 5.3); // Sigma in (X,Y,Z) (cm) on IP position 
      gener->SetCutVertexZ(1.); // Truncate at 1 sigma 
      gener->SetVertexSmear(kPerEvent); // Smear per event 
      gener->SetTrackingFlag(1); // Particle transport 
      //gener->SetEtaInFOCAL(kTRUE);
      //gener->SetPi0InFOCAL(kTRUE);
      if (generatorType == pythia_MBtrig) {
        gener->SetProcess(kPyMb); // Min. bias events 
        gener->SetDecayPhotonInFOCAL(kTRUE);
      }
      else {
        gener->SetProcess(kPyDirectGamma);
        gener->SetFragPhotonInFOCAL(kTRUE);
      }
      gener->SetCheckFOCAL(kTRUE);  
      gener->SetFOCALEta(3.0, 6.2);
      float ptmin = 4.0;      
      if (gSystem->Getenv("CONFIG_PTMIN")) {
        ptmin = atof(gSystem->Getenv("CONFIG_PTMIN"));
      }
      gener->SetTriggerParticleMinPt(ptmin);
      generator = gener;
    }
    break;

    case pythia_gammajet_trig:
    // Example for Pythia            
    {
      float energy = 14000;  // GeV
      if (gSystem->Getenv("CONFIG_ENERGY")) {
        energy = atof(gSystem->Getenv("CONFIG_ENERGY"));
      }      
            
      AliGenPythiaFOCAL *gener = new AliGenPythiaFOCAL(-1); 
      gener->SetMomentumRange(0,999999); 
      //gener->SetThetaRange(0., 45.); 
      gener->SetYRange(-12,12); 
      gener->SetPtRange(0,1000); 
      gener->SetEnergyCMS(energy); // LHC energy 
      //gener->SetOrigin(0, 0, 0); // Vertex position 
      gener->SetSigma(0, 0, 5.3); // Sigma in (X,Y,Z) (cm) on IP position 
      gener->SetCutVertexZ(1.); // Truncate at 1 sigma 
      gener->SetVertexSmear(kPerEvent); // Smear per event 
      gener->SetTrackingFlag(1); // Particle transport 
      //gener->SetEtaInFOCAL(kTRUE);
      //gener->SetPi0InFOCAL(kTRUE);
      gener->SetProcess(kPyDirectGamma);
      gener->SetDirPhotonInFOCAL(kTRUE);
      
      gener->SetCheckFOCAL(kTRUE);  
      gener->SetFOCALEta(3.0, 6.2);
      float ptmin = 4.0;      
      if (gSystem->Getenv("CONFIG_PTMIN")) {
        ptmin = atof(gSystem->Getenv("CONFIG_PTMIN"));
      }
      gener->SetTriggerParticleMinPt(ptmin);
      generator = gener;
    }
    break;
    
    case pythia_particle_trig:
    // Example for Pythia            
    {
      float energy = 14000;  // GeV
      if (gSystem->Getenv("CONFIG_ENERGY")) {
        energy = atof(gSystem->Getenv("CONFIG_ENERGY"));
      }
      float ptmin = 0.0;      
      if (gSystem->Getenv("CONFIG_PTMIN")) {
        ptmin = atof(gSystem->Getenv("CONFIG_PTMIN"));
      }
      int pdg = kGamma;
      if (gSystem->Getenv("CONFIG_PDG")) {
        pdg = atoi(gSystem->Getenv("CONFIG_PDG"));
      }
            
      // set external decayer
      TVirtualMCDecayer* decayer = new AliDecayerPythia();
      decayer->SetForceDecay(kAll);
      decayer->Init();
      gMC->SetExternalDecayer(decayer);
      
      // Create the cocktail generator
      AliGenCocktail *cocktail = new AliGenCocktail();
      cocktail->UsePerEventRates();
            
      AliGenPythiaFOCAL *pythiaFOCAL = new AliGenPythiaFOCAL(-1); 
      pythiaFOCAL->SetMomentumRange(0,999999); 
      //pythiaFOCAL->SetThetaRange(0., 45.); 
      pythiaFOCAL->SetYRange(-12,12); 
      pythiaFOCAL->SetPtRange(0,1000); 
      pythiaFOCAL->SetEnergyCMS(energy); // LHC energy 
      //pythiaFOCAL->SetOrigin(0, 0, 0); // Vertex position 
      pythiaFOCAL->SetSigma(0, 0, 5.3); // Sigma in (X,Y,Z) (cm) on IP position 
      pythiaFOCAL->SetCutVertexZ(1.); // Truncate at 1 sigma 
      pythiaFOCAL->SetVertexSmear(kPerEvent); // Smear per event 
      pythiaFOCAL->SetTrackingFlag(1); // Particle transport 
      pythiaFOCAL->SetProcess(kPyMb);
      pythiaFOCAL->SetCheckParticleInFOCAL(pdg);
      pythiaFOCAL->SetUseRapidity();
      pythiaFOCAL->SetCheckFOCAL(kTRUE);  
      pythiaFOCAL->SetFOCALEta(3.0, 6.2);
      pythiaFOCAL->SetTriggerParticleMinPt(ptmin);
      cocktail->AddGenerator(pythiaFOCAL, "PythiaFOCAL", 1.);
      
      AliGenEvtGen *evtGen = new AliGenEvtGen();
      if (pdg==23 || pdg==443 || pdg==553 || pdg==100553 || pdg==200553 || pdg==221 || pdg==223) {   // this is needed to force special decay channels
        char* decayTable = "CUSTOMDECAYS.DEC";
        evtGen->SetUserDecayTable(decayTable);              
      }
      if (pdg==23) {
        evtGen->SetForceDecay(kZDiElectron);
        evtGen->SetParticleSwitchedOff(AliGenEvtGen::kAllPart);
      }
      if (pdg==443) {
        evtGen->SetForceDecay(kDiElectron);
        evtGen->SetParticleSwitchedOff(AliGenEvtGen::kCharmPart);      
      }
      if (pdg==553 || pdg==100553 || pdg==200553) {
        evtGen->SetForceDecay(kDiElectron);
        evtGen->SetParticleSwitchedOff(AliGenEvtGen::kCharmPart);      
      }
      if (pdg==221 || pdg==223) {
        evtGen->SetParticleSwitchedOff(AliGenEvtGen::kAllPart);        
      }
      evtGen->Init();
      cocktail->AddGenerator(evtGen, "EvtGen", 1.);
      
      generator = cocktail;
    }
    break;

    case ntuple:
    // Example for reading from a external file             *
    {
      AliGenExtFile *gener = new AliGenExtFile(-1); 
      //gener->SetVertexSmear(kPerEvent); 
      gener->SetTrackingFlag(1);

      AliGenReaderTreeK * reader = new AliGenReaderTreeK();
      reader->SetFileName("sim1/galice.root");
      gener->SetReader(reader);
      generator = gener;
    }
    break;

    case hijing:
    // Example for reading from a external file             *
    {
      AliGenHijing *gener = new AliGenHijing(-1);
      // Take collision energy from command line params if specified
      float energy = 5500.0;
      if (gSystem->Getenv("CONFIG_ENERGY")) {
        energy = atof(gSystem->Getenv("CONFIG_ENERGY"));
      }
      float bmin = 0.0;
      if (gSystem->Getenv("CONFIG_BMIN")) {
        bmin = atof(gSystem->Getenv("CONFIG_BMIN"));
      }
      float bmax = 15.0;
      if (gSystem->Getenv("CONFIG_BMAX")) {
        bmax = atof(gSystem->Getenv("CONFIG_BMAX"));
      }
      gener->SetEnergyCMS(energy); // center of mass energy 
      gener->SetReferenceFrame("CMS"); // reference frame 
      gener->SetProjectile("A", 208, 82); // projectile 
      gener->SetTarget ("A", 208, 82); // projectile 
      gener->KeepFullEvent(); // HIJING will keep the full parent child chain 
      gener->SetJetQuenching(1); // enable jet quenching 
      gener->SetShadowing(1); // enable shadowing 
      gener->SetDecaysOff(1); // neutral pion and heavy particle decays switched off 
      gener->SetSpectators(0); // Don't track spectators 
      gener->SetSelectAll(0); // kinematic selection 
      gener->SetImpactParameterRange(bmin, bmax); // Impact parameter range (fm) 
      generator = gener;
    }
    break;
      
    case hijingAP:
    {
      AliGenHijingFOCAL *gener = new AliGenHijingFOCAL(); 
      // Take collision energy from command line params if specified
      float energy = 8790.0;
      if (gSystem->Getenv("CONFIG_ENERGY")) {
        energy = atof(gSystem->Getenv("CONFIG_ENERGY"));
      }
      
      gener->SetEnergyCMS(energy); // center of mass energy 
      gener->SetReferenceFrame("CMS"); // reference frame 
      gener->SetBoostLHC(0.0);   // No boost for now; need to check sign
      gener->SetProjectile("A", 208, 82); // projectile proton
      gener->SetTarget ("P", 1, 1); // projectile Pb nucleus
      gener->KeepFullEvent(); // HIJING will keep the full parent child chain 
      gener->SetJetQuenching(1); // enable jet quenching 
      gener->SetShadowing(1); // enable shadowing 
      gener->SetDecaysOff(1); // neutral pion and heavy particle decays switched off 
      gener->SetSpectators(0); // Don't track spectators 
      gener->SetSelectAll(0); // kinematic selection 
      gener->SetImpactParameterRange(0., 8.); // Impact parameter range (fm) 
      gener->UnsetDataDrivenSpectators();

      gener->SetTrigger(2); // 2: direct photons; 3: decay photons
      gener->SetPtJet(4.);
      gener->SetJetEtaRange(3.5,5.8);
      generator = gener;
    }
    break;
      
    case PA_cocktail_dirgam:
    case PA_cocktail_MBtrig:
    {
      AliGenCocktail *cocktail = new AliGenCocktail();
      cocktail->SetOrigin(0, 0, 0); // Vertex position
      cocktail->SetSigma(0, 0, 5.8); // Sigma in (X,Y,Z) (cm) on IP position
      cocktail->SetVertexSmear(kPerEvent); // Smear per event

      AliGenHijing *hijing = new AliGenHijing(-1); 
      // Take collision energy from command line params if specified
      float energy = 8790.0;
      if (gSystem->Getenv("CONFIG_ENERGY")) {
        energy = atof(gSystem->Getenv("CONFIG_ENERGY"));
      }
      
      hijing->SetEnergyCMS(energy); // center of mass energy 
      hijing->SetReferenceFrame("CMS"); // reference frame 
      hijing->SetBoostLHC(0.0);   // No boost for now; need to check sign
      hijing->SetProjectile("A", 208, 82); // projectile proton
      hijing->SetTarget ("P", 1, 1); // projectile Pb nucleus
      hijing->KeepFullEvent(); // HIJING will keep the full parent child chain 
      hijing->SetJetQuenching(1); // enable jet quenching 
      hijing->SetShadowing(1); // enable shadowing 
      hijing->SetDecaysOff(1); // neutral pion and heavy particle decays switched off 
      hijing->SetSpectators(0); // Don't track spectators 
      hijing->SetSelectAll(0); // kinematic selection 
      hijing->SetImpactParameterRange(0., 15.); // Impact parameter range (fm) 
      //hijing->UnsetDataDrivenSpectators();
      cocktail->AddGenerator(hijing,"Hijing",1.);

      AliGenPythiaFOCAL *pythia = new AliGenPythiaFOCAL(-1);
      pythia->SetMomentumRange(0,999999);
      pythia->SetThetaRange(0., 45.);
      pythia->SetYRange(-12,12);
      pythia->SetPtRange(0,1000);
      pythia->SetEnergyCMS(energy); // LHC energy
      pythia->SetTrackingFlag(1); // Particle transport
      if (generatorType == PA_cocktail_dirgam) {
        pythia->SetProcess(kPyDirectGamma); // Direct photon events
        pythia->SetFragPhotonInFOCAL(kTRUE);
      }
      else {
        pythia->SetProcess(kPyMb); // MB events
        pythia->SetDecayPhotonInFOCAL(kTRUE);
      }
      pythia->SetCheckFOCAL(kTRUE);
      pythia->SetFOCALEta(3.0, 6.2);
      pythia->SetTriggerParticleMinPt(5.0);
      cocktail->AddGenerator(pythia,"Pythia",1.);

      generator = cocktail;	
    }
    break;

    case PA_cocktail_EPOS_dirgam:
    case PA_cocktail_EPOS_MBtrig:
    {
      // Take collision energy from command line params if specified
      float energy = 8790.0;
      if (gSystem->Getenv("CONFIG_ENERGY")) {
        energy = atof(gSystem->Getenv("CONFIG_ENERGY"));
      }
      int nEvents = 1;
      if (gSystem->Getenv("CONFIG_NEVENTS")) {
        nEvents = atoi(gSystem->Getenv("CONFIG_NEVENTS"));
      }
      int projectileId = 2212;            // proton
      int targetId = 82*10000 + 208*10;   // Pb nucleus in the EPOS language
      float projectileEnergy = 0.5 * energy * TMath::Sqrt(208./82.);
      float targetEnergy = 0.5 * energy * TMath::Sqrt(82./208.);
      bool pileup = false;
      
      AliGenCocktailFOCAL *cocktail = new AliGenCocktailFOCAL();
      cocktail->SetOrigin(0, 0, 0); // Vertex position
      cocktail->SetSigma(0, 0, 5.8); // Sigma in (X,Y,Z) (cm) on IP position
      cocktail->SetVertexSmear(kPerEvent); // Smear per event

      /*TString fifoname("myfifo");
      AliGenExtExec* eposExt = new AliGenExtExec();
      //eposExt->SetPathScript(Form("./gen_eposlhc.sh %s",fifoname.Data()));
      eposExt->SetPathScript(Form("crmc -o hepmc -f %s -n %d -m 0 -i %d -p %f -I %d -P %f",
                                  fifoname.Data(),nEvents,projectileId,projectileEnergy,targetId,targetEnergy));
      eposExt->SetPathFIFO(fifoname);
      cocktail->AddGenerator(eposExt,"EPOS",1.);*/
      
      TString fifoname("myfifo");
      gROOT->ProcessLine(Form(".! rm -rf %s", fifoname.Data()));
      gROOT->ProcessLine(Form(".! mkfifo %s", fifoname.Data()));
      /*gROOT->ProcessLine(Form(".! bash $ALIDPG_ROOT/MC/EXTRA/gen_eposlhc.sh %s %d %d %f %d %f &> gen_eposlhc%d.log &",
                              fifoname.Data(), nEvents,
                              projectileId, projectileEnergy,
                              targetId, targetEnergy, pileup));*/
      gROOT->ProcessLine(Form(".! crmc -o hepmc -f %s -n %d -m 0 -i %d -p %f -I %d -P %f &> gen_eposlhc.log &",
                              fifoname.Data(), nEvents,
                              projectileId, projectileEnergy,
                              targetId, targetEnergy));
      
      // connect HepMC reader
      AliGenReaderHepMC *reader = new AliGenReaderHepMC();
      reader->SetFileName(fifoname.Data());
      AliGenExtFile *eposExt = new AliGenExtFile(-1);
      eposExt->SetName("EPOSLHC");
      eposExt->SetReader(reader);
      cocktail->AddGenerator(eposExt,"EPOS",1.);

      AliGenPythiaFOCAL *pythia = new AliGenPythiaFOCAL(-1);
      pythia->SetMomentumRange(0,999999);
      pythia->SetThetaRange(0., 45.);
      pythia->SetYRange(-12,12);
      pythia->SetPtRange(0,1000);
      pythia->SetEnergyCMS(energy); // LHC energy
      pythia->SetTrackingFlag(1); // Particle transport
      if (generatorType == PA_cocktail_EPOS_dirgam) {
        pythia->SetProcess(kPyDirectGamma); // Direct photon events
        pythia->SetFragPhotonInFOCAL(kTRUE);
        pythia->SetCheckFOCAL(kTRUE);
        pythia->SetFOCALEta(3.5, 5.9);
        pythia->SetTriggerParticleMinPt(4.0);
      } else {
        pythia->SetProcess(kPyMb); // MB events
        //pythia->SetDecayPhotonInFOCAL(kTRUE);
      } 
      cocktail->AddGenerator(pythia,"Pythia",1.);
      cocktail->SetTriggerKinematics(4.0,3.5,5.9);
      generator = cocktail;
    }
    break;
    
    case jpsiAndPythiaMB:
    {
      float energy = 14000;  // GeV, used for the pythia MB event
      if (gSystem->Getenv("CONFIG_ENERGY")) {
        energy = atof(gSystem->Getenv("CONFIG_ENERGY"));
      }
      
      float ptmin = 0.0;      
      if (gSystem->Getenv("CONFIG_PTMIN")) {
        ptmin = atof(gSystem->Getenv("CONFIG_PTMIN"));
      }
      float ptmax = 10.0;      
      if (gSystem->Getenv("CONFIG_PTMAX")) {
        ptmax = atof(gSystem->Getenv("CONFIG_PTMAX"));
      }
      float ymin = 3.0;      
      if (gSystem->Getenv("CONFIG_YMIN")) {
        ymin = atof(gSystem->Getenv("CONFIG_YMIN"));
      }
      float ymax = 6.0;      
      if (gSystem->Getenv("CONFIG_YMAX")) {
        ymax = atof(gSystem->Getenv("CONFIG_YMAX"));
      }
      float phimin = 0.0;      
      if (gSystem->Getenv("CONFIG_PHIMIN")) {
        phimin = atof(gSystem->Getenv("CONFIG_PHIMIN"));
      }
      float phimax = 360.0;      
      if (gSystem->Getenv("CONFIG_PHIMAX")) {
        phimax = atof(gSystem->Getenv("CONFIG_PHIMAX"));
      }
      // load libraries to use Evtgen
      gSystem->Load("libPhotos");
      //gSystem->Load("libEvtGenBase");
      //gSystem->Load("libEvtGenModels");
      gSystem->Load("libEvtGen");
      gSystem->Load("libEvtGenExternal");
      gSystem->Load("libTEvtGen");  
      //
      // set external decayer
      TVirtualMCDecayer* decayer = new AliDecayerPythia();
      decayer->SetForceDecay(kAll);
      decayer->Init();
      gMC->SetExternalDecayer(decayer);
      
      // Create the cocktail generator
      AliGenCocktail *gener = new AliGenCocktail();
      gener->UsePerEventRates();

      // Add one MB event
      AliGenPythia *myPythia = new AliGenPythia(-1); 
      myPythia->SetMomentumRange(0,999999); 
      myPythia->SetThetaRange(0., 45.);          // NOTE: generate particles in a hemi-sphere which includes FOCAL. Change this if you need kinematics in the full acceptance
      myPythia->SetYRange(-12,12); 
      myPythia->SetPtRange(0,1000); 
      myPythia->SetProcess(kPyMb); // Min. bias events 
      myPythia->SetEnergyCMS(energy); // LHC energy 
      //myPythia->SetSigma(0.0, 0.0, 0.0); // Sigma in (X,Y,Z) (cm) on IP position 
      //myPythia->SetCutVertexZ(1.); // Truncate at 1 sigma 
      //myPythia->SetVertexSmear(kPerEvent); // Smear per event 
      myPythia->SetTrackingFlag(1); // Particle transport 
      //gener->AddGenerator(myPythia, "pythiaMB", 1.0);
      
      // Add the jpsi generator (parameterization based on previous forward measurements)
      AliGenParam *jpsi = new AliGenParam(1, AliGenMUONlib::kJpsi, "pp 8", "Jpsi");  // flat pt distribution
      jpsi->SetPtRange(ptmin, ptmax);
      jpsi->SetYRange(ymin, ymax);
      jpsi->SetPhiRange(phimin, phimax);
      jpsi->SetForceDecay(kNoDecay);
      gener->AddGenerator(jpsi, "jpsi", 1.0);
      
      //add the EVTGEN generator (purpose is just to decay the jpsi)
      AliGenEvtGen *gene = new AliGenEvtGen();
      gene->SetForceDecay(kBJpsiDiElectron);
      gene->SetParticleSwitchedOff(AliGenEvtGen::kCharmPart);
      gener->AddGenerator(gene, "EvtGen", 1.);
      generator = gener;
    };
    break;
    
    case pi0WithPileup:
    {
      float energy = 14000;  // GeV, used for the pythia MB event
      if (gSystem->Getenv("CONFIG_ENERGY")) {
        energy = atof(gSystem->Getenv("CONFIG_ENERGY"));
      }
      
      float ptmin = 4.0;      
      if (gSystem->Getenv("CONFIG_PTMIN")) {
        ptmin = atof(gSystem->Getenv("CONFIG_PTMIN"));
      }
      int nbkg = 0;      
      if (gSystem->Getenv("CONFIG_NBKG")) {
        nbkg = atoi(gSystem->Getenv("CONFIG_NBKG"));
      }
            
      // Create the cocktail generator
      AliGenCocktail *cocktail = new AliGenCocktail();
      cocktail->UsePerEventRates();
            
      AliGenPythiaFOCAL *gener = new AliGenPythiaFOCAL(-1); 
      gener->SetMomentumRange(0,999999); 
      gener->SetThetaRange(0., 45.); 
      gener->SetYRange(-12,12); 
      gener->SetPtRange(0,1000); 
      gener->SetEnergyCMS(energy); // LHC energy 
      //gener->SetOrigin(0, 0, 0); // Vertex position 
      gener->SetSigma(0, 0, 5.3); // Sigma in (X,Y,Z) (cm) on IP position 
      gener->SetCutVertexZ(1.); // Truncate at 1 sigma 
      gener->SetVertexSmear(kPerEvent); // Smear per event 
      gener->SetTrackingFlag(1); // Particle transport 
      gener->SetProcess(kPyMb); // Min. bias events 
      gener->SetDecayPhotonInFOCAL(kTRUE);        // trigger mostly on pi0 
      gener->SetCheckFOCAL(kTRUE);  
      gener->SetFOCALEta(3.0, 6.2);
      gener->SetTriggerParticleMinPt(ptmin);
      cocktail->AddGenerator(gener, "PythiaTriggered", 1.0);
      
      AliGenPythia *pythiaMB = new AliGenPythia(-1); 
      pythiaMB->SetMomentumRange(0,999999); 
      pythiaMB->SetThetaRange(0., 45.); 
      pythiaMB->SetYRange(-12,12); 
      pythiaMB->SetPtRange(0,1000); 
      pythiaMB->SetProcess(kPyMb); // Min. bias events 
      pythiaMB->SetEnergyCMS(energy); // LHC energy 
      //pythiaMB->SetOrigin(0, 0, 0); // Vertex position 
      pythiaMB->SetSigma(0, 0, 5.3); // Sigma in (X,Y,Z) (cm) on IP position 
      pythiaMB->SetCutVertexZ(1.); // Truncate at 1 sigma 
      pythiaMB->SetVertexSmear(kPerEvent); // Smear per event 
      pythiaMB->SetTrackingFlag(1); // Particle transport 
      if (nbkg>0) {
        cocktail->AddGenerator(pythiaMB, "PythiaMB", 1.0, 0, nbkg);
      }
      
      generator = cocktail;
    };
    break;

    case hydjet:
    {
    /*////////////////////////////////////////////////////////////////////////////////////////

    The estimation of momentum and spatial anisotropy parameters for different centralities

% centrality   fEpsilon   fDelta
     0-5%        0.05      0.10
    5-10%        0.10      0.24
   10-20%        0.11      0.28 
   20-30%        0.13      0.33 
   30-40%        0.14      0.35 
   40-60%        0.15      0.37

The range of impact parameters in the units of nuclear radius for different 
centralities at RHIC (Au+Au, 200 A GeV) and LHC (Pb+Pb, 5500 A GeV) is 
specified below (the standard Woods-Saxon nucleon distribution is assumed).

% centrality      b/RA (RHIC)        b/RA (LHC) 
0                 0                  0          
5                 0.5                0.51
6                 0.55               0.57
10                0.72               0.74 
12                0.79               0.81
15		            0.89               0.91
20                1.02               1.05
25		            1.15               1.18 
30                1.26               1.29
35                1.36               1.39
40                1.46               1.49
45		            1.55               1.58
50		            1.63               1.67
55		            1.71               1.75  
60                1.79               1.83
65		            1.86               1.90
70		            1.93               1.97 
75		            2.01               2.06
*/

      float bmin = 0.0;
      if (gSystem->Getenv("CONFIG_BMIN")) {
        bmin = atof(gSystem->Getenv("CONFIG_BMIN"));
      }
      float bmax = 3.0;
      if (gSystem->Getenv("CONFIG_BMAX")) {
        bmax = atof(gSystem->Getenv("CONFIG_BMAX"));
      }

      AliGenUHKM *uhkm = new AliGenUHKM(); 
      uhkm->SetAllParametersLHC();
      uhkm->SetPDGParticleFile("particles.data");
      uhkm->SetPDGDecayFile("tabledecay.txt");
      uhkm->SetBmin(bmin);
      uhkm->SetBmax(bmax);
      // values for 30-40% centrality range: fDelta = 0.35, fEpsilon = 0.14
      uhkm->SetMomAsymmPar(0.35);             // fDelta
      uhkm->SetCoordAsymmPar(0.14);           // fEpsilon
      
      generator = uhkm;
    }
    break;
    
  }  // end switch
  
  return generator;
}
