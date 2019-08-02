AliGenerator *GeneratorCustom(TString opt = "")
{ // to check: SetCutOChild ; deuteron ; decay table usage

  comment = comment.Append(" PbPb: Hjing2500 at 5.5 + ITS3 signals injected via PYTHIA, AliGenParams, AliGenBox");
  TString optList[5] = {"hadHF","extDecayTable","HFjet_cc","HFjet_bb","HFjet_lf"};

  // injection scheme:
  // 1) opt = hadHF. HIJING + AliGenBox HYPERNUCLEI + PYTHIA with D,Lc to hadronic channels (mainly for non-prompt Ds and non-prompt Lc studies) 
  //                20% (10% ccbar, 10% bbbar) ntimes PYTHIA w/o trigger particle, ntimes according to imp. par (<=60),  only HF signal saved ; 
  //                40% (20% ccbar, 20% bbbar) ntimes PYTHIA with Ds trigger, ntimes according to imp. par (<=60), only HF signal saved  ; 
  //                40% (20% ccbar, 20% bbbar) ntimes PYTHIA with Lc trigger, ntimes according to imp. par (<=60), only HF signal saved  ; 
  //                100% with hypernuclei and deuteron (10 per type) 
  //
  // 2) opt = extDecayTable.  HIJING + AliGenBox HYPERNUCLEI + AliGenParams for Lb,Lc,B0,B+,Xic+,D+,Ds + PYTHIA with Bs,Xic0,Xic+ trigger (assumed dedicated decay table on wn)
  //                          100% with Lb,Lc,B0,B+,Xic+,D+,Ds via AliGenParams, 30 (15+15 c.c.) per type. Decay: kBeautyUpgrade (Lb 50% in Lc pi, 50% LbX, Xic+->Xi X to be checked due to dedicated decayTable)       
  //                          33% ntimes PYTHIA bbbar with Bs trigger, Bs->Ds pi assuming dedicated decay table on wn, ntimes according to imp. par (<=60),  only HF signal saved ; 
  //                          33% ntimes PYTHIA (half bbbar, half ccbar) with Xic0 trigger, decaying according to dedicated decay table on wn, ntimes according to imp. par (<=60),  only HF signal saved ; 
  //                          33% ntimes PYTHIA (half bbbar, half ccbar) with Xic+ trigger, decaying according to dedicated decay table on wn, ntimes according to imp. par (<=60),  only HF signal saved ; 
  //                          100% with hypernuclei and deuteron (10 per type)
  // 3) opt =  HFjet_cc, HFjet_bb, HFjet_lf --> simulation for HF jets + LF jet for mistagging.
  //

  Int_t iMain = 0;
  for (Int_t iopt = 0; iopt < 5; iopt++){
    if (opt.Contains(optList[iopt])){
      iMain = iopt;
      printf("-- HF process = %s -- \n",opt.Data());
    }
  }
  TString strDecayTab="";
  if(iMain==1)strDecayTab="ExtDT";		
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
      ctl->AddGenerator(phf, Form("%s_%s",label[iprocess][0],strDecayTab.Data()), 1., formula,ntimes);    
      printf(">>>>> added generator for HF production:  %s , %d times \n", Form("%s_%s",label[iprocess][0],strDecayTab.Data()),ntimes);
    }
    else {
      phf=GeneratorPythia6Heavy(process[iprocess], kPythia6HeavyDecay_Hadrons,kPythia6Tune_Perugia2011, kTRUE);
      Int_t trigPart=((Int_t)(((randHF-0.2)/0.8)*4.))%4;// just a way to get an int from 0 to 3 --> overall 40% with Ds trigger an 40% with Lc trigger
      ((AliGenPythia *)phf)->SetTriggerParticle(particleTrigger[trigPart], 999, 999, -1, 1000);
      ((AliGenPythia *)phf)->SetTriggerY(1.2);
      ((AliGenPythia *)phf)->SetHeavyQuarkYRange(-1.5,1.5);
      //      ((AliGenPythia *)phf)->SetForceDecay(kHadronicDWithout4Bodies);// This includes Lc->L pi: also useful for displaced Lb, not needed implicit in kPythia6HeavyDecay_Hadrons 
      ctl->AddGenerator(phf, Form("%s_%s",label[iprocess][1+trigPart/2],strDecayTab.Data()), 1., formula,ntimes);    
      printf(">>>>> added generator for HF production:  %s , %d times \n", Form("%s_%s",label[iprocess][1+trigPart/2],strDecayTab.Data()),ntimes);
    }
  }
  else if(iMain==1){
       
    // Bs, Xic+, Xic0 injected via PYTHIA, requiring them as trigger particle. We assume that a dedicated decay table is 
    // present on the worker node, so that we are effectively injected the desired decay channels: this only to speed 
    //up the production, avoiding modifications to AliGenITSULib. Can be changed in the future.
    // N.B. cannot add them on the same job with PYTHIA6 --> inject them according to random number.
    // for Bs we use only the bbbar process
    
    const Char_t *label[2][3] = {
      {"ccbar_PYTHIA_Bs", "ccbar_PYTHIA_XicZero","ccbar_PYTHIA_XicPlus"},
      {"bbbar_PYTHIA_Bs", "bbbar_PYTHIA_XicZero","bbbar_PYTHIA_XicPlus"}
    };    
    Int_t particleTrigger[6]={531, -531, 4132, -4132, 4232, -4232};
    Float_t randHF;
    Int_t trigPart=(Int_t)gRandom->Uniform(0,6);
    if(trigPart<2)iprocess=1;
    
    AliGenerator *phf = GeneratorPythia6Heavy(process[iprocess], kPythia6HeavyDecay_Hadrons, kPythia6Tune_Perugia2011, kTRUE);// the decay type assures
    ((AliGenPythia *)phf)->SetTriggerParticle(particleTrigger[trigPart], 999, 999, -1, 1000);
    ((AliGenPythia *)phf)->SetTriggerY(1.2);
    ((AliGenPythia *)phf)->SetHeavyQuarkYRange(-1.5,1.5);
    ctl->AddGenerator(phf, Form("%s_%s",label[iprocess][trigPart/2],strDecayTab.Data()), 1., formula,ntimes);
    printf(">>>>> added generator for HF production:  %s , %d times \n", Form("%s_%s",label[iprocess][trigPart/2],strDecayTab.Data()),ntimes);


    TVirtualMCDecayer* decayer = new AliDecayerPythia();
    decayer->SetForceDecay(kAll);
    decayer->Init();
    gMC->SetExternalDecayer(decayer);  

    // Rare decays of Lc, Lb, Ds, excl B, Xi_c via AliGenParams
    
    //
    // Set pseudorapidity range from -1. to 1.
    // 
    Float_t thmin          = (180./TMath::Pi())*2.*atan(exp(-1.));  
    Float_t thmax          = (180./TMath::Pi())*2.*atan(exp( 1.));  
    
    AliGenParam *gen[14];
    UInt_t partId[7] = {AliGenITSULib::kLc,AliGenITSULib::kLb,AliGenITSULib::kXi_c,AliGenITSULib::kBplus, AliGenITSULib::kBzero, AliGenITSULib::kDs, AliGenITSULib::kDplus};  
    for(Int_t iPart=0; iPart<14 ; iPart++){
      if(iPart%2==0) gen[iPart] = new AliGenParam(15,new AliGenITSULib(),partId[iPart/2],"DIST");
      if(iPart%2==1) gen[iPart]= new AliGenParam(15,new AliGenITSULib(),-partId[iPart/2],"DIST");
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


