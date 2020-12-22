AliGenerator *GeneratorCustom(TString opt = "")
{

  AliGenCocktail *ctl  = (AliGenCocktail*) GeneratorCocktail("Hijing_Str_pbpb2010");

  //underlying hijing generator
  AliGenerator *hij = GeneratorHijing();
  ctl->AddGenerator(hij, "Hijing", 1.);

  //define and add signal generators
  // "a" option is large stat, mild inj., rapidity-flat
  // "b" option is limited stat, heavy inj., rapidity flat
  // "c" option is limited stat, heavy inj., theta flat (bugged one)
  
  int ninj[2][7] = {{1,1,1,5,5,2,2},{0,0,0,30,30,30,30}};  //2 inj. strategies, 7 particles
  float ptrange[7][2] = {{4,12},{4,12},{4,12},{0.5,12},{0.5,12},{0.5,10},{0.5,10}};  //7 particles, low and high limits of pT injection
  int pdgcodes[7] = {310,3122,-3122,3312,-3312,3334,-3334}; //kk0s, klam, kalam, kxim, kxip, komm, komp
  int iinj = 0, startpart=0;
  if(opt.EqualTo("b")||opt.EqualTo("c")) {
      iinj = 1; 
      startpart = 3;
  }
  
  AliGenBox *injgen[7];
  for(int i=startpart; i<7; i++) {
    injgen[i] = new AliGenBox(ninj[iinj][i]);
    injgen[i]->SetPart(pdgcodes[i]);
    injgen[i]->SetPtRange(ptrange[i][0],ptrange[i][1]);
    if(opt.EqualTo("a")||opt.EqualTo("b")) injgen[i]->SetYRange(-0.6,0.6);
    else injgen[i]->SetThetaRange(45,135);//this defines theta ranges, but also sets the bit kThetaRange which in AliGenBox sets flat theta
    ctl->AddGenerator(injgen[i], Form("Injector pdg:%d",pdgcodes[i]), 1.);
  }

  return ctl;
}
