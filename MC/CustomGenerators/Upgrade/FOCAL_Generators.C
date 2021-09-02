Double_t EtaToTheta(Double_t eta)
{
  return 2*TMath::ATan(TMath::Exp(-eta))*180/TMath::Pi();
}

// Generators for FOCAL simulations
enum GenTypes : Int_t {
    gun=0, 
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

Int_t gNtracks = 1;

AliGenerator* GeneratorCustom(TString opt = "") {

  Int_t generatorType = gun;
  
  for (Int_t iopt = 0; iopt < kNGenTypes; iopt++)
    if (opt.EqualTo(gGenTypeNames[iopt]))
      generatorType = iopt;
    
  //switch between the different generators
  AliGenerator * generator = 0x0;
  switch(generatorType) {
    case gun:
    // Example for Fixed Particle Gun             
    {
      AliGenFixed *gener = new AliGenFixed(gNtracks);
	  gener->SetMomentum(500.);
	  gener->SetPhi(0.);
	  //gener->SetTheta(0.0556*180./TMath::Pi());   // to hit FOCAL at 20cm from beam axis at Z=360cm
	  gener->SetTheta(EtaToTheta(4.5));
	  gener->SetOrigin(0,0,0);        //vertex position
	  gener->SetPart(kGamma);
	  //gener->SetPart(kPiPlus);
	  generator = gener;
    }
    break;

    case box:  
    // Example for Moving Particle Gun  
    {
      AliGenBox *gener = new AliGenBox(gNtracks);
	  //gener->SetMomentumRange(49.999,50.001);
	  gener->SetPtRange(0.,20.);
	  gener->SetPhiRange(0.,360.);  // full polar angle around beam axis
	  gener->SetEtaRange(3.5, 5.8);
	  gener->SetOrigin(0,0,0);   
	  //vertex position
	  gener->SetSigma(0,0,0);         //Sigma in (X,Y,Z) (cm) on IP position
	  gener->SetPart(kPi0);
	  generator = gener;
    }
    break;

    case pythia:
    // Example for Pythia            
    {
	  AliGenPythia *gener = new AliGenPythia(-1); 
	  gener->SetMomentumRange(0,999999); 
	  gener->SetThetaRange(0., 45.); 
	  gener->SetYRange(-12,12); 
	  gener->SetPtRange(0,1000); 
	  gener->SetProcess(kPyMb); // Min. bias events 
	  gener->SetEnergyCMS(14000.); // LHC energy 
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
      AliGenPythiaFOCAL *gener = new AliGenPythiaFOCAL(-1); 
	  gener->SetMomentumRange(0,999999); 
	  gener->SetThetaRange(0., 45.); 
	  gener->SetYRange(-12,12); 
	  gener->SetPtRange(0,1000); 
	  gener->SetEnergyCMS(14000.); // LHC energy 
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
      gener->SetEnergyCMS(5500.); // center of mass energy 
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
      gener->SetEnergyCMS(8790.); // center of mass energy 
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
      hijing->SetEnergyCMS(8790.); // center of mass energy 
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
	  pythia->SetEnergyCMS(8790.); // LHC energy
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
      AliGenCocktailFOCAL *cocktail = new AliGenCocktailFOCAL();
      cocktail->SetOrigin(0, 0, 0); // Vertex position
      cocktail->SetSigma(0, 0, 5.8); // Sigma in (X,Y,Z) (cm) on IP position
      cocktail->SetVertexSmear(kPerEvent); // Smear per event

      TString fifoname("$TMPDIR/myfifo");
      AliGenExtExec* eposExt = new AliGenExtExec();
      eposExt->SetPathScript(Form("./gen_eposlhc.sh %s",fifoname.Data()));
      eposExt->SetPathFIFO(fifoname);
      cocktail->AddGenerator(eposExt,"EPOS",1.);

      AliGenPythiaFOCAL *pythia = new AliGenPythiaFOCAL(-1);
      pythia->SetMomentumRange(0,999999);
      pythia->SetThetaRange(0., 45.);
      pythia->SetYRange(-12,12);
      pythia->SetPtRange(0,1000);
      pythia->SetEnergyCMS(8790.); // LHC energy
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
