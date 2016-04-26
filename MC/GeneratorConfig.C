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
  kGeneratorPhojet,
  kGeneratorEPOSLHC_pp,
  kGeneratorHijing,
  kGeneratorCustom,
  kNGenerators
};

const Char_t *GeneratorName[kNGenerators] = {
  "Default",
  "Pythia6",
  "Pythia6_Perugia2011",
  "Pythia8",
  "Pythia8_Monash2013",
  "Phojet",
  "EPOSLHC_pp",
  "Hijing",
  "Custom"
};

/*****************************************************************/
/*****************************************************************/
/*****************************************************************/

// functions
AliGenerator *GeneratorPythia6(Int_t tune = 0, Int_t ntrig = 0, Int_t *trig = NULL);
AliGenerator *GeneratorPythia8(Int_t tune = 0, Int_t ntrig = 0, Int_t *trig = NULL);
AliGenerator *GeneratorPhojet();
AliGenerator *GeneratorEPOSLHC(TString system);
AliGenerator *GeneratorHijing();

/*****************************************************************/

// global variables
static TString comment;

/*****************************************************************/

GeneratorConfig(Int_t tag, Int_t run)
{

  AliGenerator *gen = NULL;
  
  switch(tag) {

    // Default
  case kGeneratorDefault:
    abort();
    return;
    
    // Pythia6 (Perugia2011)
  case kGeneratorPythia6:
  case kGeneratorPythia6_Perugia2011:
    gen = GeneratorPythia6(350);
    break;

    // Pythia8 (Monash2013)
  case kGeneratorPythia8:
  case kGeneratorPythia8_Monash2013:
    gen = GeneratorPythia8(14);
    break;

    // Phojet
  case kGeneratorPhojet:
    gen = GeneratorPhojet();
    break;

    // EPOSLHC (pp)
  case kGeneratorEPOSLHC_pp:
    gen = GeneratorEPOSLHC("pp");
    break;

    // Hijing
  case kGeneratorHijing:
    gen = GeneratorHijing();
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
  
}

/*** PYTHIA 6 ****************************************************/

AliGenerator *
GeneratorPythia6(Int_t tune, Int_t ntrig, Int_t *trig)
{
  comment = comment.Append(" pp: Pythia6 low-pt");
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
  //
  //
  comment = comment.Append(" pp: Pythia8 low-pt");
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
  comment = comment.Append(" pp: Phojet low-pt");
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
GeneratorEPOSLHC(TString system)
{
  
  if (system.EqualTo("pp")) {
    comment = comment.Append(" pp: EPOS-LHC");
    Float_t beamEnergy = energyConfig / 2.;
    TString fifoname = Form("/tmp/crmceventfifo%d", gRandom->Integer(kMaxInt));
    gROOT->ProcessLine(Form(".! rm -rf %s", fifoname.Data()));
    gROOT->ProcessLine(Form(".! mkfifo %s", fifoname.Data()));
    gROOT->ProcessLine(Form(".! sh $ALIDPG_MC/EXTRA/gen_eposlhc_pp.sh %s %d %f %f &",
			    fifoname.Data(), neventsConfig, beamEnergy, beamEnergy));
  }
  else {
    printf("EPOSLHC not implemented for %s system\n", system.Data());
    abort();
  }
  
  AliGenReaderHepMC *reader = new AliGenReaderHepMC();
  reader->SetFileName("crmceventfifo");
  AliGenExtFile *gener = new AliGenExtFile(-1);
  gener->SetReader(reader);
  
  return gener;
}

/*** EPOSLHC ****************************************************/

AliGenerator * 
GeneratorHijing()
{
  comment = comment.Append(" PbPb: HIJING");
  AliGenHijing *gener = new AliGenHijing(-1);
  // centre of mass energy
  gener->SetEnergyCMS(energyConfig);
  gener->SetImpactParameterRange(0., 20.);
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

