AliGenerator *GeneratorCustom(TString opt = "")
{ // to check: SetCutOChild ; deuteron ; decay table usage

  comment = comment.Append(" PbPb: Hjing2500 at 5.5 + ITS3 signals injected via PYTHIA, AliGenParams, AliGenBox");
  TString optList[5] = {"hadHF","HFspecificSignals","HFjet_cc","HFjet_bb","HFjet_lf"};

  // injection scheme:
  // 1) opt = hadHF. HIJING + AliGenBox HYPERNUCLEI + PYTHIA with D,Lc to hadronic channels (mainly for non-prompt Ds and non-prompt Lc studies) 
  //                20% (10% ccbar, 10% bbbar) ntimes PYTHIA w/o trigger particle, ntimes according to imp. par (<=60),  only HF signal saved ; 
  //                40% (20% ccbar, 20% bbbar) ntimes PYTHIA with Ds trigger, ntimes according to imp. par (<=60), only HF signal saved  ; 
  //                40% (20% ccbar, 20% bbbar) ntimes PYTHIA with Lc trigger, ntimes according to imp. par (<=60), only HF signal saved  ; 
  //                100% with hypernuclei and deuteron (10 per type), including c-deuteron (20 per event)
  //
  // 2) opt =     HIJING + AliGenBox HYPERNUCLEI + AliGenParams for Lb,Lc,B0,B+,Xic+,Ds+,D+, Bs,Xic0,OmegaC 
  //                          14 (7+7 c.c.) per type. Decay: kBeautyUpgrade        //                          
  //                          100% with hypernuclei and deuteron (10 per type), including c-deuteron
  // 3) opt =  HFjet_cc, HFjet_bb, HFjet_lf --> simulation for HF jets + LF jet for mistagging.
  //

  Int_t iMain = 0;
  for (Int_t iopt = 0; iopt < 5; iopt++){
    if (opt.Contains(optList[iopt])){
      iMain = iopt;
      printf("-- HF process = %s -- \n",opt.Data());
      break;// needed otherwise opt extDecayTable overrides extDecayTableAllParams
    }
  }
  // HIJING EVENT	
  AliGenCocktail *ctl  = static_cast<AliGenCocktail*>(GeneratorCocktail("Hijing_HF")); 
  TString simulation = gSystem->Getenv("CONFIG_SIMULATION");
  Int_t ntimes=1;
  
  Double_t bEvalFormula=-1;
  Bool_t isEmbedding=kFALSE;
  if(!simulation.Contains("Embed")){
    AliGenerator *hij = GeneratorHijing();
    ctl->AddGenerator(hij, "Hijing", 1.);
  }
  else{
    isEmbedding=kTRUE;
    
    if(bminConfig>bmaxConfig)Fatal("Hijing_HF001_MCtoMCembedding","max impact parameter smaller than min impact parameter");
    if(bminConfig<0.)Fatal("Hijing_HF001_MCtoMCembedding","min impact parameter <0");
    if(bmaxConfig>20)Fatal("Hijing_HF001_MCtoMCembedding","unphysical impact parameter");
    bEvalFormula=(bmaxConfig+bminConfig)/2.;
  }
  

  TFormula* formula = new TFormula("Signals","60. * (x < 5.) + 80. *(1. - x/20.)*(x>5.)");
  if(isEmbedding){
    ntimes=formula->Eval(bEvalFormula);
    delete formula; formula=0x0;
  }// needed only later for PYTHIA injection part
  //
  Int_t iprocess= uidConfig %2;// used just as a random number

  Int_t process[2] = {kPythia6HeavyProcess_Charm, kPythia6HeavyProcess_Beauty};
  Int_t decay[3]   = {kPythia6HeavyDecay_Hadrons, kPythia6HeavyDecay_All, kBeautyUpgrade};

  if(iMain==0){
    Int_t particleTrigger[4]={431, -431, 4122, -4122};
    const Char_t *label[2][3] = {
      {"ccbar_PYTHIA", "ccbar_PYTHIA_DsTrig","ccbar_PYTHIA_LcTrig"},
      {"bbbar_PYTHIA", "bbbar_PYTHIA_DsTrig","bbbar_PYTHIA_LcTrig"}
    };    

    Float_t randHF=gRandom->Rndm();
  
    AliGenerator *phf;
    if(randHF<0.2){// 20% of jobs with PYTHIA (n.b. 10% ccbar, 10%bbbar) w/o trigger particle
      phf=GeneratorPythia6Heavy(process[iprocess], kPythia6HeavyDecay_Hadrons,kPythia6Tune_Perugia2011, kTRUE);
      ctl->AddGenerator(phf, Form("%s",label[iprocess][0]), 1., formula,ntimes);    
      printf(">>>>> added generator for HF production:  %s , %d times \n", Form("%s",label[iprocess][0]),ntimes);
    }
    else {
      phf=GeneratorPythia6Heavy(process[iprocess], kPythia6HeavyDecay_Hadrons,kPythia6Tune_Perugia2011, kTRUE);
      Int_t trigPart=((Int_t)(((randHF-0.2)/0.8)*4.))%4;// just a way to get an int from 0 to 3 --> overall 40% with Ds trigger an 40% with Lc trigger
      ((AliGenPythia *)phf)->SetTriggerParticle(particleTrigger[trigPart], 999, 999, -1, 1000);
      ((AliGenPythia *)phf)->SetTriggerY(1.2);
      ((AliGenPythia *)phf)->SetHeavyQuarkYRange(-1.5,1.5);
      //      ((AliGenPythia *)phf)->SetForceDecay(kHadronicDWithout4Bodies);// This includes Lc->L pi: also useful for displaced Lb, not needed implicit in kPythia6HeavyDecay_Hadrons 
      ctl->AddGenerator(phf, Form("%s",label[iprocess][1+trigPart/2]), 1., formula,ntimes);    
      printf(">>>>> added generator for HF production:  %s , %d times \n", Form("%s",label[iprocess][1+trigPart/2]),ntimes);
    }
  }
  else if(iMain==1){
    TVirtualMCDecayer* decayer = new AliDecayerPythia();
      decayer->SetForceDecay(kAll);
      decayer->Init();
      gMC->SetExternalDecayer(decayer);  

    //
    // Set pseudorapidity range from -1. to 1. (used in AliGenParams Generators)
    // 
    Float_t thmin          = (180./TMath::Pi())*2.*atan(exp(-1.));  
    Float_t thmax          = (180./TMath::Pi())*2.*atan(exp( 1.));    
  
  
    // Rare decays of Lc, Lb, Ds, excl B, Xi_c via AliGenParams
    AliGenParam *gen[20];
    UInt_t partId[10] = {AliGenITSULib::kXi_czero,AliGenITSULib::kXi_c,AliGenITSULib::kOmega_c,AliGenITSULib::kBs,AliGenITSULib::kLc,AliGenITSULib::kLb,AliGenITSULib::kBplus,AliGenITSULib::kBzero,AliGenITSULib::kDs,AliGenITSULib::kDplus};  
    for(Int_t iPart=0; iPart<20 ; iPart++){
      if(iPart%2==0) gen[iPart] = new AliGenParam(7,new AliGenITSULib(),partId[iPart/2],"DIST");
      if(iPart%2==1) gen[iPart]= new AliGenParam(7,new AliGenITSULib(),-partId[iPart/2],"DIST");
      gen[iPart]->SetPtRange(0.,999.);
      gen[iPart]->SetPhiRange(0., 360.);
      gen[iPart]->SetYRange(-1.,1.);
      gen[iPart]->SetCutOnChild(1);
      gen[iPart]->SetChildThetaRange(thmin,thmax);
      gen[iPart]->SetSelectAll(kTRUE);
      gen[iPart]->SetForceDecay(kBeautyUpgrade);
      ctl->AddGenerator(gen[iPart], Form("Generator_%i_%i",partId[iPart/2],iPart%2), 1);
      printf(">>>>> added generator for HF production:  %s , 15 times \n", Form("Generator_%i_%i",partId[iPart/2],iPart%2));
    }
    


  }
  if(iMain<2){
  
  // Hypernuclei: 10 per type for 3LH, 4LH, 4LHe
  AliGenBox *pG1=new AliGenBox(5);
  pG1->SetPart(1010010030);
  pG1->SetPtRange(0,10);
  pG1->SetPhiRange(0,360);
  pG1->SetYRange(-1,1);
  ctl->AddGenerator(pG1,"g1",1);    

  AliGenBox *pG2=new AliGenBox(5);
  pG2->SetPart(-1010010030);
  pG2->SetPtRange(0,10);
  pG2->SetPhiRange(0,360);
  pG2->SetYRange(-1,1);
  ctl->AddGenerator(pG2,"g2",1);    

  AliGenBox *pG3=new AliGenBox(5);
  pG3->SetPart(1010010040);
  pG3->SetPtRange(0,10);
  pG3->SetPhiRange(0,360);
  pG3->SetYRange(-1,1);
  ctl->AddGenerator(pG3,"g3",1);    

  AliGenBox *pG4=new AliGenBox(5);
  pG4->SetPart(-1010010040);
  pG4->SetPtRange(0,10);
  pG4->SetPhiRange(0,360);
  pG4->SetYRange(-1,1);
  ctl->AddGenerator(pG4,"g4",1);    

  AliGenBox *pG5=new AliGenBox(5);
  pG5->SetPart(1010020040);
  pG5->SetPtRange(0,10);
  pG5->SetPhiRange(0,360);
  pG5->SetYRange(-1,1);
  ctl->AddGenerator(pG5,"g5",1);    

  AliGenBox *pG6=new AliGenBox(5);
  pG6->SetPart(-1010020040);
  pG6->SetPtRange(0,10);
  pG6->SetPhiRange(0,360);
  pG6->SetYRange(-1,1);
  ctl->AddGenerator(pG6,"g6",1);    

  AliGenBox *pG7=new AliGenBox(10); // c-deuteron
  pG7->SetPart(2010010020);
  pG7->SetPtRange(0,10);
  pG7->SetPhiRange(0,360);
  pG7->SetYRange(-1,1);
  ctl->AddGenerator(pG7,"g7",1);    

  AliGenBox *pG8=new AliGenBox(10);// anti c-deuteron
  pG8->SetPart(-2010010020);
  pG8->SetPtRange(0,10);
  pG8->SetPhiRange(0,360);
  pG8->SetYRange(-1,1);
  ctl->AddGenerator(pG8,"g8",1);    

AliGenBox *pG9=new AliGenBox(5); // deuteron (for c-deuteron background)
  pG9->SetPart(1000010020);
  pG9->SetPtRange(0,10);
  pG9->SetPhiRange(0,360);
  pG9->SetYRange(-1,1);
  ctl->AddGenerator(pG9,"g9",1);    

  AliGenBox *pG10=new AliGenBox(5);// anti-deuteron
  pG10->SetPart(-1000010020);
  pG10->SetPtRange(0,10);
  pG10->SetPhiRange(0,360);
  pG10->SetYRange(-1,1);
  ctl->AddGenerator(pG10,"g10",1);    


  }
  else{
    
    AliGenerator *phf;
    // selezione HF jet type
    if(iMain==2){
      for(Int_t k=0;k<10;k++){
	TString label(Form("ccbar_PYTHIA_pthard%.1fto%.1f",pthardminConfig,pthardmaxConfig));
	phf = GeneratorPythia6Heavy(process[0], kPythia6HeavyDecay_All, kPythia6Tune_Perugia2011, kFALSE);// no decay forced
	((AliGenPythia *)phf)->SetHeavyQuarkYRange(-1.5,1.5);
	ctl->AddGenerator(phf, label.Data(), 1.);
	printf(">>>>> added generator for HF production:  %s \n", label.Data());
      }
    }
    else if(iMain==3){
      for(Int_t k=0;k<10;k++){
	TString label(Form("bbbar_PYTHIA_pthard%.1fto%.1f",pthardminConfig,pthardmaxConfig));
	phf= GeneratorPythia6Heavy(process[1], kPythia6HeavyDecay_All, kPythia6Tune_Perugia2011, kFALSE);// no decay forced
	((AliGenPythia *)phf)->SetHeavyQuarkYRange(-1.5,1.5);
	ctl->AddGenerator(phf, label.Data(), 1.);
	printf(">>>>> added generator for HF production:  %s \n", label.Data());
      }
    }
    else if(iMain==4){
      TString label(Form("jetjet_PYTHIA_pthard%.1fto%.1f",pthardminConfig,pthardmaxConfig));
      phf  = GeneratorPythia6Jets(kPythia6Tune_Perugia2011);
      ctl->AddGenerator(phf, label.Data(), 1.);
    }
    else {
      Printf("Wrong option for HF Config");
    }
  }
  

  return ctl;
}
