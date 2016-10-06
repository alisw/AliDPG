/*
 * AliDPG - ALICE Experiment Data Preparation Group
 * Generator configuration script
 *
 */

enum EGenerator_t {
  kGeneratorDefault,
  // Pythia6
  kGeneratorPythia6,
  kGeneratorPythia6_Perugia2011,
  kGeneratorPythia6_Perugia2011_Nuclex001, // [ALIROOT-6795]
  kGeneratorPythia6_Perugia2011_Nuclex002, // [ALIROOT-6796]
  kGeneratorPythia6_Perugia2011_HFhad001, kGeneratorPythia6_Perugia2011_HFele001,
  // Pythia8
  kGeneratorPythia8,
  kGeneratorPythia8_Monash2013,
  kGeneratorPythia8_Monash2013_Rsn001,  // [ALIROOT-6685]
  kGeneratorPythia8_Monash2013_Str002,  // [ALIROOT-6874]
  // Pythia8 jets
  kGeneratorPythia8Jets, 
  kGeneratorPythia8Jets_Monash2013, 
  // Phojet
  kGeneratorPhojet,
  // EPOS-LHC
  kGeneratorEPOSLHC, kGeneratorEPOSLHC_pp, kGeneratorEPOSLHC_PbPb,
  // Hijing
  kGeneratorHijing,
  kGeneratorHijing_QA001,
  kGeneratorHijing_Rsn002a, kGeneratorHijing_Rsn002b, kGeneratorHijing_Rsn002c, // [ALIROOT-6721] [ALIROOT-6722]
  kGeneratorHijing_Jpsiee001, // [ALIROOT-6750]
  kGeneratorHijing_Nuclex001, kGeneratorHijing_Nuclex003, kGeneratorHijing_Nuclex004, // [ALIROOT-6795] [ALIROOT-6825]
  kGeneratorHijing_Jets001, kGeneratorHijing_Jets001a, kGeneratorHijing_Jets001b, kGeneratorHijing_Jets001c, kGeneratorHijing_Jets001d, kGeneratorHijing_Jets001e,  // [ALIROOT-6822] [ALIROOT-6823] 
  kGeneratorHijing_Gamma001, // [ALIROOT-6824]
  kGeneratorHijing_HFhad001, kGeneratorHijing_HFele001,
  kGeneratorHijing_Str001a,   kGeneratorHijing_Str001b,   kGeneratorHijing_Str001c, // [ALIROOT-6858]
  // Starlight
  kGeneratorStarlight,
  // AMPT
  kGeneratorAMPT,
  //
  kGeneratorCustom,
  kNGenerators
};

const Char_t *GeneratorName[kNGenerators] = {
  "Default",
  // Pythia6
  "Pythia6",
  "Pythia6_Perugia2011",
  "Pythia6_Perugia2011_Nuclex001", 
  "Pythia6_Perugia2011_Nuclex002", 
  "Pythia6_Perugia2011_HFhad001", "Pythia6_Perugia2011_HFele001",
  // Pythia8
  "Pythia8",
  "Pythia8_Monash2013",
  "Pythia8_Monash2013_Rsn001",
  "Pythia8_Monash2013_Str002",
  // Pythia8 jets
  "Pythia8Jets", 
  "Pythia8Jets_Monash2013",
  // Phijet
  "Phojet",
  // EPOS-LHC
  "EPOSLHC", "EPOSLHC_pp", "EPOSLHC_PbPb",
  // Hijing
  "Hijing",
  "Hijing_QA001",
  "Hijing_Rsn002a", "Hijing_Rsn002b", "Hijing_Rsn002c",
  "Hijing_Jpsiee001",
  "Hijing_Nuclex001", "Hijing_Nuclex003", "Hijing_Nuclex004",
  "Hijing_Jets001", "Hijing_Jets001a", "Hijing_Jets001b", "Hijing_Jets001c", "Hijing_Jets001d", "Hijing_Jets001e",
  "Hijing_Gamma001",
  "Hijing_HFhad001", "Hijing_HFele001",
  "Hijing_Str001a", "Hijing_Str001b", "Hijing_Str001c", 
  // Starlight
  "Starlight",
  // AMPT
  "AMPT",
  //
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

enum EPythia6Heavy_t {
  kPythia6Heavy_Charm,
  kPythia6Heavy_Beauty,
  kPythia6Heavy_Hadrons,
  kPythia6Heavy_Electron
};

/*****************************************************************/
/*****************************************************************/
/*****************************************************************/

// functions
AliGenerator *GeneratorCocktail(TString name);
AliGenerator *GeneratorInjector(Int_t ninj, Int_t pdg, Float_t ptmin, Float_t ptmax, Float_t ymin, Float_t ymax, Float_t phimin = 0., Float_t phimax = 360.); 
AliGenerator *GeneratorPythia6(Int_t tune = 0, Int_t pdgtrig = 0, Float_t etatrig = 1.2);
AliGenerator *GeneratorPythia6Heavy(Int_t process, Int_t decay, Int_t tune = 0);
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

    // Pythia6 (Perugia2011) - Nuclex001
  case kGeneratorPythia6_Perugia2011_Nuclex001:
    AliGenCocktail *ctl   = GeneratorCocktail("Perugia2011_Nuclex001");
    AliGenerator   *pyt   = GeneratorPythia6(kPythia6Tune_Perugia2011);
    ctl->AddGenerator(pyt,  "Pythia6", 1.);
    AliGenerator   *nu1a  = Generator_Nuclex(0xF, kFALSE, 10);
    AliGenerator   *nu1b  = Generator_Nuclex(0xF, kTRUE, 10);
    AliGenerator   *nu2a  = Generator_Nuclex(0x10, kFALSE, 40);
    AliGenerator   *nu2b  = Generator_Nuclex(0x10, kTRUE, 40);
    AliGenerator   *nu3a  = Generator_Nuclex(0xFE0, kFALSE, 20);
    AliGenerator   *nu3b  = Generator_Nuclex(0xFE0, kTRUE, 20);
    ctl->AddGenerator(nu1a,  "Nuclex1a", 1.);
    ctl->AddGenerator(nu1b,  "Nuclex1b", 1.);
    ctl->AddGenerator(nu2a,  "Nuclex2a", 1.);
    ctl->AddGenerator(nu2b,  "Nuclex2b", 1.);
    ctl->AddGenerator(nu3a,  "Nuclex3a", 1.);
    ctl->AddGenerator(nu3b,  "Nuclex3b", 1.);
    gen = ctl;
    break;
    
    // Pythia6 (Perugia2011) - Nuclex002
  case kGeneratorPythia6_Perugia2011_Nuclex002:
    AliGenCocktail *ctl   = GeneratorCocktail("Perugia2011_Nuclex002");
    AliGenerator   *pyt   = GeneratorPythia6(kPythia6Tune_Perugia2011);
    ctl->AddGenerator(pyt,  "Pythia6", 1.);
    AliGenerator   *nu1a  = Generator_Nuclex(0x1F, kFALSE, 10);
    AliGenerator   *nu1b  = Generator_Nuclex(0x1F, kTRUE, 10);
    ctl->AddGenerator(nu1a,  "Nuclex1a", 1.);
    ctl->AddGenerator(nu1b,  "Nuclex1b", 1.);
    gen = ctl;
    break;

    // Pythia6 - HF001
  case kGeneratorPythia6_Perugia2011_HFhad001:
  case kGeneratorPythia6_Perugia2011_HFele001:
    AliGenCocktail *ctl  = GeneratorCocktail("Perugia2011_HF");
    //
    Int_t process[2] = {kPythia6Heavy_Charm, kPythia6Heavy_Beauty};
    Int_t decay[2]   = {kPythia6Heavy_Hadrons, kPythia6Heavy_Electron};
    const Char_t *label[2][2] = {
      "chadr PYTHIA", "cele PYTHIA",
      "bchadr PYTHIA", "bele PYTHIA"
    };
    Int_t iprocess = uidConfig % 2;
    Int_t idecay   = tag - kGeneratorPythia6_Perugia2011_HFhad001;
    AliGenerator *phf  = GeneratorPythia6Heavy(process[iprocess], decay[idecay], kPythia6Tune_Perugia2011);
    ctl->AddGenerator(phf, label[iprocess][idecay], 1.);
    printf(">>>>> added HF generator %s \n", label[iprocess][idecay]);
    // add pi0 and eta enhancement
    if (tag == kGeneratorPythia6_Perugia2011_HFele001) {
      AliGenPHOSlib *plib = new AliGenPHOSlib();
      AliGenParam *pi0 = new AliGenParam(2, plib, AliGenPHOSlib::kPi0Flat);
      pi0->SetPhiRange(0., 360.) ;
      pi0->SetYRange(-1.2, 1.2) ;
      pi0->SetPtRange(0., 50.) ;
      ctl->AddGenerator(pi0,  "pi0", 1.);
      AliGenParam *eta = new AliGenParam(1, plib, AliGenPHOSlib::kEtaFlat);
      eta->SetPhiRange(0., 360.) ;
      eta->SetYRange(-1.2, 1.2) ;
      eta->SetPtRange(0., 50.) ;
      ctl->AddGenerator(eta,  "eta", 1.);
    }
    gen = ctl;
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
    
    // Pythia8 (Monash2013) - Str002
  case kGeneratorPythia8_Monash2013_Str002:
    // triggered particles
    Int_t pdglist[] = {310, 3122, 3312, 3334, 310, -3122, -3312, -3334};
    Int_t pdg = pdglist[uidConfig % (sizeof(pdglist) / 4)]; // select according to unique ID
    AliGenerator   *py8 = GeneratorPythia8(kPythia8Tune_Monash2013, pdg, 1.2);
    //
    gen = py8;
    break;
    
    // Pythia8Jets (Monash2013)
  case kGeneratorPythia8Jets: 
  case kGeneratorPythia8Jets_Monash2013:
    gen = GeneratorPythia8Jets(kPythia8Tune_Monash2013);
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

    // AMPT
  case kGeneratorAMPT:
    gen = GeneratorAMPT();
    break;

    // Hijing - QA001
  case kGeneratorHijing_QA001:
    AliGenCocktail *ctl    = GeneratorCocktail("Hijing_QA001");
    AliGenerator   *hij    = GeneratorHijing();
    AliGenerator   *injpip = GeneratorInjector(10,  211, 10., 110., -1.0, 1.0);
    AliGenerator   *injpim = GeneratorInjector(10, -211, 10., 110., -1.0, 1.0);
    AliGenerator   *injkap = GeneratorInjector(5,   321, 10., 110., -1.0, 1.0);
    AliGenerator   *injkam = GeneratorInjector(5,  -321, 10., 110., -1.0, 1.0);
    AliGenerator   *injprp = GeneratorInjector(1,  2212, 10., 110., -1.0, 1.0);
    AliGenerator   *injprm = GeneratorInjector(1, -2212, 10., 110., -1.0, 1.0);
    ctl->AddGenerator(hij,    "Hijing",         1.);
    ctl->AddGenerator(injpip, "Injector (pip)", 1.);
    ctl->AddGenerator(injpim, "Injector (pim)", 1.);
    ctl->AddGenerator(injkap, "Injector (kap)", 1.);
    ctl->AddGenerator(injkam, "Injector (kam)", 1.);
    ctl->AddGenerator(injprp, "Injector (prp)", 1.);
    ctl->AddGenerator(injprm, "Injector (prm)", 1.);
    gen = ctl;
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
    AliGenerator   *nu1a  = Generator_Nuclex(0xF, kFALSE, 10);
    AliGenerator   *nu1b  = Generator_Nuclex(0xF, kTRUE, 10);
    AliGenerator   *nu2a  = Generator_Nuclex(0x10, kFALSE, 40);
    AliGenerator   *nu2b  = Generator_Nuclex(0x10, kTRUE, 40);
    AliGenerator   *nu3a  = Generator_Nuclex(0xFE0, kFALSE, 20);
    AliGenerator   *nu3b  = Generator_Nuclex(0xFE0, kTRUE, 20);
    ctl->AddGenerator(nu1a,  "Nuclex1a", 1.);
    ctl->AddGenerator(nu1b,  "Nuclex1b", 1.);
    ctl->AddGenerator(nu2a,  "Nuclex2a", 1.);
    ctl->AddGenerator(nu2b,  "Nuclex2b", 1.);
    ctl->AddGenerator(nu3a,  "Nuclex3a", 1.);
    ctl->AddGenerator(nu3b,  "Nuclex3b", 1.);
    gen = ctl;
    break;
    
    // Hijing - Nuclex003
  case kGeneratorHijing_Nuclex003:
    AliGenCocktail *ctl   = GeneratorCocktail("Hijing_Nuclex003");
    AliGenerator   *hij   = GeneratorHijing();
    ctl->AddGenerator(hij,  "Hijing", 1.);
    AliGenerator   *nu1a  = Generator_Nuclex(0xF, kFALSE, 10);
    AliGenerator   *nu1b  = Generator_Nuclex(0xF, kTRUE, 10);
    AliGenerator   *nu2a  = Generator_Nuclex(0x10, kFALSE, 40);
    AliGenerator   *nu2b  = Generator_Nuclex(0x10, kTRUE, 40);
    AliGenerator   *nu3a  = Generator_Nuclex(0xFFE0, kFALSE, 20);
    AliGenerator   *nu3b  = Generator_Nuclex(0xFFE0, kTRUE, 20);
    ctl->AddGenerator(nu1a,  "Nuclex1a", 1.);
    ctl->AddGenerator(nu1b,  "Nuclex1b", 1.);
    ctl->AddGenerator(nu2a,  "Nuclex2a", 1.);
    ctl->AddGenerator(nu2b,  "Nuclex2b", 1.);
    ctl->AddGenerator(nu3a,  "Nuclex3a", 1.);
    ctl->AddGenerator(nu3b,  "Nuclex3b", 1.);
    gen = ctl;
    break;
    
    // Hijing - Nuclex004
  case kGeneratorHijing_Nuclex004:
    AliGenCocktail *ctl   = GeneratorCocktail("Hijing_Nuclex004");
    AliGenerator   *hij   = GeneratorHijing();
    ctl->AddGenerator(hij,  "Hijing", 1.);
    AliGenerator   *nu1a  = Generator_Nuclex(0xF, kFALSE, 10);
    AliGenerator   *nu1b  = Generator_Nuclex(0xF, kTRUE, 10);
    AliGenerator   *nu2a  = Generator_Nuclex(0x10, kFALSE, 40);
    AliGenerator   *nu2b  = Generator_Nuclex(0x10, kTRUE, 40);
    AliGenerator   *nu3a  = Generator_Nuclex(0xC000, kFALSE, 20);
    AliGenerator   *nu3b  = Generator_Nuclex(0xC000, kTRUE, 20);
    ctl->AddGenerator(nu1a,  "Nuclex1a", 1.);
    ctl->AddGenerator(nu1b,  "Nuclex1b", 1.);
    ctl->AddGenerator(nu2a,  "Nuclex2a", 1.);
    ctl->AddGenerator(nu2b,  "Nuclex2b", 1.);
    ctl->AddGenerator(nu3a,  "Nuclex3a", 1.);
    ctl->AddGenerator(nu3b,  "Nuclex3b", 1.);
    gen = ctl;
    break;
    
    // Hijing - Jets001
  case kGeneratorHijing_Jets001:
  case kGeneratorHijing_Jets001a:
  case kGeneratorHijing_Jets001b:
  case kGeneratorHijing_Jets001c:
  case kGeneratorHijing_Jets001d:
  case kGeneratorHijing_Jets001e:
    Int_t ninjlist[6] = {1, 10, 5, 3, 2, 1};
    Int_t ninj = ninjlist[tag - kGeneratorHijing_Jets001];
    AliGenCocktail *ctl   = GeneratorCocktail("Hijing_Jets001");
    AliGenerator   *hij   = GeneratorHijing();
    ctl->AddGenerator(hij,  "Hijing", 1.);
    AliGenerator   *jet   = GeneratorPythia8Jets();
    ctl->AddGenerator(jet,  "Jets", 1., new TFormula(Form("ninj_%d", ninj), Form("%d", ninj)));
    gen = ctl;
    break;
    
    // Hijing - Gamma001
  case kGeneratorHijing_Gamma001:
    AliGenCocktail *ctl  = GeneratorCocktail("Hijing_Gamma001");
    AliGenerator   *hij  = GeneratorHijing();
    ctl->AddGenerator(hij,  "Hijing",         1.);
    // PCM
    TFormula* neutralsF  = new TFormula("neutrals",  "30. + 30. * exp(- 0.5 * x * x / 5.12 / 5.12)");
    AliGenerator   *pi0  = GeneratorInjector(1, 111, 0., 50., -1.2, 1.2);
    AliGenerator   *eta  = GeneratorInjector(1, 221, 0., 50., -1.2, 1.2);
    ctl->AddGenerator(pi0,  "Injector (pi0)", 1., neutralsF);
    ctl->AddGenerator(eta,  "Injector (eta)", 1., neutralsF);
    // PHOS
    AliGenerator   *pi0a = GeneratorInjector(1, 111, 0., 50., -0.155, 0.155, 240., 260.);
    AliGenerator   *pi0b = GeneratorInjector(1, 111, 0., 50., -0.155, 0.155, 260., 280.);
    AliGenerator   *pi0c = GeneratorInjector(1, 111, 0., 50., -0.155, 0.155, 280., 300.);
    AliGenerator   *pi0d = GeneratorInjector(1, 111, 0., 50., -0.155, 0.155, 300., 320.);
    AliGenerator   *etaa = GeneratorInjector(1, 221, 0., 50., -0.155, 0.155, 240., 320.);
    ctl->AddGenerator(pi0a, "Injector (pi0a)", 1.);
    ctl->AddGenerator(pi0b, "Injector (pi0b)", 1.);
    ctl->AddGenerator(pi0c, "Injector (pi0c)", 1.);
    ctl->AddGenerator(pi0d, "Injector (pi0d)", 1.);
    ctl->AddGenerator(etaa, "Injector (etaa)", 1.);
    gen = ctl;
    break;

    // Hijing - HFhad001
  case kGeneratorHijing_HFhad001:
  case kGeneratorHijing_HFele001:
    AliGenCocktail *ctl  = GeneratorCocktail("Hijing_HF");
    AliGenerator   *hij  = GeneratorHijing();
    ctl->AddGenerator(hij, "Hijing", 1.);
    //
    Int_t process[2] = {kPythia6Heavy_Charm, kPythia6Heavy_Beauty};
    Int_t decay[2]   = {kPythia6Heavy_Hadrons, kPythia6Heavy_Electron};
    const Char_t *label[2][2] = {
      "chadr PYTHIA", "cele PYTHIA",
      "bchadr PYTHIA", "bele PYTHIA"
    };
    TFormula *formula = new TFormula("Signals", "max(1.,120.*(x<5.)+80.*(1.-x/20.)*(x>5.)*(x<11.)+240.*(1.-x/13.)*(x>11.))");
    Int_t iprocess = uidConfig % 2;
    Int_t idecay   = tag - kGeneratorHijing_HFhad001;
    AliGenerator *phf  = GeneratorPythia6Heavy(process[iprocess], decay[idecay], kPythia6Tune_Perugia2011);
    //
    Float_t pth[4] = {2.76, 20., 50., 1000.};
    Int_t ipt;
    if      ((uidConfig / 2) % 10 < 7) ipt = 0;
    else if ((uidConfig / 2) % 10 < 9) ipt = 1;
    else                               ipt = 2;
    ((AliGenPythia *)phf)->SetPtHard(pth[ipt], pth[ipt + 1]);
    ctl->AddGenerator(phf, label[iprocess][idecay], 1., formula);
    printf(">>>>> added HF generator %s \n", label[iprocess][idecay]);
    // add pi0 and eta enhancement
    if (tag == kGeneratorHijing_HFele001) {
      TFormula* neutralsF = new TFormula("neutrals", "20.+ 80. * exp(- 0.5 * x * x / 5.12 / 5.12)");
      AliGenPHOSlib *plib = new AliGenPHOSlib();
      AliGenParam *pi0 = new AliGenParam(1, plib, AliGenPHOSlib::kPi0Flat);
      pi0->SetPhiRange(0., 360.) ;
      pi0->SetYRange(-1.2, 1.2) ;
      pi0->SetPtRange(0., 50.) ;
      ctl->AddGenerator(pi0,  "pi0", 1., neutralsF);
      AliGenParam *eta = new AliGenParam(1, plib, AliGenPHOSlib::kEtaFlat);
      eta->SetPhiRange(0., 360.) ;
      eta->SetYRange(-1.2, 1.2) ;
      eta->SetPtRange(0., 50.) ;
      ctl->AddGenerator(eta,  "eta", 1., neutralsF);
    }
    gen = ctl;
    break;

    // Hijing - Str001
  case kGeneratorHijing_Str001a:
  case kGeneratorHijing_Str001b:
  case kGeneratorHijing_Str001c:
    Int_t ninjk0[3] = {10, 5, 1};
    Int_t ninjla[3] = {10, 5, 1};
    Int_t ninjxi[3] = {20, 8, 1};
    Int_t ninjom[3] = {17, 4, 1};
    Int_t iinj = tag - kGeneratorHijing_Str001a;
    Int_t sign = uidConfig % 2 == 0 ? 1 : -1;
    AliGenCocktail *ctl  = GeneratorCocktail("Hijing_Str001");
    AliGenerator   *hij  = GeneratorHijing();
    AliGenerator   *ik0 = GeneratorInjector(ninjk0[iinj],         310, 0., 20., -0.7, 0.7);
    AliGenerator   *ila = GeneratorInjector(ninjla[iinj], sign * 3122, 0., 20., -0.7, 0.7);
    AliGenerator   *ixi = GeneratorInjector(ninjxi[iinj], sign * 3312, 0., 12., -0.7, 0.7);
    AliGenerator   *iom = GeneratorInjector(ninjom[iinj], sign * 3334, 0., 10., -0.7, 0.7);
    ctl->AddGenerator(hij,  "Hijing",            1.);
    ctl->AddGenerator(ik0, "Injector (K0s)", 1.);
    ctl->AddGenerator(ila, "Injector (Lambda)", 1.);
    ctl->AddGenerator(ixi, "Injector (Xi)", 1.);
    ctl->AddGenerator(iom, "Injector (Omega)", 1.);
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
GeneratorPythia6Heavy(Int_t process, Int_t decay, Int_t tune)
{
  //
  //
  comment = comment.Append(Form(" | Pythia6 heavy (%d, %d)", process, decay));
  //
  // set external decayer
  if (decay == kPythia6Heavy_Electron) {
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
  case kPythia6Heavy_Charm:
    pythia->SetProcess(kPyCharmppMNRwmi);
    break;
  case kPythia6Heavy_Beauty:
    pythia->SetProcess(kPyBeautyppMNRwmi);
    break;
  }
  //
  // heavy decay
  switch (decay) {
  case kPythia6Heavy_Hadrons:
    pythia->SetHeavyQuarkYRange(-1.5, 1.5);
    pythia->SetForceDecay(kHadronicDWithout4Bodies);
    break;
  case kPythia6Heavy_Electron:
    pythia->SetCutOnChild(1);
    pythia->SetPdgCodeParticleforAcceptanceCut(11);
    pythia->SetChildYRange(-1.2, 1.2);
    pythia->SetChildPtRange(0, 10000.);
    break;
  }
  //  
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
    (AliPythia8::Instance())->ReadString(Form("Tune:pp = %d", tune));
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

  Int_t pdgcodes[16] = {
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
    1010020040
  };

  const Char_t *names[16] = {
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
    "Hyper-Helium-4" // 0x8000
  };

  for (Int_t ipart = 0; ipart < 16; ipart++) {
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
