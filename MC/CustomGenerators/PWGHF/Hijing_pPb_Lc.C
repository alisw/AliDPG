AliGenerator * GeneratorCustom(TString opt = "")
{
  Decay_t decay[2] = {AliDecayer::kLcpKpi,AliDecayer::kLcpK0S};
  Int_t idecay=0;
  const Char_t *label[2] = { "LcpKpi PYTHIA", "LcpK0S PYTHIA" } ;
  
  TString optList[2] = {"LcpKpi","LcpK0S"};
  for (Int_t iopt = 0; iopt < 2; iopt++){
    if (opt.EqualTo(optList[iopt])) idecay = iopt;
  }

  AliGenCocktail *ctl = GeneratorCocktail("Hijing_Lc");

  Float_t randHF = gRandom->Rndm();
  if (randHF>0.176797){ // add Hijing for p-Pb

  AliGenHijing *hij = GeneratorHijing();
 
  hij->SetSpectators(0);
  
  hij->SetDecaysOff(0);
  hij->SetSelectAll(1);

 hij->Init();
 //force decays
  THijing *th = hij->GetTHijing();
  
//  if (idecay == 0){ // pKpi 
//    
//     // Forbid all Lc decays that do not result in pKpi
//     for (Int_t d=927; d<=940; d++){ 
//      th->SetMDME(d,1,0);
//    }
//    //decay 941 - Delta++ + K-
//    for(Int_t d=942; d<=943; d++){ 
//      th->SetMDME(d,1,0);
//    }
//    //decay 944 - K* + p 
//     //decay 945 - p + K + pi
//    for(Int_t d=946; d<=949; d++){ 
//      th->SetMDME(d,1,0);
//    }
//    //decay 950 - Lambda1520 + pi+
//    for(Int_t d=951; d<=1004; d++){ 
//      th->SetMDME(d,1,0);
//    }
// }  else if (idecay == 1) { // pK0S
// 
//    // Forbid all Lc decays except Lc->Kbar0 + p ====>>> HIJING
//
//    for(Int_t d=927; d<=942; d++){ 
//      th->SetMDME(d,1,0);
//    }
//    // decay 943 is Lc->Kbar0 + p
//    for(Int_t d=944; d<=1004; d++){ 
//      th->SetMDME(d,1,0);
//    }
// } 
 ctl->AddGenerator(hij, "Hijing",1.);
}

  AliGenPythia* pyth = GeneratorPythia6(kPythia6Tune_Perugia2011);
  pyth->SetProcess(kPyCharmppMNRwmi);
  pyth->SetTriggerParticle(4122);
  pyth->SetHeavyQuarkYRange(-1.5,1.5);
  pyth->SetForceDecay(decay[idecay]);  //Force Lc decay mode in PYTHIA
  pyth->SetPtHard(pthardminConfig,pthardmaxConfig);
  pyth->SetStackFillOpt(AliGenPythia::kHeavyFlavor);
  ctl->AddGenerator(pyth, "Lc PYTHIA", 1.);  
  return ctl;
}

