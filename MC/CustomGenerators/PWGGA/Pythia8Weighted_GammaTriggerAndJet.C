///
/// \file Pythia8Weighted_GammaTriggerAndJet.C
/// \brief Configuration of gamma-jet and jet-jet with/out decay gamma trigger
///
/// Generate PYTHIA8 gamma-jet (kPyDirectGamma) or jet-jet (kPyJets), with or without 
/// triggering the gamma in one of the calorimeter possible acceptances 
/// with a weight in the parton generatio to make the pT-hard distribution flatter.
/// Copy of Pythia8Weighted_GammaTriggerAndJet.C.
/// Options are:
/// * process and trigger :
///   * Pythia8Jets: jet-jet events, kPyJets (optionally with jet axis restricted to some zone or not, preferred open)
///   * Pythia8GammaJet: gamma-jet events, kPyDirectGamma, with gamma restricted to a calorimeter acceptance.
///   * Pythia8JetsGammaTrg: jet-jet events, kPyJets, with a decay gamma to a calorimeter acceptance above a given pT.
///
/// * acceptance of photon :
///   * FullDetector: Minimal acceptance cuts
///   * EMCRun1 : EMCal |eta| < 0.70,  80 < phi < 180
///   * EMCRun2 : EMCal |eta| < 0.70,  80 < phi < 187
///   * PHSRun1 : PHOS  |eta| < 0.13, 260 < phi < 320
///   * PHSRun2 : PHOS  |eta| < 0.13, 250 < phi < 320
///   * PHSDMC  : DCal+PHOS |eta| < 0.7, 250 < phi < 327
///   * EMCPHSDMC: EMCal+DCal+PHOS |eta| < 0.7, 250 < phi < 327 & 80 < phi < 187
///   
/// This macro can only be used for AliRoot containing the setting of the weight, 
/// from v5-09-54m, v5-09-55b, v5-09-56f. On master from April 2021
 
///
/// Main configuration method 
///
AliGenerator * 
GeneratorCustom
(TString opt = "kFullDetector")
{
  // acceptance
  Int_t acceptance = kCalorimeterAcceptance_FullDetector;
  if (opt.EqualTo("FullDetector"))
    acceptance = kCalorimeterAcceptance_FullDetector;
  if (opt.EqualTo("EMCRun1"))
    acceptance = kCalorimeterAcceptance_EMCRun1;
  if (opt.EqualTo("PHSRun1"))
    acceptance = kCalorimeterAcceptance_PHSRun1;
  if (opt.EqualTo("EMCRun2"))
    acceptance = kCalorimeterAcceptance_EMCRun2;
  if (opt.EqualTo("PHSRun2"))
    acceptance = kCalorimeterAcceptance_PHSRun2;
  if (opt.EqualTo("PHSDMC"))
    acceptance = kCalorimeterAcceptance_PHSDMC;
  if (opt.EqualTo("EMCPHSDMC"))
     acceptance = kCalorimeterAcceptance_EMCPHSDMC;
  
  AliGenerator * gener = 0;
  
  // Select the process
  if (processConfig.EqualTo("Pythia8Jets"))         {
    gener = GeneratorPythia8Jets         (kPythia8Tune_Monash2013 , acceptance); }
  if (processConfig.EqualTo("Pythia8GammaJet"))     {
    gener = GeneratorPythia8GammaJet     (kPythia8Tune_Monash2013 , acceptance); 
    
    // Rely on AliGenPythia/Plus calorimeter default acceptances or set them here
    if ( acceptance == kCalorimeterAcceptance_EMCPHSDMC )
       ((AliGenPythiaPlus*) gener)->SetCheckBarrelCalos(kTRUE);  
  }
  if (processConfig.EqualTo("Pythia8JetsGammaTrg")) {
    gener = GeneratorPythia8JetsGammaTrg (kPythia8Tune_Monash2013 , acceptance); 
    // Rely on AliGenPythia/Plus calorimeter default acceptances or set them here
    if ( acceptance == kCalorimeterAcceptance_EMCPHSDMC )
       ((AliGenPythiaPlus*) gener)->SetDecayPhotonInBarrelCalos(kTRUE);
  }
  
  // Set the desired weight
  ((AliGenPythiaPlus*) gener)->SetWeightPower(4.);
  
  return gener;
}
