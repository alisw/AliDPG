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
AliGenerator *GeneratorPhojet();
AliGenerator *GeneratorEPOSLHC();
AliGenerator *GeneratorHijing();
AliGenerator *Generator_Jpsiee(const Char_t *params, Float_t jpsifrac, Float_t lowfrac, Float_t highfrac, Float_t bfrac);
AliGenerator *Generator_Nuclex(UInt_t injbit = 0xFFFFFFFF, Float_t scale = 1.);

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
GeneratorCocktail(TString name)
{

  // configure projectile/target
  TString projN, targN;
  Int_t projA, projZ, targA, targZ;
  TString system = gSystem->Getenv("CONFIG_SYSTEM");
  // pp
  if (system.EqualTo("p-p")) {
    projN = "p"; projA = 1; projZ = 1;
    targN = "p"; targA = 1; targZ = 1;
  }
  // PbPb
  else if (system.EqualTo("Pb-Pb")) {
    projN = "A"; projA = 208; projZ = 82;
    targN = "A"; targA = 208; targZ = 82;
  }
  // not implemented
  else {
    printf("Cocktail not implemented for %s system\n", system.Data());
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

  return gener;
}
