AliGenerator* GeneratorCustom() 
{
  AliGenCocktail *gener = new AliGenCocktail();

  AliGenHijing *hijing = (AliGenHijing*)GeneratorHijing();
  hijing->SetImpactParameterRange(0., 100.);
  hijing->SetShadowing(0);
  hijing->SetNoElas(kTRUE);
  // Cocktail with slow nucleons generator DO NOT track spectators
  hijing->SetSpectators(0);

  AliCollisionGeometry* coll = hijing->CollisionGeometry();
  AliGenSlowNucleons* gray = new AliGenSlowNucleons(1);
  AliSlowNucleonModelExp* model = new AliSlowNucleonModelExp();
  model->SetSaturation(kTRUE);
  gray->SetSlowNucleonModel(model);
  gray->SetTarget(208,82);
  gray->SetThetaDist(1);
  if (systemConfig.EqualTo("p-Pb")){
    // in the MC configuration p-Pb has the p going towards positive z;
    // Pb coming from A side and slow nucleons detected on the C side
    // -->the slow nucleons should have negative pz component:
    // SetProtonDirection sets the sign of the momentum of the nucleons of Pb
    gray->SetProtonDirection(-1);
    printf("AliGenSlowNucleons::SetProtonDirection(-1)\n");
  }else if (systemConfig.EqualTo("Pb-p")){
    // in the MC configuration Pb-p has the p going towards negative z;
    // Pb coming from C side and slow nucleons detected on the A side
    // -->the slow nucleons should have positive pz component:
    // SetProtonDirection sets the sign of the momentum of the nucleons of Pb
    gray->SetProtonDirection(1);
    printf("AliGenSlowNucleons::SetProtonDirection(1)\n");
  }
  Double_t pBeamEnergy=energyConfig/2.*TMath::Sqrt(208./82.);
  printf("Proton beam energy = %f GeV\n",pBeamEnergy);
  gray->SetNominalCmsEnergy(2.*pBeamEnergy);
  gray->SetCollisionGeometry(coll);
  gray->SetTrackingFlag(1);

  gener->AddGenerator(hijing, "Hijing_pPb", 1);
  gener->AddGenerator(gray, "SNM", 1);
  gener->SetTrackingFlag(1);

  return gener;
}
