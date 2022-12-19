void DetectorInitFOCAL() {
/*
Special FOCAL simulations detector init
   */
  Int_t iABSO   = 1;
  Int_t iACORDE = 1;
  Int_t iAD     = 0;
  Int_t iDIPO   = 1;
  Int_t iEMCAL  = 0;
  Int_t iFMD    = 0;
  Int_t iFRAME  = 1;
  Int_t iHALL   = 1;
  Int_t iITS    = 1;
  Int_t iMAG    = 1;
  Int_t iMUON   = 0;
  Int_t iPHOS   = 0;
  Int_t iPIPE   = 1;
  Int_t iHMPID  = 0;
  Int_t iSHIL   = 1;
  Int_t iT0     = 0;
  Int_t iTOF    = 0;
  Int_t iTPC    = 0;
  Int_t iTRD    = 0;
  Int_t iVZERO  = 0;
  Int_t iZDC    = 0;
  Int_t iFIT    = 1;
  Int_t iFOCAL  = 1;
  
  Bool_t isGeant4 = kFALSE;
  Bool_t isFluka = kFALSE;
  if (gSystem->Getenv("CONFIG_GEANT4")) {
    if (strcmp(gSystem->Getenv("CONFIG_GEANT4"), "on") == 0) {
      isGeant4 = kTRUE;        
    }
  }
  if (gSystem->Getenv("CONFIG_FLUKA")) {
    if (strcmp(gSystem->Getenv("CONFIG_FLUKA"), "on") == 0) {
      isFluka = kTRUE;        
    }
  }
  
  if (gSystem->Getenv("CONFIG_DETECTOR")) {
    if (strcmp(gSystem->Getenv("CONFIG_DETECTOR"), "FOCALnoFIT") == 0) {
      iFIT = 0;        
    }
  }
  
  Int_t year = atoi(gSystem->Getenv("CONFIG_YEAR"));

  //=================== Alice BODY parameters =============================
  AliBODY *BODY = new AliBODY("BODY", "Alice envelop");  
  if (iMAG) {
    //=================== MAG parameters ============================
    // --- Start with Magnet since detector layouts may be depending ---
    // --- on the selected Magnet dimensions ---
    AliMAG *MAG = new AliMAG("MAG", "Magnet");
  }
  
  if (iABSO) {
    //=================== ABSO parameters ============================
    AliABSO *ABSO = new AliABSOv3("ABSO", "Muon Absorber");
  }

  if (iDIPO) {
  //=================== DIPO parameters ============================
    AliDIPO *DIPO = new AliDIPOv3("DIPO", "Dipole version 3");
  }

  if (iHALL) {
  //=================== HALL parameters ============================
    AliHALL *HALL = new AliHALLv3("HALL", "Alice Hall");
  }

  if (iFRAME) {
  //=================== FRAME parameters ============================
    if (year < 2015) {
      AliFRAMEv2 *FRAME = new AliFRAMEv2("FRAME", "Space Frame");
	  FRAME->SetHoles(1);
    } else {
 	  AliFRAMEv3 *FRAME = new AliFRAMEv3("FRAME", "Space Frame");
	  FRAME->SetHoles(1);
    }
  }

  if (iSHIL) {
    //=================== SHIL parameters ============================
    AliSHIL *SHIL = new AliSHILv3("SHIL", "Shielding Version 3");
  }

  if (iPIPE) {
    //=================== PIPE parameters (special FOCAL PIPE) ============================
    AliPIPEFOCAL *PIPE = new AliPIPEFOCAL("PIPE", "Beam Pipe");
    PIPE->SetConical(35,400);
    PIPE->SetZflange(483.8);
    PIPE->SetR2(3.6);
    PIPE->SetIsConeBe(1);
    PIPE->SetIsFlangeBe(1);
    PIPE->SetConeW(0.1);
  }
 
  if (iITS) {
    //=================== ITS parameters ============================
    AliITS *ITS  = new AliITSv11("ITS","ITS v11");
  }

  if (iTPC) {
    //============================ TPC parameters =====================
    AliTPC *TPC = new AliTPCv2("TPC", "Default");
    if (isGeant4 || isFluka) 
      TPC->SetPrimaryIonisation(1);
  }

  if (iTOF) {
    //=================== TOF parameters ============================
    AliTOF *TOF = new AliTOFv6T0("TOF", "normal TOF");
  }

  if (iHMPID) {
    //=================== HMPID parameters ===========================
    AliHMPID *HMPID = new AliHMPIDv3("HMPID", "normal HMPID");
  }

  if (iZDC) {
    //=================== ZDC parameters ============================
    AliZDC *ZDC;
    if (year == 2010) {
      ZDC = new AliZDCv3("ZDC", "normal ZDC");
	  ZDC->SetSpectatorsTrack();
      ZDC->SetLumiLength(0.);
    } else if (year < 2015) {
	  ZDC = new AliZDCv3("ZDC", "normal ZDC");
	  //Collimators aperture
	  ZDC->SetVCollSideCAperture(0.85);
	  ZDC->SetVCollSideCCentre(0.);
	  ZDC->SetVCollSideAAperture(0.75);
	  ZDC->SetVCollSideACentre(0.);
	  //Detector position
	  ZDC->SetYZNC(1.6);
	  ZDC->SetYZNA(1.6);
	  ZDC->SetYZPC(1.6);
	  ZDC->SetYZPA(1.6);
    } else {
	  ZDC = new AliZDCv4("ZDC", "normal ZDC");
	  ZDC->SetLumiLength(0.);
	  ZDC->SetVCollSideCAperture(2.8);
	  ZDC->SetVCollSideCApertureNeg(2.8);
    }
  }

  if (iTRD) {
    //=================== TRD parameters ============================
    AliTRDgeometry *geoTRD;
    if (isGeant4) {
	  AliTRDtestG4 *TRD = new AliTRDtestG4("TRD", "TRD slow simulator");
	  TRD->SetScaleG4(1.11);
      geoTRD = TRD->GetGeometry();
    } else {
	  AliTRD *TRD = new AliTRDv1("TRD", "TRD slow simulator");
	  geoTRD = TRD->GetGeometry();
    }
    // Partial geometry: modules at 0,1,7,8,9,16,17
    // starting at 3h in positive direction

    if ((year == 2010) || (year == 2009)) {
	  geoTRD->SetSMstatus(2,0);
	  geoTRD->SetSMstatus(3,0);
	  geoTRD->SetSMstatus(4,0);
	  geoTRD->SetSMstatus(5,0);
	  geoTRD->SetSMstatus(6,0);
	  geoTRD->SetSMstatus(11,0);
	  geoTRD->SetSMstatus(12,0);
	  geoTRD->SetSMstatus(13,0);
	  geoTRD->SetSMstatus(14,0);
	  geoTRD->SetSMstatus(15,0);
	  geoTRD->SetSMstatus(16,0);	
    } else if (year == 2011) {
	  geoTRD->SetSMstatus(2,0);
	  geoTRD->SetSMstatus(3,0);
	  geoTRD->SetSMstatus(4,0);
	  geoTRD->SetSMstatus(5,0);
	  geoTRD->SetSMstatus(6,0);
	  geoTRD->SetSMstatus(12,0);
	  geoTRD->SetSMstatus(13,0);
	  geoTRD->SetSMstatus(14,0);
    } else if ((year == 2012) || (year ==2013)) {
	  geoTRD->SetSMstatus(4,0);
	  geoTRD->SetSMstatus(5,0);
	  geoTRD->SetSMstatus(12,0);
	  geoTRD->SetSMstatus(13,0);
	  geoTRD->SetSMstatus(14,0);
    }
  }
  
  if (iFMD) {
    //=================== FMD parameters ============================
    AliFMD *FMD = new AliFMDv1("FMD", "normal FMD");
  }

  if (iMUON) {
    //=================== MUON parameters ===========================
    // New MUONv1 version (geometry defined via builders)
    AliMUON *MUON = new AliMUONv1("MUON", "default");
    // activate trigger efficiency by cells
    MUON->SetTriggerEffCells(1); // backward compatibility
    MUON->SetTriggerResponseV1(2); // backward compatibility
  }

  if (iPHOS) {
    //=================== PHOS parameters ===========================
    if (year < 2015) {
      AliPHOS *PHOS = new AliPHOSv1("PHOS", "noCPV_Modules123");
    } else {
	  AliPHOS *PHOS = new AliPHOSv1("PHOS", "Run2");
    }
    if (gSystem->Getenv("CONFIG_PHOS_TIMERES")) {
      if ( !strcmp(gSystem->Getenv("CONFIG_PHOS_TIMERES"), "off") ) {
        printf("*** PHOS time resolution off\n");
        AliPHOSSimParam::GetInstance()->SetTOFparameters(1.e-12,1.e-12) ;
      }
    }
  }

  if (iT0) {
    //=================== T0 parameters ============================
    AliT0 *T0 = new AliT0v1("T0", "T0 Detector");
  }

  if (iEMCAL) {
    //=================== EMCAL parameters ============================
    //Cannot use EMCAL as name since it collides with EMCAL namespace definition
    AliEMCAL *EMCAL_DET = new AliEMCALv2("EMCAL", "EMCAL_COMPLETE12SMV1_DCAL_8SM");
    // by default Run2 configuration name but the proper geometry name is taken 
    // automatically depending on the anchor run 
    // 2010 - 4 SM, 2011 - 10 SM, 2012 - 12 SM, >2014 - 20 SM
  }

  if (iACORDE) {
    //=================== ACORDE parameters ============================
    AliACORDE *ACORDE = new AliACORDEv1("ACORDE", "normal ACORDE");
  }

  if (iVZERO) {
    //=================== ACORDE parameters ============================
    AliVZERO *VZERO = new AliVZEROv7("VZERO", "normal VZERO");
  }  

  if (iAD) {
    if (!isFluka) { // does not work for now with Fluka
      //=================== AD parameters ============================
	  // AliAD *AD = new AliADv1("AD", "normal AD");
      AliAD *AD = NULL;
      if (year>0) 
        AD = new AliADv1("AD", Form("normal AD year=%d", year));
      else        
        AD = new AliADv1("AD", "normal AD");
    } else {
	  Printf("AD is disabled, as we are using Fluka");
    }
  }
  
  if (iFOCAL) {
    //=================== FOCAL parameters ============================
    const char *gfname="";
    if (gSystem->Getenv("CONFIG_FOCALGEOMETRYFILE")) {
      gfname = gSystem->Getenv("CONFIG_FOCALGEOMETRYFILE");
    }
    AliFOCAL *FOCAL = new AliFOCALv2("FOCAL","FOCAL version-2",gfname);
  }
  
  if (iFIT) {
    AliFIT *fit = new AliFITv9("FIT","FIT");
  }
}
