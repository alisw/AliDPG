/*
 * AliDPG - ALICE Experiment Data Preparation Group
 * Generator configuration script
 *
 */

enum EGenerator_t {
  kGeneratorDefault,
  kGeneratorPythia6,
  kGeneratorPythia6_Perugia2011,
  kGeneratorPythia8,
  kGeneratorPythia8_Monash2013,
  kGeneratorPythia8_Monash2013_Rsn001, // [ALIROOT-6685]
  kGeneratorPhojet,
  kGeneratorEPOSLHC, kGeneratorEPOSLHC_pp, kGeneratorEPOSLHC_PbPb,
  kGeneratorHijing,
  kGeneratorHijing_Rsn002a, kGeneratorHijing_Rsn002b, kGeneratorHijing_Rsn002c, // [ALIROOT-6721] [ALIROOT-6722]
  kGeneratorHijing_Jpsiee001, // [ALIROOT-6750]
  kGeneratorHijing_Nuclex001, // [ALIROOT-6795]
  kGeneratorHijing_Jets001,   // [ALIROOT-xxxx]
  kGeneratorStarlight,
  kGeneratorCustom,
  kNGenerators
};

const Char_t *GeneratorName[kNGenerators] = {
  "Default",
  "Pythia6",
  "Pythia6_Perugia2011",
  "Pythia8",
  "Pythia8_Monash2013",
  "Pythia8_Monash2013_Rsn001",
  "Phojet",
  "EPOSLHC", "EPOSLHC_pp", "EPOSLHC_PbPb",
  "Hijing",
  "Hijing_Rsn002a", "Hijing_Rsn002b", "Hijing_Rsn002c",
  "Hijing_Jpsiee001",
  "Hijing_Nuclex001",
  "Hijing_Jets001",
  "Starlight",
  "Custom"
};

enum ETrigger_t {
  kTriggerDefault,
  kTriggerPP,
  kTriggerPbPb,
  kNTriggers
};

const Char_t *TriggerName[kNTriggers] = {
  "ocdb",
  "p-p",
  "Pb-Pb"
};

enum EPythia6Tune_t {
  kPythia6Tune_Perugia0    = 320,
  kPythia6Tune_Perugia2011 = 350
};

enum EPythia8Tune_t {
  kPythia8Tune_Monash2013  = 14
};

/*****************************************************************/
/*****************************************************************/
/*****************************************************************/

// functions
AliGenerator *GeneratorCocktail(TString name);
AliGenerator *GeneratorInjector(Int_t ninj, Int_t pdg, Float_t ptmin, Float_t ptmax, Float_t ymin, Float_t ymax); 
AliGenerator *GeneratorPythia6(Int_t tune = 0, Int_t ntrig = 0, Int_t *trig = NULL);
AliGenerator *GeneratorPythia8(Int_t tune = 0, Int_t ntrig = 0, Int_t *trig = NULL);
AliGenerator *GeneratorPythia8Jets();
AliGenerator *GeneratorPhojet();
AliGenerator *GeneratorEPOSLHC();
AliGenerator *GeneratorHijing();
AliGenerator *Generator_Jpsiee(const Char_t *params, Float_t jpsifrac, Float_t lowfrac, Float_t highfrac, Float_t bfrac);
AliGenerator *Generator_Nuclex(UInt_t injbit = 0xFFFFFFFF, Float_t scale = 1.);
AliGenerator *GeneratorStarlight();

/*****************************************************************/

// global variables
static TString comment;

/*****************************************************************/

GeneratorConfig(Int_t tag)
{

  AliGenerator *gen = NULL;
  
  switch (tag) {

    // Default
  case kGeneratorDefault:
    abort();
    return;
    
    // Pythia6 (Perugia2011)
  case kGeneratorPythia6:
  case kGeneratorPythia6_Perugia2011:
    gen = GeneratorPythia6(kPythia6Tune_Perugia2011);
    break;

    // Pythia8 (Monash2013)
  case kGeneratorPythia8:
  case kGeneratorPythia8_Monash2013:
    gen = GeneratorPythia8(kPythia8Tune_Monash2013);
    break;
    
    // Pythia8 (Monash2013) - Rsn001
  case kGeneratorPythia8_Monash2013_Rsn001:
    AliGenCocktail *ctl = GeneratorCocktail("Monash2013_Rsn001");
    // pythia8
    AliGenerator   *py8 = GeneratorPythia8(kPythia8Tune_Monash2013);
    ctl->AddGenerator(py8, "Pythia8 (Monash2013)", 1.);
    // randomly injected particles
    Int_t pdglist[] = {225, 3124, -3124, 9010221}; // f2(1270), Lambda(1520), Lambda_bar(1520), f0(980)
    Int_t pdg = pdglist[uidConfig % (sizeof(pdglist) / 4)]; // select according to unique ID
    AliGenerator   *inj = GeneratorInjector(1, pdg, 0., 15., -0.6, 0.6);
    ctl->AddGenerator(inj, "Injector (Rsn001)", 1.);
    //
    gen = ctl;
    break;
    
    // Phojet
  case kGeneratorPhojet:
    gen = GeneratorPhojet();
    break;

    // EPOSLHC
  case kGeneratorEPOSLHC:
  case kGeneratorEPOSLHC_pp:
  case kGeneratorEPOSLHC_PbPb:
    gen = GeneratorEPOSLHC();
    break;

    // Hijing
  case kGeneratorHijing:
    gen = GeneratorHijing();
    break;

    // Starlight
  case kGeneratorStarlight:
    gen = GeneratorStarlight();
    break;

    // Hijing - Rsn002
  case kGeneratorHijing_Rsn002a:
  case kGeneratorHijing_Rsn002b:
  case kGeneratorHijing_Rsn002c:
    Int_t ninjlist[3] = {25, 7, 3};
    Int_t ninj = ninjlist[tag - kGeneratorHijing_Rsn002a];
    AliGenCocktail *ctl  = GeneratorCocktail("Hijing_Rsn002");
    AliGenerator   *hij  = GeneratorHijing();
    AliGenerator   *inj1 = GeneratorInjector(ninj,  3124, 0., 10., -0.6, 0.6);
    AliGenerator   *inj2 = GeneratorInjector(ninj, -3124, 0., 10., -0.6, 0.6);
    ctl->AddGenerator(hij,  "Hijing",            1.);
    ctl->AddGenerator(inj1, "Injector (Rsn002)", 1.);
    ctl->AddGenerator(inj2, "Injector (Rsn002)", 1.);
    gen = ctl;
    break;

    // Hijing - Jpsiee001
  case kGeneratorHijing_Jpsiee001:
    AliGenCocktail *ctl   = GeneratorCocktail("Hijing_Jpsiee001");
    AliGenerator   *hij   = GeneratorHijing();
    ctl->AddGenerator(hij,  "Hijing", 1.);    
    if (uidConfig % 10 < 7) {
      AliGenerator *jpsi  = Generator_Jpsiee("PbPb 2.76", 1.0, 0.3, 0.3, 0.0);
      ctl->AddGenerator(jpsi, "Jpsi2ee", 1., new TFormula("ten", "10."));
      TFile *file = new TFile("typeHF_4.proc", "recreate");
      file->Close();
    }
    else {
      AliGenerator *bjpsi = Generator_Jpsiee("Pythia BBar", 0.0, 0.0, 0.0, 1.0);
      ctl->AddGenerator(bjpsi, "B2Jpsi2ee", 1., new TFormula("ten", "10."));
      TFile *file = new TFile("typeHF_5.proc", "recreate");
      file->Close();
    }
    gen = ctl;
    break;
    
    // Hijing - Nuclex001
  case kGeneratorHijing_Nuclex001:
    AliGenCocktail *ctl   = GeneratorCocktail("Hijing_Nuclex001");
    AliGenerator   *hij   = GeneratorHijing();
    ctl->AddGenerator(hij,  "Hijing", 1.);
    AliGenerator   *nux   = Generator_Nuclex(0xFFFFFFFF, 1.);
    ctl->AddGenerator(nux,  "Nuclex", 1.);
    gen = ctl;
    break;
    
    // Hijing - Jets001
  case kGeneratorHijing_Jets001:
    AliGenCocktail *ctl   = GeneratorCocktail("Hijing_Jets001");
    AliGenerator   *hij   = GeneratorHijing();
    ctl->AddGenerator(hij,  "Hijing", 1.);
    AliGenerator   *jet   = GeneratorPythia8Jets();
    ctl->AddGenerator(jet,  "Jets", 1.);
    gen = ctl;
    break;
    
    // Custom
  case kGeneratorCustom:
    if ((gROOT->LoadMacro("GeneratorCustom.C")) != 0) {
      printf("ERROR: cannot find GeneratorCustom.C\n");
      abort();
      return;
    }
    gen = GeneratorCustom();

  }

  // default diamond parameters
  Float_t betast  = 10.;                 // beta* [m]
  if (runNumber >= 117048) betast = 2.;
  if (runNumber >  122375) betast = 3.5; // starting with fill 1179
  Float_t eps     = 5.e-6;            // emittance [m]
  Float_t gamma   = energyConfig / 2.0 / 0.938272;  // relativistic gamma [1]
  Float_t sigmaxy = TMath::Sqrt(eps * betast / gamma) / TMath::Sqrt(2.) * 100.;  // LHC period specific
  if (gSystem->Getenv("CONFIG_PERIOD")) {
    TString periodName = gSystem->Getenv("CONFIG_PERIOD");
    if (periodName.EqualTo("LHC10h")) sigmaxy =  80.e-4;
    if (periodName.EqualTo("LHC11a")) sigmaxy = 120.e-4;
    if (periodName.EqualTo("LHC11h")) sigmaxy = 110.e-4;
    if (periodName.EqualTo("LHC13b")) sigmaxy =  35.e-4;
    if (periodName.EqualTo("LHC13c")) sigmaxy =  35.e-4;
    if (periodName.EqualTo("LHC13d")) sigmaxy =  35.e-4;
    if (periodName.EqualTo("LHC13e")) sigmaxy =  35.e-4;
    if (periodName.EqualTo("LHC15n")) sigmaxy =  90.e-4;
    if (periodName.EqualTo("LHC15o")) sigmaxy =  30.e-4;
  }

  gen->SetOrigin(0., 0., 0.);
  gen->SetSigma(sigmaxy, sigmaxy, 5.);
  
  gen->SetVertexSmear(kPerEvent);
  gen->Init();
  printf(">>>>> Generator Configuration: %s \n", comment.Data());
  // Set the trigger configuration: proton-proton
  AliSimulation::Instance()->SetTriggerConfig(TriggerName[triggerConfig]);
  printf(">>>>> Trigger configuration:   %s \n", TriggerName[triggerConfig]);
 
}

/*** PYTHIA 6 ****************************************************/

AliGenerator *
GeneratorPythia6(Int_t tune, Int_t ntrig, Int_t *trig)
{
  comment = comment.Append(" | Pythia6 low-pt");
  //
  // Pythia
  AliGenPythia* pythia = new AliGenPythia(-1); 
  pythia->SetMomentumRange(0, 999999.);
  pythia->SetThetaRange(0., 180.);
  pythia->SetYRange(-12.,12.);
  pythia->SetPtRange(0,1000.);
  pythia->SetProcess(kPyMb);
  pythia->SetEnergyCMS(energyConfig);
  pythia->SetCrossingAngle(0,crossingConfig);
  //
  // Tune
  if (tune > 0) {
    comment = comment.Append(Form(" | tune %d", tune));
    pythia->SetTune(tune); 
    //    pythia->UseNewMultipleInteractionsScenario();
  }
  //
  // Trigger particles
  if (ntrig > 0) {
    Int_t pdg = trig[gRandom->Integer(ntrig)];
    comment = comment.Append(Form(" | %s enhanced", TDatabasePDG::Instance()->GetParticle(pdg)->GetName()));
    pythia->SetTriggerParticle(pdg, 1.2);
  }
  //
  return pythia;
}

/*** PYTHIA 8 ****************************************************/

AliGenerator *
GeneratorPythia8(Int_t tune, Int_t ntrig, Int_t *trig)
{
  //
  // Libraries
  gSystem->Load("libpythia8.so");
  gSystem->Load("libAliPythia8.so");
  //
  //
  comment = comment.Append(" | Pythia8 low-pt");
  //
  // Pythia
  AliGenPythiaPlus *pythia = new AliGenPythiaPlus(AliPythia8::Instance()); 
  pythia->SetMomentumRange(0, 999999.);
  pythia->SetThetaRange(0., 180.);
  pythia->SetYRange(-12.,12.);
  pythia->SetPtRange(0,1000.);
  pythia->SetProcess(kPyMbDefault); // pythia->SetProcess(kPyMb);
  pythia->SetEnergyCMS(energyConfig);
  pythia->SetCrossingAngle(0, crossingConfig);
  //
  // Initialize
  pythia->SetEventListRange(-1, 2); 
  (AliPythia8::Instance())->ReadString("Random:setSeed = on");
  (AliPythia8::Instance())->ReadString(Form("Random:seed = %ld", seedConfig % 900000000)); 
  //
  // Tune
  if (tune > 0) {
    comment = comment.Append(Form(" | tune %d", tune));
    (AliPythia8::Instance())->ReadString(Form("Tune:pp = %d", tune));
  }
  //
  // Trigger particles
  if (ntrig > 0) {
    Int_t pdg = trig[gRandom->Integer(ntrig)];
    comment = comment.Append(Form(" | %s enhanced", DatabasePDG::Instance()->GetParticle(pdg)->GetName()));
    pythia->SetTriggerParticle(pdg, 1.2);
  }
  //
  return pythia;
}

/*** PYTHIA 8 JETS ***********************************************/

AliGenerator *
GeneratorPythia8Jets()
{
  //
  //
  comment = comment.Append(" | Pythia8 jets (%.1f, %.1f, %d, %.1f)", pthardminConfig, pthardmaxConfig, quenchingConfig, qhatConfig);
  //
  // Pythia
  AliGenPythiaPlus *pythia = GeneratorPythia8();
  //
  // jets settings
  pythia->SetProcess(kPyJets);
  pythia->SetJetEtaRange(-1.5, 1.5); // Final state kinematic cuts
  pythia->SetJetPhiRange(0., 360.);
  pythia->SetJetEtRange(5., 1000.);
  pythia->SetPtHard(pthardminConfig, pthardmaxConfig); // Pt transfer of the hard scattering
  pythia->SetStrucFunc(kCTEQ5L);
  // quenching
  pythia->SetQuench(quenchingConfig);
  switch (quenchingConfig) {
  case 1:
    Float_t k = 6.e5 * (qhatConfig / 1.7);  //qhat=1.7, k=6e5, default value
    AliPythia8::Instance()->InitQuenching(0., 0.1, k, 0, 0.95, 6);		
    break;
  case 2:
    pythia->SetPyquenPar(1.,0.1,0,0,1);			
    break;
  }
  //
  return pythia;
}

/*** PHOJET ****************************************************/

AliGenerator *
GeneratorPhojet()
{
  //
  // Libraries
  gSystem->Load("libDPMJET");
  gSystem->Load("libTDPMjet");
  //
  comment = comment.Append(" | Phojet low-pt");
  //                                                                                      
  //    DPMJET                                                                            
  AliGenDPMjet* dpmjet = new AliGenDPMjet(-1);
  dpmjet->SetMomentumRange(0, 999999.);
  dpmjet->SetThetaRange(0., 180.);
  dpmjet->SetYRange(-12.,12.);
  dpmjet->SetPtRange(0,1000.);
  dpmjet->SetProcess(kDpmMb);
  dpmjet->SetEnergyCMS(energyConfig);
  dpmjet->SetCrossingAngle(0,crossingConfig);
  return dpmjet;
}

/*** EPOSLHC ****************************************************/

AliGenerator *
GeneratorEPOSLHC()
{

  comment = comment.Append(Form(" | EPOS-LHC (%s)", systemConfig.Data()));
  //
  // configure projectile/target
  Int_t projectileId = 0; // PDG or Z*10000+A*10
  Float_t projectileEnergy = 0.; // momentum/(GeV/c)
  Int_t targetId = 0;
  Float_t targetEnergy = 0;
  // pp
  if (systemConfig.EqualTo("p-p")) {
    projectileId = 2212;
    targetId = projectileId;
    projectileEnergy = energyConfig / 2.;
    targetEnergy = energyConfig / 2.;
  }
  // PbPb
  else if (systemConfig.EqualTo("Pb-Pb")) {
    projectileId = 82*10000 + 208*10;
    targetId = projectileId;
    projectileEnergy = energyConfig / 2.;
    targetEnergy = energyConfig / 2.;
  }
  // not implemented
  else {
    printf("EPOSLHC not implemented for %s system\n", systemConfig.Data());
    abort();
  }
  //
  // run CRMC
  TString fifoname = "crmceventfifo";
  gROOT->ProcessLine(Form(".! rm -rf %s", fifoname.Data()));
  gROOT->ProcessLine(Form(".! mkfifo %s", fifoname.Data()));
  gROOT->ProcessLine(Form(".! sh $ALIDPG_ROOT/MC/EXTRA/gen_eposlhc.sh %s %d %d %f %d %f &> gen_eposlhc.log &",
			  fifoname.Data(), neventsConfig,
			  projectileId, projectileEnergy,
			  targetId, targetEnergy));
  //
  // connect HepMC reader
  AliGenReaderHepMC *reader = new AliGenReaderHepMC();
  reader->SetFileName("crmceventfifo");
  AliGenExtFile *gener = new AliGenExtFile(-1);
  gener->SetName(Form("EPOSLHC_%s", systemConfig.Data()));
  gener->SetReader(reader);
  
  return gener;
}

/*** HIJING ****************************************************/

AliGenerator * 
GeneratorHijing()
{
  //
  // Libraries
  gSystem->Load("libHIJING");
  gSystem->Load("libTHijing");

  comment = comment.Append(Form(" | HIJING (b = %f-%f fm)", bminConfig, bmaxConfig));
  AliGenHijing *gener = new AliGenHijing(-1);
  // centre of mass energy
  gener->SetEnergyCMS(energyConfig);
  gener->SetImpactParameterRange(bminConfig, bmaxConfig);
  // reference frame
  gener->SetReferenceFrame("CMS");
  // projectile
  gener->SetProjectile("A", 208, 82);
  gener->SetTarget    ("A", 208, 82);
  // tell hijing to keep the full parent child chain
  gener->KeepFullEvent();
  // enable jet quenching
  gener->SetJetQuenching(0);
  // enable shadowing
  gener->SetShadowing(1);
  // neutral pion and heavy particle decays switched off
  gener->SetDecaysOff(1);
  // Don't track spectators
  gener->SetSpectators(0);
  // kinematic selection
  gener->SetSelectAll(0);
  gener->SetPtHardMin (2.3);
  return gener;
}

/*** STARLIGHT ****************************************************/

AliGenerator *
GeneratorStarlight(){
  //
  gSystem->Load("libStarLight.so");
  gSystem->Load("libAliStarLight.so");

  // Supported processes:
  // kTwoGammaToMuLow    - from 0.4 to 15 GeV
  // kTwoGammaToElLow    - from 0.4 to 15 GeV
  // kTwoGammaToMuMedium - from 2.0 to 15 GeV
  // kTwoGammaToElMedium - from 2.0 to 15 GeV
  // kTwoGammaToMuHigh   - from 4.0 to 15 GeV
  // kTwoGammaToElHigh   - from 4.0to 15 GeV
  // kTwoGammaToRhoRho
  // kTwoGammaToF2
  // kCohRhoToPi
  // kCohJpsiToMu
  // kCohJpsiToEl
  // kCohPsi2sToMu
  // kCohPsi2sToEl
  // kCohPsi2sToMuPi
  // kCohPsi2sToElPi
  // kCohUpsilonToMu
  // kCohUpsilonToEl
  // kIncohRhoToPi
  // kIncohJpsiToMu
  // kIncohJpsiToEl
  // kIncohPsi2sToMu
  // kIncohPsi2sToEl
  // kIncohPsi2sToMuPi
  // kIncohPsi2sToElPi
  // kIncohUpsilonToMu
  // kIncohUpsilonToEl
  // kCohRhoPrime
  // kIncohRhoPrime
  
  comment.Append(Form(" | Starlight %s (%s)", processConfig.Data(), systemConfig.Data()));
  
  Int_t projA=1,targA=1,projZ=1,targZ=1;
  // pp
  if (systemConfig.EqualTo("p-p")) {
    projA = 1; projZ = 1;
    targA = 1; targZ = 1;
  }
  // PbPb
  else if (systemConfig.EqualTo("Pb-Pb")) {
    projA = 208; projZ = 82;
    targA = 208; targZ = 82;
  }
  // p-Pb
  else if (systemConfig.EqualTo("p-Pb")) {
    projA =   1; projZ =  1;
    targA = 208; targZ = 82;
  }
  // Pb-p
  else if (systemConfig.EqualTo("Pb-p")) {
    projA = 208; projZ = 82;
    targA =   1; targZ =  1;
  }
  
  Float_t beam1energy = TMath::Sqrt(Double_t(projZ)/projA*targA/targZ)*energyConfig/2;
  Float_t beam2energy = TMath::Sqrt(Double_t(projA)/projZ*targZ/targA)*energyConfig/2;
  Float_t gamma1  = beam1energy/0.938272;
  Float_t gamma2  = beam2energy/0.938272;

  Int_t prod_mode = 4;     // default incoherent
  if      (processConfig.Contains("TwoGammaToMu")) prod_mode = 1;
  else if (processConfig.Contains("TwoGammaToEl")) prod_mode = 1;
  else if (processConfig.Contains("CohRho"))       prod_mode = 3;
  else if (processConfig.Contains("CohPhi"))       prod_mode = 2;
  else if (processConfig.Contains("CohJpsi"))      prod_mode = 2;
  else if (processConfig.Contains("CohPsi2s"))     prod_mode = 2;
  else if (processConfig.Contains("CohUpsilon"))   prod_mode = 2;
  
  Int_t prod_pid = 443013; // default jpsi
  if      (processConfig.Contains("JpsiToMu"))         prod_pid = 443013;
  else if (processConfig.Contains("JpsiToEl"))         prod_pid = 443011;
  else if (processConfig.Contains("Psi2sToMu"))        prod_pid = 444013;
  else if (processConfig.Contains("Psi2sToEl"))        prod_pid = 444011;
  else if (processConfig.Contains("UpsilonToMu"))      prod_pid = 553013;
  else if (processConfig.Contains("UpsilonToEl"))      prod_pid = 553011;
  else if (processConfig.Contains("RhoToPi"))          prod_pid = 113;
  else if (processConfig.Contains("PhiToKa"))          prod_pid = 333;
  else if (processConfig.Contains("TwoGammaToMu"))     prod_pid = 13;
  else if (processConfig.Contains("TwoGammaToEl"))     prod_pid = 11;
  else if (processConfig.Contains("TwoGammaToF2"))     prod_pid = 225;
  else if (processConfig.Contains("TwoGammaToRhoRho")) prod_pid = 33;
  
  Float_t wmin = -1;       // automatic wmin
  Float_t wmax = 15;
  if      (processConfig.Contains("Low"))    wmin = 0.4;
  else if (processConfig.Contains("Medium")) wmin = 2.0;
  else if (processConfig.Contains("High"))   wmin = 4.0;
  Int_t nwbins = 20*(wmax-wmin);
  
  Bool_t cocktail = 0;
  cocktail |= processConfig.Contains("Psi2sToElPi");
  cocktail |= processConfig.Contains("Psi2sToMuPi");
  cocktail |= processConfig.Contains("RhoPrime");
  
  AliGenCocktail *genCocktail = 0;
  if (cocktail)  genCocktail = new AliGenCocktail(); // constructor must be called before other generators

  AliGenStarLight* genStarLight = new AliGenStarLight(1000*1000); 
  genStarLight->SetParameter(Form("BEAM_1_Z     =    %3i    #Z of target",targZ));
  genStarLight->SetParameter(Form("BEAM_1_A     =    %3i    #A of target",targA)); 
  genStarLight->SetParameter(Form("BEAM_2_Z     =    %3i    #Z of projectile",projZ));
  genStarLight->SetParameter(Form("BEAM_2_A     =    %3i    #A of projectile",projA));
  genStarLight->SetParameter(Form("BEAM_1_GAMMA = %6.1f    #Gamma of the target",gamma1));
  genStarLight->SetParameter(Form("BEAM_2_GAMMA = %6.1f    #Gamma of the projectile",gamma2));
  genStarLight->SetParameter("W_MAX        =   15    #Max value of w");
  genStarLight->SetParameter(Form("W_MIN        =   %.1f    #Min value of w",wmin));
  genStarLight->SetParameter(Form("W_N_BINS     =    %3i    #Bins i w",nwbins));
  genStarLight->SetParameter(Form("RAP_MAX      =   %.1f    #max y",TMath::Max(TMath::Abs(yminConfig),TMath::Abs(ymaxConfig))));
  genStarLight->SetParameter(Form("RAP_N_BINS   =   %.0f    #Bins i y",10*(ymaxConfig-yminConfig)));
  genStarLight->SetParameter("CUT_PT       =    0    #Cut in pT? 0 = (no, 1 = yes)");
  genStarLight->SetParameter("PT_MIN       =    0    #Minimum pT in GeV");
  genStarLight->SetParameter("PT_MAX       =   10    #Maximum pT in GeV");
  genStarLight->SetParameter("CUT_ETA      =    0    #Cut in pseudorapidity? (0 = no, 1 = yes)");
  genStarLight->SetParameter("ETA_MIN      =   -5    #Minimum pseudorapidity");
  genStarLight->SetParameter("ETA_MAX      =    5    #Maximum pseudorapidity");
  genStarLight->SetParameter(Form("PROD_MODE    =    %i    #gg or gP switch (1 = 2-photon, 2 = coherent vector meson (narrow), 3 = coherent vector meson (wide), # 4 = incoherent vector meson, 5 = A+A DPMJet single, 6 = A+A DPMJet double, 7 = p+A DPMJet single, 8 = p+A Pythia single )",prod_mode));
  genStarLight->SetParameter(Form("PROD_PID     =   %6i    #Channel of interest (not relevant for photonuclear processes)",prod_pid));
  genStarLight->SetParameter(Form("RND_SEED     =    %i    #Random number seed", seedConfig));
  genStarLight->SetParameter("BREAKUP_MODE  =   5    #Controls the nuclear breakup");
  genStarLight->SetParameter("INTERFERENCE  =   0    #Interference (0 = off, 1 = on)");
  genStarLight->SetParameter("IF_STRENGTH   =   1.   #% of interfernce (0.0 - 0.1)");
  genStarLight->SetParameter("COHERENT      =   0    #Coherent=1,Incoherent=0");
  genStarLight->SetParameter("INCO_FACTOR   =   1.   #percentage of incoherence");
  genStarLight->SetParameter("INT_PT_MAX    =   0.24 #Maximum pt considered, when interference is turned on");
  genStarLight->SetParameter("INT_PT_N_BINS = 120    #Number of pt bins when interference is turned on");
  genStarLight->SetRapidityMotherRange(yminConfig,ymaxConfig);

  if (!genCocktail) return genStarLight;
  
  AliGenSLEvtGen *genEvtGen = new AliGenSLEvtGen();
  
  TString decayTable="PSI2S.MUMUPIPI.DEC"; // default
  if      (processConfig.Contains("Psi2sToMuPi")) decayTable="PSI2S.MUMUPIPI.DEC";
  else if (processConfig.Contains("Psi2sToElPi")) decayTable="PSI2S.EEPIPI.DEC";
  else if (processConfig.Contains("RhoPrime"))    decayTable="RHOPRIME.RHOPIPI.DEC";
  genEvtGen->SetUserDecayTable(gSystem->ExpandPathName(Form("$ALICE_ROOT/STARLIGHT/AliStarLight/DecayTables/%s",decayTable.Data())));
  
  if      (processConfig.Contains("Psi2s"))    genEvtGen->SetEvtGenPartNumber(100443);
  else if (processConfig.Contains("RhoPrime")) genEvtGen->SetEvtGenPartNumber(30113);

  genEvtGen->SetPolarization(1);           // Set polarization: transversal(1),longitudinal(-1),unpolarized(0)
  gSystem->Setenv("PYTHIA8DATA", gSystem->ExpandPathName("$ALICE_ROOT/PYTHIA8/pythia8/xmldoc"));
  
  genCocktail->AddGenerator(genStarLight,"StarLight",1.);
  genCocktail->AddGenerator(genEvtGen,"EvtGen",1.);
  return genCocktail;
}

/*** COCKTAIL ****************************************************/

AliGenerator * 
GeneratorCocktail(TString name)
{

  // configure projectile/target
  TString projN, targN;
  Int_t projA, projZ, targA, targZ;
  // pp
  if (systemConfig.EqualTo("p-p")) {
    projN = "p"; projA = 1; projZ = 1;
    targN = "p"; targA = 1; targZ = 1;
  }
  // PbPb
  else if (systemConfig.EqualTo("Pb-Pb")) {
    projN = "A"; projA = 208; projZ = 82;
    targN = "A"; targA = 208; targZ = 82;
  }
  // not implemented
  else {
    printf("Cocktail not implemented for %s system\n", systemConfig.Data());
    abort();
  }
  
  comment = comment.Append(Form(" | cocktail %s (%s-%s)", name.Data(), projN.Data(), targN.Data()));
  //
  AliGenCocktail *ctl = new AliGenCocktail();
  ctl->SetName(name.Data());
  ctl->SetProjectile(projN, projA, projZ);
  ctl->SetTarget(targN, targA, targZ);
  ctl->SetEnergyCMS(energyConfig);
  ctl->SetSeed(seedConfig);
  return ctl;
}

/*** INJECTOR ****************************************************/

AliGenerator * 
GeneratorInjector(Int_t ninj, Int_t pdg, Float_t ptmin, Float_t ptmax, Float_t ymin, Float_t ymax)
{
  comment = comment.Append(Form(" | injected (pdg=%d, %d particles)", pdg, ninj));
  //
  // Injected particles
  AliGenBox *box = new AliGenBox(ninj);
  box->SetPart(pdg);
  box->SetPtRange(ptmin, ptmax);
  box->SetYRange(ymin, ymax);
  box->SetPhiRange(0., 360.);
  return box;
}

/*** JPSI -> EE ****************************************************/

AliGenerator *
Generator_Jpsiee(const Char_t *params, Float_t jpsifrac, Float_t lowfrac, Float_t highfrac, Float_t bfrac)
{

  /*
    params
    "CDF pp 8.8"
    "PbPb 2.76"
  */
  
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

  comment = comment.Append(Form(" | J/psi -> ee (%s, %.1f/%.1f/%.1f/%.1f)", params, jpsifrac, lowfrac, highfrac, bfrac));
  
  //
  //Generating a cocktail
  AliGenCocktail *gener = new AliGenCocktail();
  gener->UsePerEventRates();
  //
  // J/psi
  AliGenParam *jpsi = new AliGenParam(1, AliGenMUONlib::kJpsi, params, "Jpsi");
  jpsi->SetPtRange(0., 6.);
  jpsi->SetYRange(-1.0, 1.0);
  jpsi->SetPhiRange(0., 360.);
  jpsi->SetForceDecay(kNoDecay);
  //
  // flat low pT
  AliGenParam *jpsiLowPt = new AliGenParam(1, AliGenMUONlib::kJpsi, "Flat", "Jpsi");  // flat pt distribution
  jpsiLowPt->SetPtRange(0., 0.5);
  jpsiLowPt->SetYRange(-1.0, 1.0);
  jpsiLowPt->SetPhiRange(0., 360.);
  jpsiLowPt->SetForceDecay(kNoDecay);
  //
  // flat high pT
  AliGenParam *jpsiHighPtFlat = new AliGenParam(1, AliGenMUONlib::kJpsi, "Flat", "Jpsi");  // 7 TeV
  jpsiHighPtFlat->SetPtRange(6., 25.);
  jpsiHighPtFlat->SetYRange(-1.0, 1.0);
  jpsiHighPtFlat->SetPhiRange(0., 360.);
  jpsiHighPtFlat->SetForceDecay(kNoDecay);
  //
  // Jpsi from B
  AliGenPythia *pythia = new AliGenPythia(-1);
  pythia->SetMomentumRange(0, 999999.);
  pythia->SetThetaRange(0., 180.);
  pythia->SetYRange(-2., 2.);
  pythia->SetPtRange(0, 1000.);
  pythia->SetProcess(kPyBeautyppMNRwmi);
  pythia->SetEnergyCMS(energyConfig);
  pythia->SetTune(kPythia6Tune_Perugia0);
  pythia->UseNewMultipleInteractionsScenario();
  pythia->SetCutOnChild(1);
  pythia->SetPdgCodeParticleforAcceptanceCut(443);
  pythia->SetChildYRange(-2, 2);
  pythia->SetChildPtRange(0, 10000.);
  pythia->SetForceDecay(kBJpsiUndecayed);
  pythia->SetStackFillOpt(AliGenPythia::kHeavyFlavor);  
  //
  // 
  AliGenEvtGen *gene = new AliGenEvtGen();
  gene->SetForceDecay(kBJpsiDiElectron);
  gene->SetParticleSwitchedOff(AliGenEvtGen::kCharmPart);
  if (jpsifrac > 0.) gener->AddGenerator(jpsi,           "JPsi",           jpsifrac);
  if (lowfrac  > 0.) gener->AddGenerator(jpsiLowPt,      "jpsiLowPt",      lowfrac);
  if (highfrac > 0.) gener->AddGenerator(jpsiHighPtFlat, "jpsiHighPtFlat", highfrac);
  if (bfrac    > 0.) gener->AddGenerator(pythia,         "Pythia",         bfrac);
  gener->AddGenerator(gene, "EvtGen", 1.);
  //
  return gener;
}

/*** NUCLEI EXOTICA ****************************************************/

AliGenerator *
Generator_Nuclex(UInt_t injbit, Float_t scale)
{

  comment = comment.Append(" | Nuclex ");
  
  //
  //Generating a cocktail
  AliGenCocktail *gener = new AliGenCocktail();

  // 2. Deuteron
  AliGenBox *box2 = new AliGenBox(TMath::Max(1, TMath::Nint(10. * scale)));
  box2->SetPart(1000010020);
  box2->SetPtRange(0., 10.);
  box2->SetPhiRange(0., 360.);
  box2->SetYRange(-1,1);

  // 3. Anti-Deuteron
  AliGenBox *box3 = new AliGenBox(TMath::Max(1, TMath::Nint(10. * scale)));
  box3->SetPart(-1000010020);
  box3->SetPtRange(0., 10.);
  box3->SetPhiRange(0., 360.);
  box3->SetYRange(-1,1);

  // 4. He-3
  AliGenBox *box4 = new AliGenBox(TMath::Max(1, TMath::Nint(10. * scale)));
  box4->SetPart(1000020030);
  box4->SetPtRange(0., 10.);
  box4->SetPhiRange(0., 360.);
  box4->SetYRange(-1,1);

  // 5. Anti-He-3
  AliGenBox *box5 = new AliGenBox(TMath::Max(1, TMath::Nint(10. * scale)));
  box5->SetPart(-1000020030);
  box5->SetPtRange(0., 10.);
  box5->SetPhiRange(0., 360.);
  box5->SetYRange(-1,1);

  // 6. Tritons
  AliGenBox *box6 = new AliGenBox(TMath::Max(1, TMath::Nint(10. * scale)));
  box6->SetPart(1000010030);
  box6->SetPtRange(0., 10.);
  box6->SetPhiRange(0., 360.);
  box6->SetYRange(-1,1);

  // 7. Anti-Tritons
  AliGenBox *box7 = new AliGenBox(TMath::Max(1, TMath::Nint(10. * scale)));
  box7->SetPart(-1000010030);
  box7->SetPtRange(0., 10.);
  box7->SetPhiRange(0., 360.);
  box7->SetYRange(-1,1);

  // 8. He-4
  AliGenBox *box8 = new AliGenBox(TMath::Max(1, TMath::Nint(10. * scale)));
  box8->SetPart(1000020040);
  box8->SetPtRange(0., 10.);
  box8->SetPhiRange(0., 360.);
  box8->SetYRange(-1,1);

  // 9. Anti-He-4
  AliGenBox *box9 = new AliGenBox(TMath::Max(1, TMath::Nint(10. * scale)));
  box9->SetPart(-1000020040);
  box9->SetPtRange(0., 10.);
  box9->SetPhiRange(0., 360.);
  box9->SetYRange(-1,1);

  // 10. Hypertriton
  AliGenBox *box10 = new AliGenBox(TMath::Max(1, TMath::Nint(40. * scale)));
  box10->SetPart(1010010030);
  box10->SetPtRange(0., 10.);
  box10->SetPhiRange(0., 360.);
  box10->SetYRange(-1,1);

  // 11. Anti-hypertriton
  AliGenBox *box11 = new AliGenBox(TMath::Max(1, TMath::Nint(40. * scale)));
  box11->SetPart(-1010010030);
  box11->SetPtRange(0., 10.);
  box11->SetPhiRange(0., 360.);
  box11->SetYRange(-1,1);

  // 12. Lambda-Neutron bound state
  AliGenBox *box12 = new AliGenBox(TMath::Max(1, TMath::Nint(20. * scale)));
  box12->SetPart(1010000020);
  box12->SetPtRange(0., 10.);
  box12->SetPhiRange(0., 360.);
  box12->SetYRange(-1,1);

  // 13. Anti-Lambda-Neutron bound state
  AliGenBox *box13 = new AliGenBox(TMath::Max(1, TMath::Nint(20. * scale)));
  box13->SetPart(-1010000020);
  box13->SetPtRange(0., 10.);
  box13->SetPhiRange(0., 360.);
  box13->SetYRange(-1,1);

  // 14. Lambda-Lambda bound state
  AliGenBox *box14 = new AliGenBox(TMath::Max(1, TMath::Nint(20. * scale)));
  box14->SetPart(1020000020);
  box14->SetPtRange(0., 10.);
  box14->SetPhiRange(0., 360.);
  box14->SetYRange(-1,1);

  // 15. Anti-Lambda-Lambda bound state
  AliGenBox *box15 = new AliGenBox(TMath::Max(1, TMath::Nint(20. * scale)));
  box15->SetPart(-1020000020);
  box15->SetPtRange(0., 10.);
  box15->SetPhiRange(0., 360.);
  box15->SetYRange(-1,1);

  // 16. Omega-Proton bound state
  AliGenBox *box16 = new AliGenBox(TMath::Max(1, TMath::Nint(20. * scale)));
  box16->SetPart(1030000020);
  box16->SetPtRange(0., 10.);
  box16->SetPhiRange(0., 360.);
  box16->SetYRange(-1,1);

  // 17. Anti-Omega-Proton bound state
  AliGenBox *box17 = new AliGenBox(TMath::Max(1, TMath::Nint(20. * scale)));
  box17->SetPart(-1030000020);
  box17->SetPtRange(0., 10.);
  box17->SetPhiRange(0., 360.);
  box17->SetYRange(-1,1);

  // 18. Omega-Neutron bound state
  AliGenBox *box18 = new AliGenBox(TMath::Max(1, TMath::Nint(20. * scale)));
  box18->SetPart(1030010020);
  box18->SetPtRange(0., 10.);
  box18->SetPhiRange(0., 360.);
  box18->SetYRange(-1,1);

  // 19. Anti-Omega-Neutron bound state
  AliGenBox *box19 = new AliGenBox(TMath::Max(1, TMath::Nint(20. * scale)));
  box19->SetPart(-1030010020);
  box19->SetPtRange(0., 10.);
  box19->SetPhiRange(0., 360.);
  box19->SetYRange(-1,1);

  // 20. Omega-Omega bound state
  AliGenBox *box20 = new AliGenBox(TMath::Max(1, TMath::Nint(20. * scale)));
  box20->SetPart(1060020020);
  box20->SetPtRange(0., 10.);
  box20->SetPhiRange(0., 360.);
  box20->SetYRange(-1,1);

  // 21. Anti-Omega-Omega bound state
  AliGenBox *box21 = new AliGenBox(TMath::Max(1, TMath::Nint(20. * scale)));
  box21->SetPart(-1060020020);
  box21->SetPtRange(0., 10.);
  box21->SetPhiRange(0., 360.);
  box21->SetYRange(-1,1);

  // 22. Lambda(1405)-proton bound state
  AliGenBox *box22 = new AliGenBox(TMath::Max(1, TMath::Nint(20. * scale)));
  box22->SetPart(1010010021);
  box22->SetPtRange(0., 10.);
  box22->SetPhiRange(0., 360.);
  box22->SetYRange(-1,1);

  // 23. Anti-Lambda(1405)-proton bound state
  AliGenBox *box23 = new AliGenBox(TMath::Max(1, TMath::Nint(20. * scale)));
  box23->SetPart(-1010010021);
  box23->SetPtRange(0., 10.);
  box23->SetPhiRange(0., 360.);
  box23->SetYRange(-1,1);

  // 24. Lambda(1405)-Lambda(1405) bound state
  AliGenBox *box24 = new AliGenBox(TMath::Max(1, TMath::Nint(20. * scale)));
  box24->SetPart(1020000021);
  box24->SetPtRange(0., 10.);
  box24->SetPhiRange(0., 360.);
  box24->SetYRange(-1,1);

  // 25. Lambda(1405)-Lambda(1405) bound state
  AliGenBox *box25 = new AliGenBox(TMath::Max(1, TMath::Nint(20. * scale)));
  box25->SetPart(-1020000021);
  box25->SetPtRange(0., 10.);
  box25->SetPhiRange(0., 360.);
  box25->SetYRange(-1,1);

  // 26. Xi0-proton bound state
  AliGenBox *box26 = new AliGenBox(TMath::Max(1, TMath::Nint(20. * scale)));
  box26->SetPart(1020010020);
  box26->SetPtRange(0., 10.);
  box26->SetPhiRange(0., 360.);
  box26->SetYRange(-1,1);

  // 27. Anti-Xi0-proton bound state
  AliGenBox *box27 = new AliGenBox(TMath::Max(1, TMath::Nint(20. * scale)));
  box27->SetPart(-1020010020);
  box27->SetPtRange(0., 10.);
  box27->SetPhiRange(0., 360.);
  box27->SetYRange(-1,1);
  
  if (injbit && 1 << 2)  gener->AddGenerator(box2,"fbox2",1);
  if (injbit && 1 << 3)  gener->AddGenerator(box3,"fbox3",1);
  if (injbit && 1 << 4)  gener->AddGenerator(box4,"fbox4",1);
  if (injbit && 1 << 5)  gener->AddGenerator(box5,"fbox5",1);
  if (injbit && 1 << 6)  gener->AddGenerator(box6,"fbox6",1);
  if (injbit && 1 << 7)  gener->AddGenerator(box7,"fbox7",1);
  if (injbit && 1 << 8)  gener->AddGenerator(box8,"fbox8",1);
  if (injbit && 1 << 9)  gener->AddGenerator(box9,"fbox9",1);
  if (injbit && 1 << 10) gener->AddGenerator(box10,"fbox10",1);
  if (injbit && 1 << 11) gener->AddGenerator(box11,"fbox11",1);
  if (injbit && 1 << 12) gener->AddGenerator(box12,"fbox12",1);
  if (injbit && 1 << 13) gener->AddGenerator(box13,"fbox13",1);
  if (injbit && 1 << 14) gener->AddGenerator(box14,"fbox14",1);
  if (injbit && 1 << 15) gener->AddGenerator(box15,"fbox15",1);
  if (injbit && 1 << 16) gener->AddGenerator(box16,"fbox16",1);
  if (injbit && 1 << 17) gener->AddGenerator(box17,"fbox17",1);
  if (injbit && 1 << 18) gener->AddGenerator(box18,"fbox18",1);
  if (injbit && 1 << 19) gener->AddGenerator(box19,"fbox19",1);
  if (injbit && 1 << 20) gener->AddGenerator(box20,"fbox20",1);
  if (injbit && 1 << 21) gener->AddGenerator(box21,"fbox21",1);
  if (injbit && 1 << 22) gener->AddGenerator(box22,"fbox22",1);
  if (injbit && 1 << 23) gener->AddGenerator(box23,"fbox23",1);
  if (injbit && 1 << 24) gener->AddGenerator(box24,"fbox24",1);
  if (injbit && 1 << 25) gener->AddGenerator(box25,"fbox25",1);
  if (injbit && 1 << 26) gener->AddGenerator(box26,"fbox26",1);
  if (injbit && 1 << 27) gener->AddGenerator(box27,"fbox27",1);

  return gener;
}
