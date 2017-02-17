Geant4Config()
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
    TG4RunConfiguration* runConfiguration = new TG4RunConfiguration("geomRoot",
								    "FTFP_BERT_EMV+optical",
								    "specialCuts+stackPopper+stepLimiter",
								    true);
    geant4 = new TGeant4("TGeant4", 
			 "The Geant4 Monte Carlo : FTFP_BERT_EMV", 
			 runConfiguration);
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
  
}

