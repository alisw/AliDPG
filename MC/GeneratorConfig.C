/*
 * AliDPG - ALICE Experiment Data Preparation Group
 * Generator configuration script
 *
 */

enum EGenerator_t {
  kGeneratorDefault,
  // Pythia6
  kGeneratorPythia6, kGeneratorPythia6_Perugia2011,
  // Pythia8
  kGeneratorPythia8, kGeneratorPythia8_Monash2013,
  // Pythia8 jets
  kGeneratorPythia8Jets, kGeneratorPythia8Jets_Monash2013, 
  // Phojet
  kGeneratorPhojet, kGeneratorDpmjet,
  // EPOS-LHC
  kGeneratorEPOSLHC, kGeneratorEPOSLHC_pp, kGeneratorEPOSLHC_PbPb,
  // Hijing
  kGeneratorHijing,
  // Starlight
  kGeneratorStarlight,
  // AMPT
  kGeneratorAMPT,
  //
  kGeneratorCustom,
  //
  kGeneratorPWG,
  kNGenerators
};

const Char_t *GeneratorName[kNGenerators] = {
  "Default",
  // Pythia6
  "Pythia6", "Pythia6_Perugia2011",
  // Pythia8
  "Pythia8", "Pythia8_Monash2013",
  // Pythia8 jets
  "Pythia8Jets", "Pythia8Jets_Monash2013",
  // Phijet / DPMjet
  "Phojet", "Dpmjet",
  // EPOS-LHC
  "EPOSLHC", "EPOSLHC_pp", "EPOSLHC_PbPb",
  // Hijing
  "Hijing",
  // Starlight
  "Starlight",
  // AMPT
  "AMPT",
  //
  "Custom",
  //
  "PWG"
};

enum ETrigger_t {
  kTriggerDefault,
  kTriggerPP,
  kTriggerPbPb,
  kTriggerCustom,
  kNTriggers
};

const Char_t *TriggerName[kNTriggers] = {
  "ocdb",
  "p-p",
  "Pb-Pb",
  "Custom.cfg"
};

enum EPythia6Tune_t {
  kPythia6Tune_Perugia0    = 320,
  kPythia6Tune_Perugia2011 = 350
};

enum EPythia8Tune_t {
  kPythia8Tune_Monash2013  = 14
};

enum EPythia6HeavyProcess_t {
  kPythia6HeavyProcess_Charm,
  kPythia6HeavyProcess_Beauty
};

enum EPythia6HeavyDecay_t {
  kPythia6HeavyDecay_Hadrons,
  kPythia6HeavyDecay_HadronsWithV0,
  kPythia6HeavyDecay_Electron
};

/*****************************************************************/
/*****************************************************************/
/*****************************************************************/

// functions
AliGenerator *GeneratorCocktail(TString name);
AliGenerator *GeneratorInjector(Int_t ninj, Int_t pdg, Float_t ptmin, Float_t ptmax, Float_t ymin, Float_t ymax, Float_t phimin = 0., Float_t phimax = 360.); 
AliGenerator *GeneratorPythia6(Int_t tune = 0, Int_t pdgtrig = 0, Float_t etatrig = 1.2);
AliGenerator *GeneratorPythia6Heavy(Int_t process, Int_t decay, Int_t tune = 0, Bool_t HFonly = kTRUE);
AliGenerator *GeneratorPythia8(Int_t tune = 0, Int_t pdgtrig = 0, Float_t etatrig = 1.2);
AliGenerator *GeneratorPythia8Jets(Int_t tune = 0);
AliGenerator *GeneratorPhojet();
AliGenerator *GeneratorEPOSLHC();
AliGenerator *GeneratorHijing();
AliGenerator *Generator_Jpsiee(const Char_t *params, Float_t jpsifrac, Float_t lowfrac, Float_t highfrac, Float_t bfrac);
AliGenerator *Generator_Nuclex(UInt_t injbit, Bool_t antiparticle, Int_t ninj);
AliGenerator *GeneratorStarlight();
AliGenerator *GeneratorAMPT();

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
        
    // Pythia8Jets (Monash2013)
  case kGeneratorPythia8Jets: 
  case kGeneratorPythia8Jets_Monash2013:
    gen = GeneratorPythia8Jets(kPythia8Tune_Monash2013);
    break;
    
    // Phojet / DPMjet
  case kGeneratorPhojet:
  case kGeneratorDpmjet:
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

    // AMPT
  case kGeneratorAMPT:
    gen = GeneratorAMPT();
    break;

    // Custom
  case kGeneratorCustom:
    if ((gROOT->LoadMacro("GeneratorCustom.C")) != 0) {
      printf("ERROR: cannot find GeneratorCustom.C\n");
      abort();
      return;
    }
    gen = GeneratorCustom();
    break;

    // PWG
  case kGeneratorPWG:
    TString genstr = gSystem->Getenv("CONFIG_GENERATOR");
    TObjArray *oa = genstr.Tokenize(":");
    TObjString *pwg = oa->At(0);
    TObjString *pwggen = oa->At(1);
    TObjString *pwgopt = oa->At(2);
    if (!pwg || !pwggen) {
      printf("ERROR: problem parsing CONFIG_GENERATOR: %s \n", genstr.Data());
      abort();
      return;
    }
    // load PWG custom generator macro
    TString pwgmacro = "$ALIDPG_ROOT/MC/CustomGenerators/";
    pwgmacro += pwg->GetString();
    pwgmacro += "/";
    pwgmacro += pwggen->GetString();
    pwgmacro += ".C";
    if ((gROOT->LoadMacro(pwgmacro.Data())) != 0) {
      printf("ERROR: cannot find %s \n", pwgmacro.Data());
      abort();
      return;
    }
    if (pwgopt)
      gen = GeneratorCustom(pwgopt->String());
    else
      gen = GeneratorCustom();
    break;

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
    //
    if (periodName.EqualTo("LHC16f")) sigmaxy =  60.e-4; 
    if (periodName.EqualTo("LHC16g")) sigmaxy =  60.e-4; 
    if (periodName.EqualTo("LHC16i")) sigmaxy =  60.e-4; 
    if (periodName.EqualTo("LHC16k")) sigmaxy =  45.e-4; 
    if (periodName.EqualTo("LHC16l")) sigmaxy =  45.e-4; 
    if (periodName.EqualTo("LHC16o")) sigmaxy =  55.e-4; 
    if (periodName.EqualTo("LHC16p")) sigmaxy =  50.e-4; 
    if (periodName.EqualTo("LHC16q")) sigmaxy =  35.e-4; 
    if (periodName.EqualTo("LHC16r")) sigmaxy =  25.e-4;
    if (periodName.EqualTo("LHC16s")) sigmaxy =  25.e-4;
    if (periodName.EqualTo("LHC16t")) sigmaxy =  35.e-4;
  }
  printf(">>>>> Diamond sigma-xy: %f \n", sigmaxy);

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
GeneratorPythia6(Int_t tune, Int_t pdgtrig, Float_t etatrig)
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
  if (pdgtrig != 0) {
    comment = comment.Append(Form(" | %s enhanced", TDatabasePDG::Instance()->GetParticle(pdgtrig)->GetName()));
    pythia->SetTriggerParticle(pdgtrig, etatrig);
  }
  //
  return pythia;
}

/*** PYTHIA 6 ****************************************************/

AliGenerator *
GeneratorPythia6Heavy(Int_t process, Int_t decay, Int_t tune, Bool_t HFonly)
{
  //
  //
  comment = comment.Append(Form(" | Pythia6 heavy (%d, %d)", process, decay));
  //
  // set external decayer
  if (decay == kPythia6HeavyDecay_Electron) {
    TVirtualMCDecayer* decayer = new AliDecayerPythia();
    decayer->SetForceDecay(kAll);
    decayer->Init();
    gMC->SetExternalDecayer(decayer);
  }
  //
  // Pythia
  AliGenPythia *pythia = GeneratorPythia6(tune);
  //
  // heavy process
  switch (process) {
  case kPythia6HeavyProcess_Charm:
    pythia->SetProcess(kPyCharmppMNRwmi);
    break;
  case kPythia6HeavyProcess_Beauty:
    pythia->SetProcess(kPyBeautyppMNRwmi);
    break;
  }
  //
  // heavy decay
  switch (decay) {
  case kPythia6HeavyDecay_Hadrons:
    pythia->SetHeavyQuarkYRange(-1.5, 1.5);
    pythia->SetForceDecay(kHadronicDWithout4Bodies);
    break;
  case kPythia6HeavyDecay_HadronsWithV0:
    pythia->SetHeavyQuarkYRange(-1.5, 1.5);
    pythia->SetForceDecay(kHadronicDWithout4BodiesWithV0);
    break;
  case kPythia6HeavyDecay_Electron:
    pythia->SetCutOnChild(1);
    pythia->SetPdgCodeParticleforAcceptanceCut(11);
    pythia->SetChildYRange(-1.2, 1.2);
    pythia->SetChildPtRange(0, 10000.);
    break;
  }
  //  
  if (HFonly)
    pythia->SetStackFillOpt(AliGenPythia::kHeavyFlavor);
  return pythia;
}


/*** PYTHIA 8 ****************************************************/

AliGenerator *
GeneratorPythia8(Int_t tune, Int_t pdgtrig, Float_t etatrig)
{
  //
  // Libraries
  gSystem->Load("libpythia8.so");
  gSystem->Load("libAliPythia8.so");
  //
  // Environment settings
  gSystem->Setenv("PYTHIA8DATA", gSystem->ExpandPathName("$ALICE_ROOT/PYTHIA8/pythia8/xmldoc"));
  gSystem->Setenv("LHAPDF",      gSystem->ExpandPathName("$ALICE_ROOT/LHAPDF"));
  gSystem->Setenv("LHAPATH",     gSystem->ExpandPathName("$ALICE_ROOT/LHAPDF/PDFsets"));
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
  (AliPythia8::Instance())->ReadString("111:mayDecay = on");
  //
  // Tune
  if (tune > 0) {
    comment = comment.Append(Form(" | tune %d", tune));
    pythia->SetTune(tune);
  }
  //
  // Trigger particles
  if (pdgtrig != 0) {
    comment = comment.Append(Form(" | %s enhanced", TDatabasePDG::Instance()->GetParticle(pdgtrig)->GetName()));
    pythia->SetTriggerParticle(pdgtrig, etatrig);
  }
  //
  return pythia;
}

/*** PYTHIA 8 JETS ***********************************************/

AliGenerator *
GeneratorPythia8Jets(Int_t tune)
{
  //
  //
  comment = comment.Append(Form(" | Pythia8 jets (%.1f, %.1f, %d, %.1f)", pthardminConfig, pthardmaxConfig, quenchingConfig, qhatConfig));
  //
  // Pythia
  AliGenPythiaPlus *pythia = GeneratorPythia8(tune);
  //
  // jets settings
  pythia->SetProcess(kPyJets);
  pythia->SetJetEtaRange(-1.5, 1.5); // Final state kinematic cuts
  pythia->SetJetPhiRange(0., 360.);
  pythia->SetJetEtRange(0., 1000.);
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
  //  gSystem->Load("libDPMJET");
  //  gSystem->Load("libTDPMjet");
  //
  comment = comment.Append(" | Phojet/DPMjet low-pt");
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

  // projectile-target
  if (systemConfig.EqualTo("Pb-Pb")) {
    dpmjet->SetProjectile("A", 208, 82);
    dpmjet->SetTarget    ("A", 208, 82);
    dpmjet->SetImpactParameterRange(bminConfig, bmaxConfig);
  }
  else if (systemConfig.EqualTo("p-Pb")) {
    dpmjet->SetProjectile("P", 1, 1);
    dpmjet->SetTarget    ("A", 208, 82);
    dpmjet->SetProjectileBeamEnergy(0.5 * energyConfig * TMath::Sqrt(208./82.));
    dpmjet->SetImpactParameterRange(bminConfig, bmaxConfig);
    dpmjet->SetFragmentProd(kFALSE);
    if (processConfig.Contains("fragments")) 
      dpmjet->SetFragmentProd(kTRUE);
  }
  else if (systemConfig.EqualTo("Pb-p")) {
    dpmjet->SetProjectile("A", 208, 82);
    dpmjet->SetTarget    ("P", 1, 1);
    dpmjet->SetProjectileBeamEnergy(0.5 * energyConfig * TMath::Sqrt(82./208.));
    dpmjet->SetImpactParameterRange(bminConfig, bmaxConfig);
    dpmjet->SetFragmentProd(kFALSE);
    if (processConfig.Contains("fragments")) 
      dpmjet->SetFragmentProd(kTRUE);
  }

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
  // pPb
  else if (systemConfig.EqualTo("p-Pb")) {
    projectileId = 2212;
    targetId = 82*10000 + 208*10;
    projectileEnergy = 0.5 * energyConfig * TMath::Sqrt(208./82.);
    targetEnergy = 0.5 * energyConfig * TMath::Sqrt(82./208.);
  }
  // Pbp
  else if (systemConfig.EqualTo("Pb-p")) {
    projectileId = 82*10000 + 208*10;
    targetId = 2212;
    projectileEnergy = 0.5 * energyConfig * TMath::Sqrt(82./208.);
    targetEnergy = 0.5 * energyConfig * TMath::Sqrt(208./82.);
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
  // projectile-target
  if (systemConfig.EqualTo("Pb-Pb")) {
    gener->SetProjectile("A", 208, 82);
    gener->SetTarget    ("A", 208, 82);
    gener->SetSpectators(0);
  }
  else if (systemConfig.EqualTo("p-Pb")) {
    gener->SetProjectile("P", 1, 1);
    gener->SetTarget    ("A", 208, 82);
    gener->SetBoostLHC(1);
    gener->SetSpectators(1);
  }
  else if (systemConfig.EqualTo("Pb-p")) {
    gener->SetProjectile("A", 208, 82);
    gener->SetTarget    ("P", 1, 1);
    gener->SetBoostLHC(1);
    gener->SetSpectators(1);
  }
  // tell hijing to keep the full parent child chain
  gener->KeepFullEvent();
  // enable jet quenching
  gener->SetJetQuenching(0);
  // enable shadowing
  gener->SetShadowing(1);
  // neutral pion and heavy particle decays switched off
  gener->SetDecaysOff(1);
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
  // _STARLIGHT_PROCESSES_BEGIN_ this comment is used by AliRoot/STARLIGHT/test/testsl.C
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
  // _STARLIGHT_PROCESSES_END_ this comment is used by AliRoot/STARLIGHT/test/testsl.C

  comment.Append(Form(" | Starlight %s (%s)", processConfig.Data(), systemConfig.Data()));

  // restict ymin,ymax to [-8,8] as with the default ymin,ymax set in Config.C
  // there is a floating point exception in starlight
  yminConfig = TMath::Max(yminConfig, -8.0f);
  ymaxConfig = TMath::Min(ymaxConfig,  8.0f);

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
  if      (processConfig.Contains("TwoGamma")) prod_mode = 1;
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

  // by default use automatic wmin, wmax
  Float_t wmin = -1;
  Float_t wmax = -1;
  Int_t nwbins = 20;
  if (processConfig.Contains("TwoGammaToMu") ||
      processConfig.Contains("TwoGammaToEl")) {
    wmax = 15;
    if      (processConfig.Contains("Low"))    wmin = 0.4;
    else if (processConfig.Contains("Medium")) wmin = 2.0;
    else if (processConfig.Contains("High"))   wmin = 4.0;

    nwbins = 20*(wmax-wmin);
  }

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
  genStarLight->SetParameter(Form("W_MAX        =   %.1f    #Max value of w",wmax));
  genStarLight->SetParameter(Form("W_MIN        =   %.1f    #Min value of w",wmin));
  genStarLight->SetParameter(Form("W_N_BINS     =    %3i    #Bins i w",nwbins));
  genStarLight->SetParameter(Form("RAP_MAX      =   %.2f    #max y",TMath::Max(TMath::Abs(yminConfig),TMath::Abs(ymaxConfig))));
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
  genStarLight->SetParameter("INT_PT_MAX    =   0.24 #Maximum pt considered, when interference is turned on");
  genStarLight->SetParameter("INT_PT_N_BINS = 120    #Number of pt bins when interference is turned on");
  genStarLight->SetParameter("XSEC_METHOD   = 1      # Set to 0 to use old method for calculating gamma-gamma luminosity"); //CM
  genStarLight->SetParameter("BSLOPE_DEFINITION = 0");   // using default slope
  genStarLight->SetParameter("BSLOPE_VALUE      = 4.0"); // default slope value

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

/*** AMPT ********************************************************/

AliGenerator *
GeneratorAMPT() {

  //
  // load libs
  gSystem->Load("libAMPT");  
  gSystem->Load("libTAmpt");
  //
  comment.Append(Form(" | AMPT %s (%s)", processConfig.Data(), systemConfig.Data()));
  //
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
  //
  // configure AMPT processes
  Int_t isoft = 1;  //1: defaul - 4: string melting
  Int_t ntmax = 150;        // NTMAX: number of timesteps (D=150)
  if (processConfig.Contains("StringMelting")) isoft = 4;
  if (processConfig.Contains("NoART")) ntmax = 3;

  //
  // AMPT
  AliGenAmpt *genHi = new AliGenAmpt(-1);
  AliDecayer *decayer = new AliDecayerPythia();
  genHi->SetForceDecay(kHadronicD);
  genHi->SetDecayer(decayer);
  genHi->SetEnergyCMS(energyConfig);
  genHi->SetReferenceFrame("CMS");
  genHi->SetProjectile(projN.Data(), projA, projZ);
  genHi->SetTarget(targN.Data(), targA, targZ);
  genHi->SetIsoft(isoft);  //1: defaul - 4: string melting
  genHi->SetStringFrag(0.5, 0.9); //Lund string frgmentation parameters
  genHi->SetPtHardMin(3);
  genHi->SetImpactParameterRange(bminConfig, bmaxConfig);
    
  // Xmu = 3.2 fm^-1 and as = 0.33 ==> sigma_{partonic} = 1.5mb
  // Ntmax = 150
  // v_2{2} = 0.102105 +/- 0.000266894
  // v_2{4} = 0.0829477 +/- 0.00106158
    
  genHi->SetNtMax(ntmax);        // NTMAX: number of timesteps (D=150)
  genHi->SetXmu(3.2264);        // parton screening mass in fm^(-1) (D=3.2264d0)
    
  genHi->SetJetQuenching(0);  // enable jet quenching
  genHi->SetShadowing(1);     // enable shadowing
  genHi->SetDecaysOff(1);     // neutral pion and heavy particle decays switched off
  genHi->SetSpectators(0);    // track spectators
  //Boost into LHC lab frame
  genHi->SetBoostLHC(1);
  //  genHi->Init();
  genHi->SetRandomReactionPlane(kTRUE);
 
  return genHi;

  
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
  else if (systemConfig.EqualTo("Pb-p")) {
    projN = "A"; projA = 208; projZ = 82;
    targN = "p"; targA = 1; targZ = 1;
  }
  else if (systemConfig.EqualTo("p-Pb")) {
    projN = "p"; projA = 1; projZ = 1;
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
GeneratorInjector(Int_t ninj, Int_t pdg, Float_t ptmin, Float_t ptmax, Float_t ymin, Float_t ymax, Float_t phimin, Float_t phimax)
{
  comment = comment.Append(Form(" | injected (pdg=%d, %d particles)", pdg, ninj));
  //
  // Injected particles
  AliGenBox *box = new AliGenBox(ninj);
  box->SetPart(pdg);
  box->SetPtRange(ptmin, ptmax);
  box->SetYRange(ymin, ymax);
  box->SetPhiRange(phimin, phimax);
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
Generator_Nuclex(UInt_t injbit, Bool_t antiparticle, Int_t ninj)
{

  comment = comment.Append(Form(" | Nuclex (0x%x) ", injbit));
  
  //
  //Generating a cocktail
  AliGenCocktail *gener = new AliGenCocktail();

  Int_t pdgcodes[19] = {
    1000010020,
    1000020030,
    1000010030,
    1000020040,
    1010010030,
    1010000020,
    1020000020,
    1030000020,
    1030010020,
    1060020020,
    1010010021,
    1020000021,
    1020010020,
    1010000030,
    1010010040,
    1010020040,
    9322134,
    9322136,
    900010020 
  };

  const Char_t *names[19] = {
    "Deuteron", // 0x1
    "Helium-3", // 0x2
    "Triton",   // 0x4
    "Helium-4", // 0x8
    "Hyper-Triton", // 0x10
    "Lambda-Neutron", // 0x20
    "Lambda-Lambda", // 0x40
    "Omega-Proton", // 0x80
    "Omega-Neutron", // 0x100
    "Omega-Omega", // 0x200
    "Lambda(1405)-proton", // 0x400
    "Lambda(1405)-Lambda(1405)", // 0x800
    "Xi0-proton", // 0x1000
    "Lambda-Neutron-Neutron", // 0x2000
    "Hyper-Hydrogen-4", // 0x4000
    "Hyper-Helium-4", // 0x8000
    "Ps(2100)", // 0x10000
    "Ps(2500)", // 0x20000
    "d*(2380)"  //0x40000
  };

  for (Int_t ipart = 0; ipart < 19; ipart++) {
    if (injbit & 1 << ipart) {
      AliGenBox *box = new AliGenBox(ninj);
      Int_t pdg = pdgcodes[ipart];
      if (antiparticle) pdg = -pdg;
      box->SetPart(pdg);
      box->SetPtRange(0., 10.);
      box->SetPhiRange(0., 360.);
      box->SetYRange(-1,1);
      gener->AddGenerator(box, names[ipart], 1);
      printf(">>>>> adding %d %s%s (%d) to the cocktail \n", ninj, antiparticle ? "anti-" : "", names[ipart], pdg);
    }
  }

  return gener;
}
