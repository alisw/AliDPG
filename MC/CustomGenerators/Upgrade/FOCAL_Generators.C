Double_t EtaToTheta(Double_t eta)
{
  return 2*TMath::ATan(TMath::Exp(-eta))*180/TMath::Pi();
}

// Generators for FOCAL simulations
enum GenTypes {
  gun=0, 
  gunJpsi,
  box, 
  pythia, 
  pythia_MBtrig, 
  pythia_dirgamma_trig, 
  ntuple, 
  hijing, 
  hijingAP, 
  PA_cocktail_dirgam, 
  PA_cocktail_MBtrig, 
  PA_cocktail_EPOS_MBtrig, 
  PA_cocktail_EPOS_dirgam,
  kNGenTypes
};

TString gGenTypeNames[kNGenTypes] = {
  "gun", 
  "gunJpsi",
  "box", 
  "pythia", 
  "pythia_MBtrig", 
  "pythia_dirgamma_trig",
  "ntuple",
  "hijing",
  "hijingAP",
  "PA_cocktail_dirgam",
  "PA_cocktail_MBtrig",
  "PA_cocktail_EPOS_MBtrig",
  "PA_cocktail_EPOS_dirgam"
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
      gener->SetThetaRange(0., 45.); 
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
      gener->SetFOCALEta(3.5, 6.2);
      gener->SetTriggerParticleMinPt(4.0);
      generator = gener;
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
      gener->SetImpactParameterRange(0., 15.); // Impact parameter range (fm) 
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
      //gener->UnsetDataDrivenSpectators();

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
      pythia->SetFOCALEta(3.5, 5.9);
      pythia->SetTriggerParticleMinPt(4.0);
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

      /*TString fifoname("$TMPDIR/myfifo");
      AliGenExtExec* eposExt = new AliGenExtExec();
      eposExt->SetPathScript(Form("./gen_eposlhc.sh %s",fifoname.Data()));
      eposExt->SetPathFIFO(fifoname);
      cocktail->AddGenerator(eposExt,"EPOS",1.);*/
      
      TString fifoname("myfifo");
      gROOT->ProcessLine(Form(".! rm -rf %s", fifoname.Data()));
      gROOT->ProcessLine(Form(".! mkfifo %s", fifoname.Data()));
      gROOT->ProcessLine(Form(".! bash $ALIDPG_ROOT/MC/EXTRA/gen_eposlhc.sh %s %d %d %f %d %f &> gen_eposlhc%d.log &",
                              fifoname.Data(), nEvents,
                              projectileId, projectileEnergy,
                              targetId, targetEnergy, pileup));
  
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
  }  // end switch
  
  return generator;
}
