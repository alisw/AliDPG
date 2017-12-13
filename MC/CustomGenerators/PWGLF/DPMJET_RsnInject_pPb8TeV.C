
AliGenerator *GeneratorCustom(TString opt = "")
{
  // Custom generator to inject phi,K*0,A-K*0, K*+-, Lambda*, A-Lambda*, f0, f2 resonances 
  // Options:
  // pPb:  injection landau in pT up to  21 GeV/c; 3 particles per event, but one species per event;
  // rapidity range: -0.6 < y < 0.1 : Anchored to period LHC16r

  // Pbp: injection same as pPb, but rapidity range -0.1 < y < 0.6 : Anchored to period LHC16s

    
  Int_t ninj = 1; // 1 particle per species

  // Set pT range (pT_L, pT_H)
  Double_t  pT_L = gRandom->Landau(0.5,0.2); // Landau distribution with mu = 0.5 and sigma = 0.2

  for(int j=0; j<1; j++)
    {
      if(pT_L < 0. || pT_L > 21.) {
	pT_L = gRandom->Landau(0.5,0.2);     j--;
      }
    }

  Double_t  pT_H = pT_L + 0.0005;

 
  // Set Rapidity
  Double_t y_L = 0, y_H = 0;
  TString Select[2] = {"pPb","Pbp"};

  if(opt.EqualTo(Select[0])) { y_L = -0.6; y_H = 0.1; }      // LHC16r
  else if(opt.EqualTo(Select[1])) { y_L = -0.1; y_H = 0.6; } // LHC16s
  else return kFALSE;


  AliGenCocktail *ctl  = GeneratorCocktail("DPMJET_RsnInject");
  AliGenDPMjet *dpmjet  = (AliGenDPMjet*) GeneratorPhojet();
  ctl->AddGenerator(dpmjet,"Dpmjet",           1.);

  Double_t randm = gRandom->Rndm();

  if ( randm < 0.333) {
    AliGenerator *injphi_pPb     = GeneratorInjector(ninj, 333, pT_L, pT_H,  y_L, y_H); // Phi 
    AliGenerator *injkstar0_pPb  = GeneratorInjector(ninj, 313, pT_L, pT_H,  y_L, y_H); // K*0
    AliGenerator *injkstarp_pPb  = GeneratorInjector(ninj, 323, pT_L, pT_H,  y_L, y_H); // K*+

    ctl->AddGenerator(injphi_pPb,"Injector(RsnPhi)", 1.);
    ctl->AddGenerator(injkstar0_pPb,"Injector(Rsnkstar0)", 1.);
    ctl->AddGenerator(injkstarp_pPb,"Injector(Rsnkstarp)", 1.);
 }

  else if ( randm >= 0.333 && randm < 0.667) {
    AliGenerator *injlstar_pPb   = GeneratorInjector(ninj, 3124, pT_L, pT_H,  y_L, y_H); // Lambda* 
    AliGenerator *injakstar0_pPb = GeneratorInjector(ninj, -313, pT_L, pT_H,  y_L, y_H); // A-K*0
    AliGenerator *injkstarm_pPb  = GeneratorInjector(ninj, -323, pT_L, pT_H,  y_L, y_H); // K*-

    ctl->AddGenerator(injlstar_pPb,"Injector(Rsnlstar)", 1.);
    ctl->AddGenerator(injakstar0_pPb,"Injector(Rsnakstar0)", 1.);
    ctl->AddGenerator(injkstarm_pPb,"Injector(Rsnkstarm)", 1.);
  }

  else {
    AliGenerator *injalstar_pPb = GeneratorInjector(ninj,  -3124, pT_L, pT_H,  y_L, y_H); // A-Lambda* 
    AliGenerator *injf0_pPb     = GeneratorInjector(ninj,9010221, pT_L, pT_H,  y_L, y_H); // f0(980)
    AliGenerator *injf2_pPb     = GeneratorInjector(ninj,    225, pT_L, pT_H,  y_L, y_H); // f2(1270)

    ctl->AddGenerator(injalstar_pPb,"Injector(Rsnalstar)", 1.);
    ctl->AddGenerator(injf0_pPb,"Injector(Rsnf0)", 1.);
    ctl->AddGenerator(injf2_pPb,"Injector(Rsnf2)", 1.);
   }

  return ctl;

}

 
