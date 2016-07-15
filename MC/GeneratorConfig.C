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
AliGenerator *GeneratorCocktail(TString projN, Int_t projA, Int_t projZ, TString targN, Int_t targA, Int_t targZ);
AliGenerator *GeneratorInjector(Int_t ninj, Int_t pdg, Float_t ptmin, Float_t ptmax, Float_t ymin, Float_t ymax); 
AliGenerator *GeneratorPythia6(Int_t tune = 0, Int_t ntrig = 0, Int_t *trig = NULL);
AliGenerator *GeneratorPythia8(Int_t tune = 0, Int_t ntrig = 0, Int_t *trig = NULL);
AliGenerator *GeneratorPhojet();
AliGenerator *GeneratorEPOSLHC();
AliGenerator *GeneratorHijing();
AliGenerator *Generator_Jpsiee(const Char_t *params, Float_t jpsifrac, Float_t lowfrac, Float_t highfrac, Float_t bfrac);

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
    AliGenCocktail *ctl = GeneratorCocktail("p", 1, 1, "p", 1, 1);
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

    // Hijing - Rsn002
  case kGeneratorHijing_Rsn002a:
  case kGeneratorHijing_Rsn002b:
  case kGeneratorHijing_Rsn002c:
    Int_t ninjlist[3] = {25, 7, 3};
    Int_t ninj = ninjlist[tag - kGeneratorHijing_Rsn002a];
    AliGenCocktail *ctl  = GeneratorCocktail("A", 208, 82, "A", 208, 82);
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
    AliGenCocktail *ctl   = GeneratorCocktail("A", 208, 82, "A", 208, 82);
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
    
    // Custom
  case kGeneratorCustom:
    if ((gROOT->LoadMacro("GeneratorCustom.C")) != 0) {
      printf("ERROR: cannot find GeneratorCustom.C\n");
      abort();
      return;
    }
    gen = GeneratorCustom();

  }
  
  //  gener->SetOrigin(0.075, 0.522, -0.884); // R+HACK                        
  //  gener->SetSigma(65e-4, 65e-4, 5.); // R+HACK                                           
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

  TString system = gSystem->Getenv("CONFIG_SYSTEM");
  comment = comment.Append(Form(" | EPOS-LHC (%s)", system.Data()));
  //
  // configure projectile/target
  Int_t projectileId = 0; // PDG or Z*10000+A*10
  Float_t projectileEnergy = 0.; // momentum/(GeV/c)
  Int_t targetId = 0;
  Float_t targetEnergy = 0;
  // pp
  if (system.EqualTo("p-p")) {
    projectileId = 2212;
    targetId = projectileId;
    projectileEnergy = energyConfig / 2.;
    targetEnergy = energyConfig / 2.;
  }
  // PbPb
  else if (system.EqualTo("Pb-Pb")) {
    projectileId = 82*10000 + 208*10;
    targetId = projectileId;
    projectileEnergy = energyConfig / 2.;
    targetEnergy = energyConfig / 2.;
  }
  // not implemented
  else {
    printf("EPOSLHC not implemented for %s system\n", system.Data());
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
  gener->SetName(Form("EPOSLHC_%s", system.Data()));
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

/*** COCKTAIL ****************************************************/

AliGenerator * 
GeneratorCocktail(TString projN, Int_t projA, Int_t projZ,
		  TString targN, Int_t targA, Int_t targZ)
{
  comment = comment.Append(Form(" | cocktail (%s-%s)", projN.Data(), targN.Data()));
  //
  AliGenCocktail *ctl = new AliGenCocktail();
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

