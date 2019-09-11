void Geant4Config()
{

  if (gClassTable->GetID("TGeant4") == -1) {
    // LOCAL TEST
    TString g4libsMacro="$ALIDPG_ROOT/MC/g4libs.C";
    //*TString g4libsMacro="$ALICE/geant4_vmc/examples/examples/macro/g4libs.C";
    
    //Load Geant4 libraries 
    if (!gInterpreter->IsLoaded(g4libsMacro.Data())) {
      gROOT->LoadMacro(g4libsMacro.Data());
      gInterpreter->ProcessLine("g4libs()");
    }
  }    
  
  // Create Geant4 VMC
  //  
  TGeant4 *geant4 = 0;
  if ( ! gMC ) {
    TG4RunConfiguration* runConfiguration = NULL;
    TString configPhysList = "FTFP_INCLXX_EMV+optical";
    if (g4PhysList == "BERT") configPhysList = "FTFP_BERT_EMV+optical";
    else if (g4PhysList == "BERT+biasing") configPhysList = "FTFP_BERT_EMV+optical+biasing";
    // check if monopole injection requested
    if(abs(pdgConfig)<60000000 || abs(pdgConfig)>=70000000){
      
      runConfiguration = new TG4RunConfiguration("geomRoot",
						 Form("%s", configPhysList.Data()),
						 "specialCuts+stackPopper+stepLimiter",
						 true);
      geant4 = new TGeant4("TGeant4", 
			   Form("The Geant4 Monte Carlo : %s", configPhysList.Data()), 
			   runConfiguration);
    }
    else{
      Printf("\n\nSET UP GEANT4 for magnetic monopoles\n\n");
      if (g4PhysList == "BERT+biasing") {
	Printf("We cannot use the BERT+biasing with the monopoles, we will unset the pointer to geant4");
	delete geant4;
	geant4 = 0;
	return;
      }
      //decode mass, electric charge and magnetic charge:  pdgConfig= sign 6abcdefg -> mass= c.defg*10^g; el charge=sign a; mag. charge= sign b
      Double_t mass=Double_t (int(abs(pdgConfig)%(int(1e5))/10)/1000.)*pow(10,(int(abs(pdgConfig)%10)));
      Double_t eCh=(abs(pdgConfig)/pdgConfig)*abs(pdgConfig)/(int(1e6))%10;
      Double_t mCh=(abs(pdgConfig)/pdgConfig)*abs(pdgConfig)/(int(1e5))%10;
      pdgConfig=60000000;
      Printf("\n PDG Code: %d, mass: %3.3f, el. charge: %d; mag. charge: %d",pdgConfig,mass,eCh,mCh);

      
      runConfiguration = new TG4RunConfiguration("geomRoot",
						 Form("%s+monopole", configPhysList.Data()),
						 "specialCuts+stackPopper+stepLimiter",
						 true);
      
      runConfiguration->SetParameter("monopoleMass", mass);
      runConfiguration->SetParameter("monopoleElCharge", eCh);
      runConfiguration->SetParameter("monopoleMagCharge", mCh);
      
      geant4 = new TGeant4("TGeant4", 
			   Form("The Geant4 Monte Carlo : %s+monopole", configPhysList.Data()), 
			   runConfiguration);
    }
    
    cout << "Geant4 has been created." << endl;
  } 
  else {
    cout << "Monte Carlo has been already created." << endl;
  }  
  
  //((AliMC*)gMC)->SetUseMonitoring(1);
  
  // Customization of Geant4 VMC
  //
  geant4->ProcessGeantCommand("/control/verbose 2");  
  geant4->ProcessGeantCommand("/mcVerbose/all 1");  
  geant4->ProcessGeantCommand("/mcVerbose/geometryManager 1");  
  geant4->ProcessGeantCommand("/mcVerbose/opGeometryManager 1");  
  geant4->ProcessGeantCommand("/mcTracking/loopVerbose 1");     
  geant4->ProcessGeantCommand("/mcPhysics/rangeCuts 0.01 mm"); 
  
  geant4->ProcessGeantCommand("/mcVerbose/composedPhysicsList 2");  
  geant4->ProcessGeantCommand("/mcTracking/skipNeutrino true");
  geant4->ProcessGeantCommand("/mcDet/setIsMaxStepInLowDensityMaterials true");
  geant4->ProcessGeantCommand("/mcDet/setMaxStepInLowDensityMaterials 10 m");
  geant4->ProcessGeantCommand("/mcMagField/setConstDistance 1 mm");
  //
  // optical
  //
  geant4->ProcessGeantCommand("/process/optical/verbose 0");
  geant4->ProcessGeantCommand("/process/optical/processActivation Scintillation 0");
  geant4->ProcessGeantCommand("/process/optical/processActivation OpWLS 0");
  geant4->ProcessGeantCommand("/process/optical/processActivation OpMieHG 0");
  geant4->ProcessGeantCommand("/process/optical/setTrackSecondariesFirst Cerenkov 0");
  geant4->ProcessGeantCommand("/mcMagField/stepperType NystromRK4");
  
  //
  // PAI for TRD
  //
  // Geant4 VMC >= v3.2
  geant4->ProcessGeantCommand("/mcPhysics/emModel/setEmModel  PAI");
  geant4->ProcessGeantCommand("/mcPhysics/emModel/setRegions  TRD_Gas-mix");
  geant4->ProcessGeantCommand("/mcPhysics/emModel/setParticles  all");
  // Geant4 VMC < v3.2
  // Int_t trdGas = gMC->MediumId("TRD_Gas-mix");
  // geant4->ProcessGeantCommand(Form("/mcPhysics/emModel/selectMedium %3d", trdGas));
  // geant4->ProcessGeantCommand("/mcPhysics/emModel/setElossModel  PAI");
  // geant4->ProcessGeantCommand("/mcPhysics/emModel/setFluctModel  PAI");
  // geant4->ProcessGeantCommand("/mcPhysics/emModel/setParticles   all");
  // geant4->ProcessGeantCommand("/mcDet/printMedia");
  
  //
  // Precise Msc for EMCAL
  //
  // Geant4 VMC >= v3.2
  geant4->ProcessGeantCommand("/mcPhysics/emModel/setEmModel  SpecialUrbanMsc");
  geant4->ProcessGeantCommand("/mcPhysics/emModel/setRegions  EMCAL_Lead$ EMCAL_Scintillator$");
  geant4->ProcessGeantCommand("/mcPhysics/emModel/setParticles  e- e+");
  
  // Print media
  //geant4->ProcessGeantCommand("/mcDet/printMedia");
  //geant4->ProcessGeantCommand("/mcVerbose/physicsEmModel 2"); 
  //geant4->ProcessGeantCommand("/mcVerbose/regionsManager 2"); 
  
  // Magnetic monopole settings
  if(!(abs(pdgConfig)<60000000 || abs(pdgConfig)>=70000000)){
    geant4->ProcessGeantCommand("/mcMagField/setConstDistance 1 mm");
    geant4->ProcessGeantCommand("/mcMagField/setIsMonopole true");
  }

  geant4->ProcessGeantCommand("/mcMagField/setDeltaIntersection 1.0e-05 mm");
  geant4->ProcessGeantCommand("/mcMagField/setMinimumEpsilonStep 0.5e-05");
  geant4->ProcessGeantCommand("/mcMagField/setMaximumEpsilonStep 1.0e-05");

  if (g4PhysList == "BERT+biasing") {
    geant4->ProcessGeantCommand("/mcVerbose/biasingConfigurationManager 1");
    geant4->ProcessGeantCommand("/mcPhysics/biasing/setModel inclxx");
    
    //geant4->ProcessGeantCommand("/mcPhysics/biasing/setRegions ITS_AIR$ ITS_WATER$ ITS_COPPER$ ITS_KAPTON(POLYCH2)$ ITS_GLUE_IBFPC$ ITS_CERAMIC$ ITS_K13D2U2k$ ITS_K13D2U120$ ITS_F6151B05M$ ITS_M60J3K$ ITS_M55J6K$ ITS_FGS003$ ITS_CarbonFleece$ ITS_PEEKCF30$ ITS_GLUE$ ITS_ALUMINUM$ ITS_INOX304$ ALPIDE_METALSTACK$ ALPIDE_SI$");
    geant4->ProcessGeantCommand("/mcPhysics/biasing/setRegions ITS* PIPE_BE*");
    
    geant4->ProcessGeantCommand("/mcPhysics/biasing/setParticles proton neutron pi+ pi-");
  }
  
}

