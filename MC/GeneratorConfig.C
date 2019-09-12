/*
 * AliDPG - ALICE Experiment Data Preparation Group
 * Generator configuration script
 *
 */
 
#if !(defined(__CLING__)  || defined(__CINT__)) || defined(__ROOTCLING__) || defined(__ROOTCINT__)
#include "AliGenPythia.h"
#include "AliDecayer.h"
#endif

enum EGenerator_t {
  kGeneratorDefault,
  // Pythia6
  kGeneratorPythia6, kGeneratorPythia6_Perugia2011,
  // Pythia6 jets
  kGeneratorPythia6Jets, kGeneratorPythia6Jets_Perugia2011,
  // Pythia6 jets gamma-triggered
  kGeneratorPythia6JetsGammaTrg, kGeneratorPythia6JetsGammaTrg_Perugia2011,
  // Pythia6 gamma-jet
  kGeneratorPythia6GammaJet, kGeneratorPythia6GammaJet_Perugia2011,
  // Pythia8
  kGeneratorPythia8, kGeneratorPythia8_Monash2013,
  // Pythia8 jets
  kGeneratorPythia8Jets, kGeneratorPythia8Jets_Monash2013,
  // Pythia8 jets gamma-triggered
  kGeneratorPythia8JetsGammaTrg, kGeneratorPythia8JetsGammaTrg_Monash2013,
  // Pythia8 gamma-jet
  kGeneratorPythia8GammaJet, kGeneratorPythia8GammaJet_Monash2013,
  // Phojet
  kGeneratorPhojet, kGeneratorDpmjet,
  // EPOS-LHC
  kGeneratorEPOSLHC, kGeneratorEPOSLHC_pp, kGeneratorEPOSLHC_PbPb,
  // Hijing
  kGeneratorHijing,
  // Starlight
  kGeneratorStarlight,
  // DRgen
  kGeneratorDRgen,
  // AMPT
  kGeneratorAMPT, kGeneratorAMPT_v2,
  // Therminator2
  kGeneratorTherminator2,
  // ALIGENMC
  kGeneratorALIGENMC,
  // QED electrons
  kGeneratorQED,
  // RELDIS
  kGeneratorRELDIS,
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
  // Pythia6 jets
  "Pythia6Jets", "Pythia6Jets_Perugia2011",
  // Pythia6 jets gamma-triggered
  "Pythia6JetsGammaTrg", "Pythia6JetsGammaTrg_Perugia2011",
  // Pythia6 gamma-jet
  "Pythia6GammaJet", "Pythia6GammaJet_Perugia2011",
  // Pythia8
  "Pythia8", "Pythia8_Monash2013",
  // Pythia8 jets
  "Pythia8Jets", "Pythia8Jets_Monash2013",
  // Pythia8 jets gamma-triggered
  "Pythia8JetsGammaTrg", "Pythia8JetsGammaTrg_Monash2013",
  // Pythia8 gamma-jet
  "Pythia8GammaJet", "Pythia8GammaJet_Monash2013",
  // Phojet / DPMjet
  "Phojet", "Dpmjet",
  // EPOS-LHC
  "EPOSLHC", "EPOSLHC_pp", "EPOSLHC_PbPb",
  // Hijing
  "Hijing",
  // Starlight
  "Starlight",
  // DRgen
  "DRgen",
  // AMPT
  "AMPT", "AMPT_v2",
  // Therminator2
  "Therminator2",
  // ALIGENMC
  "ALIGENMC",
  // QED electrons
  "QED",
  // RELDIS
  "RELDIS",
  //
  "Custom",
  //
  "PWG"
};

enum ETrigger_t {
  kTriggerDefault,
  kTriggerPP,
  kTriggerPbPb,
  kTriggerMuon,
  kTriggerCustom,
  kNTriggers
};

const Char_t *TriggerName[kNTriggers] = {
  "ocdb",
  "p-p",
  "Pb-Pb",
  "MUON",
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
  kPythia6HeavyDecay_Electron,
  kPythia6HeavyDecay_All,
  kPythia6HFYellowReport,
  kPythia6HeavyDecay_Hadrons_D0pionic,
  kPythia6HeavyDecay_HadronsWithV0_D0pionic,
};

/*****************************************************************/

enum ECalorimeterAcceptance_t {
  kCalorimeterAcceptance_FullDetector,
  kCalorimeterAcceptance_EMCRun1,
  kCalorimeterAcceptance_PHSRun1,
  kCalorimeterAcceptance_EMCRun2,
  kCalorimeterAcceptance_PHSRun2,
  kCalorimeterAcceptance_PHSDMC
};

void 
GetCalorimeterAcceptance(Int_t acceptance, Float_t &etaMax, Float_t &phiMin, Float_t &phiMax)
{
  switch (acceptance) 
    {
    case kCalorimeterAcceptance_FullDetector:
      etaMax = 1.5 ; phiMin =   0.; phiMax = 360.;
      break;   
    case kCalorimeterAcceptance_EMCRun1:
      etaMax = 0.7 ; phiMin =  80.; phiMax = 180.;
      break;
    case kCalorimeterAcceptance_EMCRun2:
      etaMax = 0.7 ; phiMin =  80.; phiMax = 187.;
      break;    
    case kCalorimeterAcceptance_PHSRun1:
      etaMax = 0.13; phiMin = 260.; phiMax = 320.;
      break;
    case kCalorimeterAcceptance_PHSRun2:
      etaMax = 0.13; phiMin = 250.; phiMax = 320.;
      break;    
    case kCalorimeterAcceptance_PHSDMC:
      etaMax = 0.7 ; phiMin = 250.; phiMax = 327.;
      break;
    }
  
  printf("\t Calorimeter acceptance for %d: |eta|<%2.2f - %2.2f<phi<%2.2f\n",acceptance,etaMax,phiMin,phiMax);
}

/*****************************************************************/
/*****************************************************************/
/*****************************************************************/

// functions
AliGenerator *GeneratorCocktail(TString name);
AliGenerator *GeneratorInjector(Int_t ninj, Int_t pdg, Float_t ptmin, Float_t ptmax, Float_t ymin, Float_t ymax, Float_t phimin = 0., Float_t phimax = 360.); 
AliGenParam* GeneratorParam(int n, int pdg, double ptmin, double ptmax, double ymin, double ymax, AliDecayer* dec = 0x0, double phimin = 0., double phimax = 360.);
AliGenerator *GeneratorPythia6(Int_t tune = 0, Int_t pdgtrig = 0, Float_t etatrig = 1.2);
AliGenerator *GeneratorPythia6Jets(Int_t tune = 0, Int_t acceptance = kCalorimeterAcceptance_FullDetector);
AliGenerator *GeneratorPythia6JetsGammaTrg(Int_t tune = 0, Int_t acceptance = kCalorimeterAcceptance_FullDetector);
AliGenerator *GeneratorPythia6GammaJet(Int_t tune = 0, Int_t acceptance = kCalorimeterAcceptance_FullDetector);
AliGenerator *GeneratorPythia6Heavy(Int_t process, Int_t decay, Int_t tune = 0, Bool_t HFonly = kTRUE);
AliGenerator *GeneratorPythia8(Int_t tune = 0, Int_t pdgtrig = 0, Float_t etatrig = 1.2);
AliGenerator *GeneratorPythia8Jets(Int_t tune = 0, Int_t acceptance = kCalorimeterAcceptance_FullDetector);
AliGenerator *GeneratorPythia8JetsGammaTrg(Int_t tune = 0, Int_t acceptance = kCalorimeterAcceptance_FullDetector);
AliGenerator *GeneratorPythia8GammaJet(Int_t tune = 0, Int_t acceptance = kCalorimeterAcceptance_FullDetector);
AliGenerator *GeneratorPhojet();
AliGenerator *GeneratorEPOSLHC();
AliGenerator *GeneratorHijing();
AliGenerator *Generator_Jpsiee(const Char_t *params, Float_t jpsifrac, Float_t lowfrac, Float_t highfrac, Float_t bfrac, Bool_t useEvtGenForB=kFALSE, Float_t bhighfrac = 0.0);
AliGenerator *Generator_Nuclex(UInt_t injbit, Bool_t antiparticle, Int_t ninj, Float_t max_pt = 10.f, Float_t max_y = 1.);
AliGenerator *GeneratorStarlight();
AliGenerator *GeneratorDRgen();
AliGenerator *GeneratorAMPT();
AliGenerator *GeneratorAMPT_v2();
AliGenerator *GeneratorTherminator2();
AliGenerator *GeneratorALIGENMC();
AliGenerator *GeneratorQED();
AliGenerator *GeneratorRELDIS();

/*****************************************************************/

// global variables
static TString comment;

/*****************************************************************/

AliGenerator* gg_tmp_gen;
TString gg_tmp_str;
void GeneratorConfig(Int_t tag)
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

    // Pythia6Jets (Perugia2011)
  case kGeneratorPythia6Jets:
  case kGeneratorPythia6Jets_Perugia2011:
    gen = GeneratorPythia6Jets(kPythia6Tune_Perugia2011);
    break;

    // Pythia6JetsGammaTrg (Perugia2011)
  case kGeneratorPythia6JetsGammaTrg:
  case kGeneratorPythia6JetsGammaTrg_Perugia2011:
    gen = GeneratorPythia6JetsGammaTrg(kPythia6Tune_Perugia2011);
    break;

    // Pythia6GammaJet (Perugia2011)
  case kGeneratorPythia6GammaJet:
  case kGeneratorPythia6GammaJet_Perugia2011:
    gen = GeneratorPythia6GammaJet(kPythia6Tune_Perugia2011);
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

    // Pythia8JetsGammaTrg (Monash2013)
  case kGeneratorPythia8JetsGammaTrg:
  case kGeneratorPythia8JetsGammaTrg_Monash2013:
    gen = GeneratorPythia8JetsGammaTrg(kPythia8Tune_Monash2013);
    break;
    
    // Pythia8GammaJet (Monash2013)
  case kGeneratorPythia8GammaJet:
  case kGeneratorPythia8GammaJet_Monash2013:
    gen = GeneratorPythia8GammaJet(kPythia8Tune_Monash2013);
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

    // Starlight
  case kGeneratorDRgen:
    gen = GeneratorDRgen();
    break;

    // AMPT
  case kGeneratorAMPT:
    gen = GeneratorAMPT();
    break;

 case kGeneratorAMPT_v2:
    gen = GeneratorAMPT_v2();
    break;

 case kGeneratorTherminator2:
    gen = GeneratorTherminator2();
    break;

 case kGeneratorALIGENMC:
    gen = GeneratorALIGENMC();
    break;
  case kGeneratorQED:
    gen = GeneratorQED();
    break;

  case kGeneratorRELDIS:
    gen = GeneratorRELDIS();
    break;

    // Custom
  case kGeneratorCustom:
    if ((gROOT->LoadMacro("GeneratorCustom.C")) != 0) {
      printf("ERROR: cannot find GeneratorCustom.C\n");
      abort();
      return;
    }
    gROOT->ProcessLine("gg_tmp_gen = GeneratorCustom();");
    gen = gg_tmp_gen;
    break;

    // PWG
  case kGeneratorPWG:
    TString genstr = gSystem->Getenv("CONFIG_GENERATOR");
    TObjArray *oa = genstr.Tokenize(":");
    TObjString *pwg = (TObjString*) oa->At(0);
    TObjString *pwggen = (TObjString*) oa->At(1);
    TObjString *pwgopt = (TObjString*) oa->At(2);
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
    {
      gg_tmp_str = pwgopt->String();
      gROOT->ProcessLine("gg_tmp_gen = GeneratorCustom(gg_tmp_str);");
      gen = gg_tmp_gen;
    }
    else
    {
      gROOT->ProcessLine("gg_tmp_gen = GeneratorCustom();");
      gen = gg_tmp_gen;
    }
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
    if (periodName.EqualTo("LHC16d")) sigmaxy =  50.e-4; 
    if (periodName.EqualTo("LHC16e")) sigmaxy =  55.e-4; 
    if (periodName.EqualTo("LHC16f")) sigmaxy =  60.e-4; 
    if (periodName.EqualTo("LHC16g")) sigmaxy =  60.e-4; 
    if (periodName.EqualTo("LHC16h")) sigmaxy =  55.e-4; 
    if (periodName.EqualTo("LHC16i")) sigmaxy =  60.e-4; 
    if (periodName.EqualTo("LHC16j")) sigmaxy =  55.e-4; 
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

  if(gSystem->Getenv("CONFIG_SIMULATION")){
    if(strcmp(gSystem->Getenv("CONFIG_SIMULATION"), "GeneratorOnly") == 0){
      Printf("Generator only simulation => ideal vertex at (0,0,0)");
      gen->SetSigma(0.,0.,0.);
      gen->SetVertexSmear(kNoSmear);
    }
    else{
      gen->SetSigma(sigmaxy, sigmaxy, 5.);
      gen->SetVertexSmear(kPerEvent);
    }
  }
  else{
    gen->SetSigma(sigmaxy, sigmaxy, 5.);
    gen->SetVertexSmear(kPerEvent);
  }
  
 
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
  // system-dependent settings
  if (systemConfig.EqualTo("p-Pb")) {
    pythia->SetProjectile("P", 1, 1);
    pythia->SetTarget    ("n", 208, 82);
    pythia->SetUseNuclearPDF(kTRUE);
    pythia->SetUseLorentzBoost(kFALSE);  // the Lorentz boost should be switched off to avoid biases in the D and B meson decay topology
    comment = comment.Append(" | p-Pb");
  }
  else if (systemConfig.EqualTo("Pb-p")) {
    pythia->SetProjectile("n", 208, 82);
    pythia->SetTarget    ("P", 1, 1);
    pythia->SetUseNuclearPDF(kTRUE);
    pythia->SetUseLorentzBoost(kFALSE);  // the Lorentz boost should be switched off to avoid biases in the D and B meson decay topology
    comment = comment.Append(" | Pb-p");
  }
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
//    comment = comment.Append(Form(" | %s enhanced", TDatabasePDG::Instance()->GetParticle(pdgtrig)->GetName()));
    pythia->SetTriggerParticle(pdgtrig, etatrig);
  }
  //
  return pythia;
}

/*** PYTHIA 6 JETS ***********************************************/

AliGenerator *
GeneratorPythia6Jets(Int_t tune, Int_t acceptance)
{
  //
  //
  comment = comment.Append(Form(" | Pythia6 jets (%.1f, %.1f, %d, %.1f)", pthardminConfig, pthardmaxConfig, quenchingConfig, qhatConfig));
  //
  // Pythia
  AliGenPythia *pythia = (AliGenPythia*) GeneratorPythia6(tune);
  //
  // jets settings
  pythia->SetProcess(kPyJets);
  Float_t etaMax, phiMin, phiMax;
  GetCalorimeterAcceptance(acceptance, etaMax, phiMin, phiMax);
  pythia->SetJetEtaRange(-etaMax, etaMax); // Final state kinematic cuts
  pythia->SetJetPhiRange(phiMin, phiMax);
  pythia->SetPtHard(pthardminConfig, pthardmaxConfig); // Pt transfer of the hard scattering
  pythia->SetStrucFunc(kCTEQ5L);
  // quenching
  pythia->SetQuench(quenchingConfig);
  switch (quenchingConfig) {
  case 1:
    {
      Float_t k = 6.e5 * (qhatConfig / 1.7);  //qhat=1.7, k=6e5, default value
      AliPythia::Instance()->InitQuenching(0., 0.1, k, 0, 0.95, 6);		
    }
    break;
  case 2:
    pythia->SetPyquenPar(1.,0.1,0,0,1);			
    break;
  }
  //
  return pythia;
}

/*** PYTHIA 6 JETS GAMMA-TRIGGERED ********************************/

AliGenerator *
GeneratorPythia6JetsGammaTrg(Int_t tune, Int_t acceptance)
{
  //
  //
  comment = comment.Append(Form(" | Pythia6 jets gamma-triggered"));
  //
  // Pythia
  AliGenPythia *pythia = (AliGenPythia*) GeneratorPythia6Jets(tune, kCalorimeterAcceptance_FullDetector);
  //
  //
  // Careful with pT hard limits if triggerParticleInCalo option is on
  pythia->SetTriggerParticleMinPt(pttrigminConfig);
  // acceptance
  Float_t etaMax, phiMin, phiMax;
  GetCalorimeterAcceptance(acceptance, etaMax, phiMin, phiMax);
  switch (acceptance) {
    //
  case kCalorimeterAcceptance_FullDetector:
    pythia->SetBarrelAcceptance(etaMax);
    pythia->SetDecayPhotonInBarrel(kTRUE);
    break;
    //
  case kCalorimeterAcceptance_EMCRun1:
  case kCalorimeterAcceptance_EMCRun2:
    pythia->SetEMCALAcceptance(phiMin,phiMax,etaMax);
    pythia->SetDecayPhotonInEMCAL(kTRUE);
    break;
    //
  case kCalorimeterAcceptance_PHSDMC:
  case kCalorimeterAcceptance_PHSRun1:
  case kCalorimeterAcceptance_PHSRun2:
    pythia->SetPHOSAcceptance(phiMin,phiMax,etaMax);
    pythia->SetDecayPhotonInPHOS(kTRUE);
    break;
  }
  //
  return pythia;
}

/*** PYTHIA 6 GAMMA-JET ***********************************************/

AliGenerator *
GeneratorPythia6GammaJet(Int_t tune, Int_t acceptance)
{
  //
  //
  comment = comment.Append(Form(" | Pythia6 gamma-jet"));
  //
  // Pythia
  AliGenPythia *pythia = (AliGenPythia*) GeneratorPythia6Jets(tune);
  //
  // gamma settings
  pythia->SetProcess(kPyDirectGamma);
  Float_t etaMax, phiMin, phiMax;
  GetCalorimeterAcceptance(acceptance, etaMax, phiMin, phiMax);
  pythia->SetGammaEtaRange(-etaMax, etaMax);
  pythia->SetGammaPhiRange(phiMin, phiMax);
  // reset jets settings
  pythia->SetJetEtaRange(-20., 20.); // Final state kinematic cuts
  pythia->SetJetPhiRange(0., 360.);
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
  AliGenPythia *pythia = (AliGenPythia*) GeneratorPythia6(tune);
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
  case kPythia6HeavyDecay_All:
    pythia->SetHeavyQuarkYRange(-1.5, 1.5);
    break;
  case kPythia6HFYellowReport:
    pythia->SetHeavyQuarkYRange(-1.5, 1.5);
    pythia->SetForceDecay(kHFYellowReport);
    break;
  case kPythia6HeavyDecay_Hadrons_D0pionic:
    pythia->SetHeavyQuarkYRange(-1.5, 1.5);
    pythia->SetForceDecay(kHadronicDWithout4BodiesPionicD0);
    break;
  case kPythia6HeavyDecay_HadronsWithV0_D0pionic:
    pythia->SetHeavyQuarkYRange(-1.5, 1.5);
    pythia->SetForceDecay(kHadronicDWithout4BodiesWithV0PionicD0);
    break;
  }
  //
  // Pt transfer of the hard scattering (set for all cases)
  pythia->SetPtHard(pthardminConfig, pthardmaxConfig); 
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
  // system-dependent settings
  if (systemConfig.EqualTo("p-Pb")) {
    pythia->SetProjectile("P", 1, 1);
    pythia->SetTarget    ("n", 208, 82);
    //    (AliPythia8::Instance())->SetMSTP(52, 2);
    //    (AliPythia8::Instance())->SetMSTP(192, 1);
    //    (AliPythia8::Instance())->SetMSTP(193, 208);
    //    pythia->SetUseNuclearPDF(kTRUE); // does not exist for AliGenPythiaPlus
    //    pythia->SetUseLorentzBoost(kTRUE); // does not exist for AliGenPythiaPlus, boost is always performed
    comment = comment.Append(" | p-Pb Boosted");
  }
  else if (systemConfig.EqualTo("Pb-p")) {
    pythia->SetProjectile("n", 208, 82);
    pythia->SetTarget    ("P", 1, 1);
    //    (AliPythia8::Instance())->SetMSTP(52, 2);
    //    (AliPythia8::Instance())->SetMSTP(192, 208);
    //    (AliPythia8::Instance())->SetMSTP(193, 1);
    //    pythia->SetUseNuclearPDF(kTRUE); // does not exist for AliGenPythiaPlus
    //    pythia->SetUseLorentzBoost(kTRUE); // does not exist for AliGenPythiaPlus, boost is always performed
    comment = comment.Append(" | Pb-p Boosted");
  }
  //
  // Initialize
  pythia->SetEventListRange(-1, 2); 
  (AliPythia8::Instance())->ReadString("Random:setSeed = on");
  (AliPythia8::Instance())->ReadString(Form("Random:seed = %ld", (long) seedConfig % 900000000)); 
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
//    comment = comment.Append(Form(" | %s enhanced", TDatabasePDG::Instance()->GetParticle(pdgtrig)->GetName()));
    pythia->SetTriggerParticle(pdgtrig, etatrig);
  }
  //
  return pythia;
}

/*** PYTHIA 8 JETS ***********************************************/

AliGenerator *
GeneratorPythia8Jets(Int_t tune, Int_t acceptance)
{
  //
  //
  comment = comment.Append(Form(" | Pythia8 jets (%.1f, %.1f, %d, %.1f)", pthardminConfig, pthardmaxConfig, quenchingConfig, qhatConfig));
  //
  // Pythia
  AliGenPythiaPlus *pythia = (AliGenPythiaPlus*) GeneratorPythia8(tune);
  //
  // jets settings
  pythia->SetProcess(kPyJets);
  Float_t etaMax, phiMin, phiMax;
  GetCalorimeterAcceptance(acceptance, etaMax, phiMin, phiMax);
  pythia->SetJetEtaRange(-etaMax, etaMax); // Final state kinematic cuts
  pythia->SetJetPhiRange(phiMin, phiMax);
  pythia->SetJetEtRange(0., 1000.);
  pythia->SetPtHard(pthardminConfig, pthardmaxConfig); // Pt transfer of the hard scattering
  pythia->SetStrucFunc(kCTEQ5L);
  // quenching
  pythia->SetQuench(quenchingConfig);
  switch (quenchingConfig) {
  case 1:
    {
      Float_t k = 6.e5 * (qhatConfig / 1.7);  //qhat=1.7, k=6e5, default value
      AliPythia8::Instance()->InitQuenching(0., 0.1, k, 0, 0.95, 6);		
    }
    break;
  case 2:
    //pythia->SetPyquenPar(1.,0.1,0,0,1); //TODO: FixMe - This doesn't exist in AliGenPythiaPlus
    break;
  }
  //
  return pythia;
}

/*** PYTHIA 8 JETS GAMMA-TRIGGERED ********************************/

AliGenerator *
GeneratorPythia8JetsGammaTrg(Int_t tune, Int_t acceptance)
{
  //
  //
  comment = comment.Append(Form(" | Pythia8 jets gamma-triggered"));
  //
  // Pythia
  AliGenPythiaPlus *pythia = (AliGenPythiaPlus*) GeneratorPythia8Jets(tune, kCalorimeterAcceptance_FullDetector);
  //
  //
  // Careful with pT hard limits if triggerParticleInCalo option is on
  pythia->SetTriggerParticleMinPt(pttrigminConfig); 
  // acceptance
  Float_t etaMax, phiMin, phiMax;
  GetCalorimeterAcceptance(acceptance, etaMax, phiMin, phiMax);
  switch (acceptance) {
    //
  case kCalorimeterAcceptance_FullDetector:
    pythia->SetBarrelAcceptance(etaMax);
    pythia->SetDecayPhotonInBarrel(kTRUE);
    break;
    //
  case kCalorimeterAcceptance_EMCRun1:
  case kCalorimeterAcceptance_EMCRun2:
    pythia->SetEMCALAcceptance(phiMin,phiMax,etaMax);
    pythia->SetDecayPhotonInEMCAL(kTRUE);
    break;
    //
  case kCalorimeterAcceptance_PHSDMC:
  case kCalorimeterAcceptance_PHSRun1:
  case kCalorimeterAcceptance_PHSRun2:
    pythia->SetPHOSAcceptance(phiMin,phiMax,etaMax);
    pythia->SetDecayPhotonInPHOS(kTRUE);
    break;
  }
  //
  return pythia;
}

/*** PYTHIA 8 GAMMA-JET ***********************************************/

AliGenerator *
GeneratorPythia8GammaJet(Int_t tune, Int_t acceptance)
{
  //
  //
  comment = comment.Append(Form(" | Pythia8 gamma-jet"));
  //
  // Pythia
  AliGenPythiaPlus *pythia = (AliGenPythiaPlus*) GeneratorPythia8Jets(tune);
  //
  // gamma settings
  pythia->SetProcess(kPyDirectGamma);
  Float_t etaMax, phiMin, phiMax;
  GetCalorimeterAcceptance(acceptance, etaMax, phiMin, phiMax);
  pythia->SetGammaEtaRange(-etaMax, etaMax);
  pythia->SetGammaPhiRange(phiMin, phiMax);
  // reset jets settings
  pythia->SetJetEtaRange(-20., 20.); // Final state kinematic cuts
  pythia->SetJetPhiRange(0., 360.);
  //
  return pythia;
}

/*** PHOJET ****************************************************/

AliGenerator *
GeneratorPhojet()
{
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,0,0)
  printf("ERROR: DPMJET not yet compatible with ROOT 6\n");
  return NULL;
#else
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
  else if (systemConfig.EqualTo("Xe-Xe")) {
    dpmjet->SetProjectile("A", 129, 54);
    dpmjet->SetTarget    ("A", 129, 54);
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
#endif
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
  // XeXe
  else if (systemConfig.EqualTo("Xe-Xe")) {
    projectileId = 54*10000 + 129*10;
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
    gener->SetDataDrivenSpectators();
  }
  else if (systemConfig.EqualTo("Xe-Xe")) {
    gener->SetProjectile("A", 129, 54);
    gener->SetTarget    ("A", 129, 54);
    gener->SetSpectators(0);
    gener->SetDataDrivenSpectators();
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
  //  gener->SetDecaysOff(1);
  // kinematic selection
  gener->SetSelectAll(0);
  gener->SetPtHardMin(ptHardMinHijing);
  return gener;
}

/*** STARLIGHT ****************************************************/

AliGenerator *
GeneratorStarlight(){
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,0,0)
  printf("ERROR: Starlight not yet compatible with ROOT 6!\n");
  return NULL;
#else
  gSystem->Load("libStarLight.so");
  gSystem->Load("libAliStarLight.so");

  // Supported processes:
  // _STARLIGHT_PROCESSES_BEGIN_ this comment is used by AliRoot/STARLIGHT/test/testsl.C
  // kTwoGammaToMuLow    - from 0.4 to 15 GeV
  // kTwoGammaToElLow    - from 0.4 to 15 GeV
  // kTwoGammaToMuMedium - from 1.8 to 15 GeV
  // kTwoGammaToElMedium - from 1.8 to 15 GeV
  // kTwoGammaToMuHigh   - from 4.0 to 15 GeV
  // kTwoGammaToElHigh   - from 4.0 to 15 GeV
  // kTwoGammaToRhoRho
  // kTwoGammaToF2
  // kCohRhoToPi
  // kCohRhoToPiWithCont
  // kCohRhoToPiFlat
  // kCohPhiToKa
  // kCohJpsiToMu
  // kCohJpsiToEl
  // kCohJpsiToElRad
  // kCohJpsiToProton
  // kCohPsi2sToMu
  // kCohPsi2sToEl
  // kCohPsi2sToMuPi
  // kCohPsi2sToElPi
  // kCohUpsilonToMu
  // kCohUpsilonToEl
  // kIncohRhoToPi
  // kIncohRhoToPiWithCont
  // kIncohRhoToPiFlat
  // kIncohPhiToKa
  // kIncohJpsiToMu
  // kIncohJpsiToEl
  // kIncohJpsiToElRad
  // kIncohJpsiToProton
  // kIncohPsi2sToMu
  // kIncohPsi2sToEl
  // kIncohPsi2sToMuPi
  // kIncohPsi2sToElPi
  // kIncohUpsilonToMu
  // kIncohUpsilonToEl
  // _STARLIGHT_PROCESSES_END_ this comment is used by AliRoot/STARLIGHT/test/testsl.C
  //
  // Not supported:
  // kCohRhoPrime
  // kIncohRhoPrime

  comment.Append(Form(" | Starlight %s (%s)", processConfig.Data(), systemConfig.Data()));

  // restict ymin,ymax to [-8,8] as with the default ymin,ymax set in Config.C
  // there is a floating point exception in starlight
  yminConfig = TMath::Max(yminConfig, -8.0f);
  ymaxConfig = TMath::Min(ymaxConfig, +8.0f);
  
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
  // XeXe
  else if (systemConfig.EqualTo("Xe-Xe")) {
    projA = 129; projZ = 54;
    targA = 129; targZ = 54;
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
  Float_t rapBoost = 0.5*(TMath::ACosH(gamma2)-TMath::ACosH(gamma1));
  
  // maximum absolute rapidity in CMS used in genuine starlight parameters 
  // rapidity bins are calculated wrt (-rapMax,rapMax) interval
  Float_t rapMax = TMath::Max(TMath::Abs(yminConfig+rapBoost),TMath::Abs(ymaxConfig+rapBoost));

  Bool_t cocktail = kFALSE;
  cocktail |= processConfig.Contains("Psi2sToElPi");
  cocktail |= processConfig.Contains("Psi2sToMuPi");
  cocktail |= processConfig.Contains("RhoPrime");
  cocktail |= processConfig.Contains("JpsiToElRad");
  
  AliGenCocktail *genCocktail = NULL;
  if (cocktail)  genCocktail = new AliGenCocktail(); // constructor must be called before other generators

  const struct SLConfig {
    const char* name;
    Int_t       prod_mode;
    Int_t       prod_pid;
    Int_t       nwbins;
    Float_t     wmin;
    Float_t     wmax;
    Float_t     dy;
  } slConfig[] = {
    {"kTwoGammaToMuLow",     1,      13,  292,  0.4, 15.0, 0.01 }, // from 0.4 to 15 GeV
    {"kTwoGammaToElLow",     1,      11,  292,  0.4, 15.0, 0.01 }, // from 0.4 to 15 GeV
    {"kTwoGammaToMuMedium",  1,      13,  264,  1.8, 15.0, 0.01 }, // from 1.8 to 15 GeV
    {"kTwoGammaToElMedium",  1,      11,  264,  1.8, 15.0, 0.01 }, // from 1.8 to 15 GeV
    {"kTwoGammaToMuHigh",    1,      13,  220,  4.0, 15.0, 0.01 }, // from 4.0 to 15 GeV
    {"kTwoGammaToElHigh",    1,      11,  220,  4.0, 15.0, 0.01 }, // from 4.0 to 15 GeV
    {"kTwoGammaToRhoRho",    1,      33,   20, -1.0, -1.0, 0.01 }, //
    {"kTwoGammaToF2",        1,     225,   20, -1.0, -1.0, 0.01 }, //
    {"kCohRhoToPi",          3,     113, 1200, -1.0, -1.0, 0.02 }, //
    {"kCohRhoToPiWithCont",  3,     913, 1200, -1.0, -1.0, 0.02 }, //
    {"kCohRhoToPiFlat",      3,     113,    1, -1.0,  2.5, 0.02 }, //
    {"kCohPhiToKa",          2,     333,   20, -1.0, -1.0, 0.01 }, //
    {"kCohJpsiToMu",         2,  443013,   20, -1.0, -1.0, 0.01 }, //
    {"kCohJpsiToEl",         2,  443011,   20, -1.0, -1.0, 0.01 }, //
    {"kCohJpsiToElRad",      2,  443011,   20, -1.0, -1.0, 0.01 }, //
    {"kCohJpsiToProton",     2, 4432212,   20, -1.0, -1.0, 0.01 }, //
    {"kCohPsi2sToMu",        2,  444013,   20, -1.0, -1.0, 0.01 }, //
    {"kCohPsi2sToEl",        2,  444011,   20, -1.0, -1.0, 0.01 }, //
    {"kCohPsi2sToMuPi",      2,  444013,   20, -1.0, -1.0, 0.01 }, //
    {"kCohPsi2sToElPi",      2,  444011,   20, -1.0, -1.0, 0.01 }, //
    {"kCohUpsilonToMu",      2,  553013,   20, -1.0, -1.0, 0.01 }, //
    {"kCohUpsilonToEl",      2,  553011,   20, -1.0, -1.0, 0.01 }, //
    {"kIncohRhoToPi",        4,     113, 1200, -1.0, -1.0, 0.02 }, //
    {"kIncohRhoToPiWithCont",4,     913, 1200, -1.0, -1.0, 0.02 }, //
    {"kIncohRhoToPiFlat",    4,     113,    1, -1.0,  2.5, 0.02 }, //
    {"kIncohPhiToKa",        4,     333,   20, -1.0, -1.0, 0.01 }, //
    {"kIncohJpsiToMu",       4,  443013,   20, -1.0, -1.0, 0.01 }, //
    {"kIncohJpsiToEl",       4,  443011,   20, -1.0, -1.0, 0.01 }, //
    {"kIncohJpsiToElRad",    4,  443011,   20, -1.0, -1.0, 0.01 }, //
    {"kIncohJpsiToProton",   4, 4432212,   20, -1.0, -1.0, 0.01 }, //
    {"kIncohPsi2sToMu",      4,  444013,   20, -1.0, -1.0, 0.01 }, //
    {"kIncohPsi2sToEl",      4,  444011,   20, -1.0, -1.0, 0.01 }, //
    {"kIncohPsi2sToMuPi",    4,  444013,   20, -1.0, -1.0, 0.01 }, //
    {"kIncohPsi2sToElPi",    4,  444011,   20, -1.0, -1.0, 0.01 }, //
    {"kIncohUpsilonToMu",    4,  553013,   20, -1.0, -1.0, 0.01 }, //
    {"kIncohUpsilonToEl",    4,  553011,   20, -1.0, -1.0, 0.01 }, //
 // {"kCohRhoPrime",         3,      0,   20,  -1.0, -1.0, 0.01 }, //
 // {"kIncohRhoPrime",       4,      0,   20,  -1.0, -1.0, 0.01 }, //
  };
  const Int_t nProcess = sizeof(slConfig)/sizeof(SLConfig);
  Int_t idx = -1;
  for (Int_t i=0; i<nProcess; ++i) {
    // Printf("i=%3d %s %s", i, slConfig[i].name, processConfig.Data());
    if (processConfig == TString(slConfig[i].name)) {
      idx = i;
      break;
    }
  }
  if (idx == -1) {
    printf("STARLIGHT process '%s' is not supported\n", processConfig.Data());
    abort();
  }
  
  AliGenStarLight* genStarLight = new AliGenStarLight(1000*1000);
  genStarLight->SetParameter(Form("BEAM_1_Z     =    %3i    #Z of target",targZ));
  genStarLight->SetParameter(Form("BEAM_1_A     =    %3i    #A of target",targA));
  genStarLight->SetParameter(Form("BEAM_2_Z     =    %3i    #Z of projectile",projZ));
  genStarLight->SetParameter(Form("BEAM_2_A     =    %3i    #A of projectile",projA));
  genStarLight->SetParameter(Form("BEAM_1_GAMMA = %6.1f    #Gamma of the target",gamma1));
  genStarLight->SetParameter(Form("BEAM_2_GAMMA = %6.1f    #Gamma of the projectile",gamma2));
  genStarLight->SetParameter(Form("W_MAX        =   %.1f    #Max value of w",slConfig[idx].wmax));
  genStarLight->SetParameter(Form("W_MIN        =   %.1f    #Min value of w",slConfig[idx].wmin));
  genStarLight->SetParameter(Form("W_N_BINS     =    %3i    #Bins i w",slConfig[idx].nwbins));
  genStarLight->SetParameter(Form("RAP_MAX      =   %.2f    #max y",rapMax));
  genStarLight->SetParameter(Form("RAP_N_BINS   =   %.0f    #Bins i y",rapMax*2./slConfig[idx].dy));
  genStarLight->SetParameter("CUT_PT       =    0    #Cut in pT? 0 = (no, 1 = yes)");
  genStarLight->SetParameter("PT_MIN       =    0    #Minimum pT in GeV");
  genStarLight->SetParameter("PT_MAX       =   10    #Maximum pT in GeV");
  genStarLight->SetParameter("CUT_ETA      =    0    #Cut in pseudorapidity? (0 = no, 1 = yes)");
  genStarLight->SetParameter("ETA_MIN      =   -5    #Minimum pseudorapidity");
  genStarLight->SetParameter("ETA_MAX      =    5    #Maximum pseudorapidity");
  genStarLight->SetParameter(Form("PROD_MODE    =    %i    #gg or gP switch (1 = 2-photon, 2 = coherent vector meson (narrow), 3 = coherent vector meson (wide), # 4 = incoherent vector meson, 5 = A+A DPMJet single, 6 = A+A DPMJet double, 7 = p+A DPMJet single, 8 = p+A Pythia single )",slConfig[idx].prod_mode));
  genStarLight->SetParameter(Form("PROD_PID     =   %6i    #Channel of interest (not relevant for photonuclear processes)",slConfig[idx].prod_pid));
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
  else if (processConfig.Contains("JpsiToElRad")) decayTable="JPSI.EE.DEC";
  genEvtGen->SetUserDecayTable(gSystem->ExpandPathName(Form("$ALICE_ROOT/STARLIGHT/AliStarLight/DecayTables/%s",decayTable.Data())));

  if      (processConfig.Contains("Psi2s"))       genEvtGen->SetEvtGenPartNumber(100443);
  else if (processConfig.Contains("RhoPrime"))    genEvtGen->SetEvtGenPartNumber(30113);
  else if (processConfig.Contains("JpsiToElRad")) genEvtGen->SetEvtGenPartNumber(443);

  genEvtGen->SetPolarization(1);           // Set polarization: transversal(1),longitudinal(-1),unpolarized(0)
  gSystem->Setenv("PYTHIA8DATA", gSystem->ExpandPathName("$ALICE_ROOT/PYTHIA8/pythia8/xmldoc"));

  genCocktail->AddGenerator(genStarLight,"StarLight",1.);
  genCocktail->AddGenerator(genEvtGen,"EvtGen",1.);
  return genCocktail;
#endif
}


/*** DRgen ****************************************************/

AliGenerator *
GeneratorDRgen(){
  // see details in AliRoot/DRGEN/README
  TString supportedProcesses[9] = {
      "kDRgen_continuum2PiPi",
      "kDRgen_sigma2PiPi",
      "kDRgen_rho2PiPi",
      "kDRgen_f02PiPi",
      "kDRgen_f22PiPi",
      "kDRgen_f2DzeroPom2PiPi",
      "kDRgen_f2DplusminusPom2PiPi",
      "kDRgen_f2DzeroProt2PiPi",
      "kDRgen_f2DplusminusProt2PiPi"
  };
  comment.Append(Form(" | DRgen generator %s",processConfig.Data()));
  gSystem->Load("libDRGEN.so");
  
  gRandom->SetSeed(seedConfig);
  
  AliGenDRgen* gener = new AliGenDRgen();
  gener->SetBeamEnergy(energyConfig/2.);
  
  for (Int_t i=0;i<9;i++){
    if (processConfig!=supportedProcesses[i]) continue;
    gener->SetProcess(i+1,i+1);
    break;
  }
  
  return (AliGenerator*) gener;
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
  //  genHi->SetDecaysOff(1);     // neutral pion and heavy particle decays switched off
  genHi->SetSpectators(0);    // track spectators
  //Boost into LHC lab frame
  genHi->SetBoostLHC(1);
  //  genHi->Init();
  genHi->SetRandomReactionPlane(kTRUE);
 
  return genHi;

  
}


/*** AMPT_v226t7 onwards ****************************************************/

AliGenerator *
GeneratorAMPT_v2()
{

  // connect HepMC reader
  AliGenReaderHepMC *reader = new AliGenReaderHepMC();
  reader->SetFileName("ampteventfifo");
  AliGenExtFile *gener = new AliGenExtFile(-1);
  gener->SetName(Form("AMPT_%s", systemConfig.Data()));
  gener->SetReader(reader);
  
  return gener;
}


/*** Therminator2 *************************************************/

AliGenerator *
GeneratorTherminator2()
{
    // All the default model parameters in the Therminator2 .ini files can be changed 
    // using the THERM2_PARAMS_<param_name> variables
    // One exception is the XML_PATH variable which is part of the configurator
    // The .ini files are in the Therminator2 distribution (events.ini and fomodel/ contents)
    // https://github.com/alisw/therminator/tree/alice/v2.0.3

    // Example of setting a custom model
    //gSystem->Setenv("THERM2_PARAMS_FreezeOutModel", "BWAVT");

    // Example of setting a local xml path for the hydro model
    //gSystem->Setenv("THERM2_PARAMS_XML_PATH", "fomodel/lhyquid2dbi/LHCPbPb5500c0005Ti500ti100Tf145.xml");

    // Example of setting a GRID xml path for the hydro model
    //gSystem->Setenv("THERM2_PARAMS_XML_PATH", "alien:/alice/<rest_of_GRID_PATH>");

    AliGenExtExec* gener = new AliGenExtExec();
    gener->SetPathScript(gSystem->ExpandPathName("$ALIDPG_ROOT/MC/EXTRA/gen_therm2.sh"));
    return gener;
}

/*** ALIGENMC ****************************************************/

AliGenerator *
GeneratorALIGENMC()
{
  // select aligenmc as proxy generator
  AliGenExtExec *gener = new AliGenExtExec();
  gener->SetPathScript("$ALIDPG_ROOT/MC/aligenmc/gen_aligenmc.sh");

  return gener;
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
  // XeXe
  else if (systemConfig.EqualTo("Xe-Xe")) {
    projN = "A"; projA = 129; projZ = 54;
    targN = "A"; targA = 129; targZ = 54;
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

/*** PARAMETRIC INJECTOR ****************************************/
AliGenParam* GeneratorParam(int n, int pdg, double ptmin, double ptmax, double ymin, double ymax, AliDecayer* dec, double phimin, double phimax) {
  comment = comment.Append(Form(" | injected (pdg=%d, %d particles)", pdg, n));
  AliGenParam* gen = new AliGenParam(Form("%i",pdg), n, pdg);
  gen->SetYRange(ymin,ymax);
  gen->SetPtRange(ptmin,ptmax);
  gen->SetPhiRange(phimin,phimax);
  if (dec) gen->SetDecayer(dec);
  return gen;
}



/*** JPSI -> EE ****************************************************/

AliGenerator *
Generator_Jpsiee(const Char_t *params, Float_t jpsifrac, Float_t lowfrac, Float_t highfrac, Float_t bfrac, Bool_t useEvtGenForB, Float_t bhighfrac)
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
  TString stringParams = params;
  AliGenParam* jpsi    = NULL;
  if(stringParams.Contains("UserParam")){ 
   // use private parametrization for y / pT instead of AliGenMUONlib
   gROOT->LoadMacro("$ALIDPG_ROOT/MC/CustomGenerators/PWGDQ/GenJPsiParaSet.C++");
#if !(defined(__CLING__))
   jpsi = GenJPsiParaSet(stringParams);
#else
   Printf("ERROR: This option is not yet available for ROOT6 ... abort");
   return NULL;
#endif
  }
  else jpsi = new AliGenParam(1, AliGenMUONlib::kJpsi, params, "Jpsi");
  jpsi->SetPtRange(0., 1000.);
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
  jpsiHighPtFlat->SetPtRange(6., 50.);
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
  if(useEvtGenForB) pythia->SetForceDecay(kNoDecayBeauty);
  else {
  pythia->SetCutOnChild(1);
  pythia->SetPdgCodeParticleforAcceptanceCut(443);
  pythia->SetChildYRange(-2, 2);
  pythia->SetChildPtRange(0, 10000.);
  pythia->SetForceDecay(kBJpsiUndecayed);
  }
  pythia->SetStackFillOpt(AliGenPythia::kHeavyFlavor);
  // Jpsi from B - high pT
  AliGenPythia *pythiaHighPt = new AliGenPythia(-1);
  pythiaHighPt->SetMomentumRange(0, 999999.);
  pythiaHighPt->SetThetaRange(0., 180.);
  pythiaHighPt->SetYRange(-2., 2.);
  pythiaHighPt->SetPtRange(0, 1000.);
  pythiaHighPt->SetProcess(kPyBeautyppMNRwmi);
  pythiaHighPt->SetEnergyCMS(energyConfig);
  pythiaHighPt->SetTune(kPythia6Tune_Perugia0);
  pythiaHighPt->UseNewMultipleInteractionsScenario();
    // Cuts on child - J/psi
  pythiaHighPt->SetCutOnChild(1);
  pythiaHighPt->SetPdgCodeParticleforAcceptanceCut(443);
  pythiaHighPt->SetChildYRange(-1, 1);
  pythiaHighPt->SetChildPtRange(9, 50.);
  pythiaHighPt->SetForceDecay(kBJpsiUndecayed);
  pythiaHighPt->SetStackFillOpt(AliGenPythia::kHeavyFlavor);
  //
  // 
  AliGenEvtGen *gene = new AliGenEvtGen();
  gene->SetForceDecay(kBJpsiDiElectron);
  if(useEvtGenForB) gene->SetParticleSwitchedOff(AliGenEvtGen::kHFPart);
  else gene->SetParticleSwitchedOff(AliGenEvtGen::kCharmPart);

  if (jpsifrac > 0.) gener->AddGenerator(jpsi,           "JPsi",           jpsifrac);
  if (lowfrac  > 0.) gener->AddGenerator(jpsiLowPt,      "jpsiLowPt",      lowfrac);
  if (highfrac > 0.) gener->AddGenerator(jpsiHighPtFlat, "jpsiHighPtFlat", highfrac);
  if (bfrac    > 0.) gener->AddGenerator(pythia,         "Pythia",         bfrac);
  if (bhighfrac    > 0.) gener->AddGenerator(pythiaHighPt,         "jpsiBdecayPythiaHighPt",         bhighfrac);
  gener->AddGenerator(gene, "EvtGen", 1.);
  //
  return gener;
}

/*** NUCLEI EXOTICA ****************************************************/

AliGenerator *
Generator_Nuclex(UInt_t injbit, Bool_t antiparticle, Int_t ninj, Float_t max_pt, Float_t max_y)
{

  comment = comment.Append(Form(" | Nuclex (0x%x) ", injbit));

  //
  //Generating a cocktail
  AliGenCocktail *gener = new AliGenCocktail();

  Int_t pdgcodes[21] = {
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
    900010020,
    1010020050,
    1020010040
  };

  const Char_t *names[21] = {
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
    "d*(2380)",  //0x40000
    "Hyper-Helium-5", //0x80000
    "Double-Hyper-Hydrogen-4" //0x100000
  };

  for (Int_t ipart = 0; ipart < 21; ipart++) {
    if (injbit & 1 << ipart) {
      AliGenBox *box = new AliGenBox(ninj);
      Int_t pdg = pdgcodes[ipart];
      if (antiparticle) pdg = -pdg;
      box->SetPart(pdg);
      box->SetPtRange(0., max_pt);
      box->SetPhiRange(0., 360.);
      box->SetYRange(-max_y,max_y);
      gener->AddGenerator(box, names[ipart], 1);
      printf(">>>>> adding %d %s%s (%d) to the cocktail \n", ninj, antiparticle ? "anti-" : "", names[ipart], pdg);
    }
  }

  return gener;
}

/*** QED electrons ****************************************************/

AliGenerator * 
GeneratorQED()
{

  // configure projectile/target
  TString projN, targN;
  Int_t projA, projZ, targA, targZ;

  // Pb-Pb (only case so far)
  if (systemConfig.EqualTo("Pb-Pb")) {
    projN = "A"; projA = 208; projZ = 82;
    targN = "A"; targA = 208; targZ = 82;
  }
  // not implemented
  else {
    printf("QED electrons not implemented for %s system\n", systemConfig.Data());
    abort();
  }
  
  comment = comment.Append(Form(" | QED electrons "));
  //
  AliGenQEDBg *genBg = new AliGenQEDBg();
  genBg->SetEnergyCMS(energyConfig);
  genBg->SetProjectile(projN, projA, projZ);
  genBg->SetTarget    (targN, targA, targZ);
  genBg->SetYRange(-6.,3);
  genBg->SetPtRange(0.4e-3,1.0);       // Set pt limits (GeV) for e+-: 1MeV corresponds to max R=13.3mm at 5kGaus
  genBg->SetLumiIntTime(6.e27,3e-6);  // luminosity and integration time
  genBg->SetVertexSource(kInternal);
  
  return genBg;
}

/*** RELDIS ****************************************************/

AliGenerator *
GeneratorRELDIS()
{

  comment = comment.Append(Form(" | RELDIS-LHC (%s)", systemConfig.Data()));

  AliGenExtFile *gener     = new AliGenExtFile(-1);
  AliGenReadersEMD *reader = new AliGenReadersEMD();

  // need to resolve files from grid, make sure we have a valid connection
  if (!gGrid) TGrid::Connect("alien");

  // RELDIS configuration via process string
  Int_t firstev          = 0;
  TObjArray*  oa         = processConfig.Tokenize(":");
  TObjString* inputfile  = (TObjString*) oa->At(0);
  TObjString* startevent = (TObjString*) oa->At(1);
  TObjString* options    = (TObjString*) oa->At(2);

  // the first event number is calculated from the input here (startevent) and the number of events (CONFIG_NEVENTS)
  if(startevent) 
    firstev = atoi(startevent->GetName()) * atoi(gSystem->Getenv("CONFIG_NEVENTS"));
  
  printf("RELDIS configuration: Input file = %s, startevent = %d\n",inputfile->GetString().Data(),firstev);
  
  reader->SetFileName(Form("alien:///alice/cern.ch/user/p/pwg_mm/%s", inputfile->GetString().Data())); // put the location of the input file
  reader->SetNtupleName("h2034"); // name of the tree inside the input file
  reader->SetStartEvent(firstev); // # of event to start with

  if(!options){
    printf("RELDIS configuration: no optional arguments, track all particles\n");
  }
  else{
    if(options->GetString().Contains("TrackOnlyNeutrons")){
      reader->TrackOnlyNeutrons(); // include this if you want to track only neutrons
      printf("RELDIS configuration: Track only neutrons\n");
    }
    else if(options->GetString().Contains("TrackOnlyNucleons")){
      reader->TrackNucleons(); // include this if you want to track only nucleons
      printf("RELDIS configuration: Track all nucleons\n");
    }
    else{
      printf("RELDIS configuration: Track all (n, p and fragments)\n");
    }
    if(options->GetString().Contains("InvertPz")){
      reader->SetInvertPz(); // include this if you want to invert the particle momentum z component (A -> C side of ALICE)
      printf("RELDIS configuration: InvertPz\n");
    }
  }

  gener->SetReader(reader); 
  
  return gener;
}


