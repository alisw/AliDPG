#if !defined(__CINT__) || defined(__MAKECINT__)
//For ROOT6
R__ADD_INCLUDE_PATH($ALIDPG_ROOT)
#include "Utils/GetTriggerBCMaskAndMu.C"
#endif

AliGenerator *GeneratorCustom()
{

#if defined(__CINT__)
  // For ROOT5
  gROOT->LoadMacro("$ALIDPG_ROOT/Utils/GetTriggerBCMaskAndMu.C++");
#endif

  Double_t mu = 0.001;
  TString bcm = GetTriggerBCMaskAndMu(mu);

  //printf("   BC mask = %s\n",bcm.Data());
  //printf("   mu = %f\n",mu);
  //printf("   Energy = %f\n",energyConfig);

  // Set the pileup interaction generator
  // The second argument is the pileup rate
  // in terms of event rate per bunch crossing
  AliGenCocktail *ctl = (AliGenCocktail *)GeneratorCocktail("Hijing_muB_pileup_002");

  const Int_t pdgcodes[3] = {
      2212,
      1000010020,
      1000020030};

  const Char_t *names[3] = {
      "Proton",
      "Deuteron"
      "He3"};

  int npart[3] = {40, 10, 10};

  double maxpt[3] = {3.5, 10.0, 10.0}; /// large pt range for Lambda for the mass measurement

  for (Int_t ipart = 0; ipart < 3; ++ipart)
  {
    for (Int_t negative = 0; negative < 2; negative++)
    {

      AliGenBox *box = new AliGenBox(npart[ipart]);
      Int_t pdg = pdgcodes[ipart];
      if (negative)
        pdg = -pdg;
      box->SetPart(pdg);
      box->SetPtRange(0.2, maxpt[ipart]);
      box->SetPhiRange(0., 360.);
      box->SetYRange(-1., 1.);
      ctl->AddGenerator(box, Form("%s%i", names[ipart], negative), 1);
    }
  }

  // this is the Hijing generator for the trigger event,
  // which could contain an impact parameter cut
  TString simulation = gSystem->Getenv("CONFIG_SIMULATION");
  if (!simulation.Contains("Embed"))
  {
    AliGenHijing *hijingC = (AliGenHijing *)GeneratorHijing();

    AliGenPileup *genpil = new AliGenPileup();
    // this is the Hijing generator for the pileup events,
    // which is configured wihtout impact parameter cuts
    AliGenHijing *hijing = (AliGenHijing *)GeneratorHijing();
    hijing->SetImpactParameterRange(0., 15.);
    genpil->SetGenerator(hijing, mu);
    // Set the beam time structure
    // Details on the syntax in STEER/AliTriggerBCMask
    genpil->SetBCMask(bcm.Data());
    // Do not generate the trigger interaction!
    genpil->GenerateTrigInteraction(kFALSE);
    genpil->Print();

    ctl->AddGenerator(genpil, "Pileup", 1.);
    ctl->AddGenerator(hijingC, "Hijing", 1.);
  }

  return ctl;
}
