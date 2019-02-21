void AMPTstandalone(){

  Printf("Running AMPT in standalone...");

  //====================================================================
  //configuration reading (needed here, since Config.C not yet called)
  static TString processConfigAMPT   = "";        // process
  static TString systemConfigAMPT    = "";        // system
  static Float_t bminConfigAMPT      = 0.;        // impact parameter min
  static Float_t bmaxConfigAMPT      = 20.;       // impact parameter max
  static Float_t energyConfigAMPT    = 0.;        // CMS energy
  static Int_t   neventsConfig   = 1;       // number of events

  
  if (gSystem->Getenv("CONFIG_PROCESS")) {
    processConfigAMPT = gSystem->Getenv("CONFIG_PROCESS");
  }
  
  if (gSystem->Getenv("CONFIG_SYSTEM")) {
    systemConfigAMPT = gSystem->Getenv("CONFIG_SYSTEM");
  }
  
  if (gSystem->Getenv("CONFIG_BMIN"))
    bminConfigAMPT = atof(gSystem->Getenv("CONFIG_BMIN"));
  if (bminConfigAMPT < 0) {
    printf(">>>>> Invalid min impact parameter: %f \n", bminConfigAMPT);
    abort();
  }
  if (gSystem->Getenv("CONFIG_BMAX"))
    bmaxConfigAMPT = atof(gSystem->Getenv("CONFIG_BMAX"));
  if (bmaxConfigAMPT <= bminConfigAMPT) {
    printf(">>>>> Invalid max impact parameter: %f \n", bmaxConfigAMPT);
    abort();
  }
  
  if (gSystem->Getenv("CONFIG_ENERGY"))
    energyConfigAMPT = atoi(gSystem->Getenv("CONFIG_ENERGY"));
  if (energyConfigAMPT <= 0) {
    printf(">>>>> Invalid CMS energy: %f \n", energyConfigAMPT);
    abort();
  }
  
  if (gSystem->Getenv("CONFIG_NEVENTS"))
    neventsConfig = atoi(gSystem->Getenv("CONFIG_NEVENTS"));


  // AMPT settings
  Int_t isoft = 1;  //1: defaul - 4: string melting
  Int_t ntmax = 150;        // NTMAX: number of timesteps (D=150)
  if (processConfigAMPT.Contains("StringMelting")) isoft = 4;
  if (processConfigAMPT.Contains("NoART")) ntmax = 3;

  // projectile-target
  Int_t projectileA = 0;
  Int_t projectileZ = 0;
  Int_t targetA = 0;
  Int_t targetZ = 0;
  TString targetString = "";
  TString projectileString = "";

  if (systemConfigAMPT.EqualTo("Pb-Pb")) {
    projectileA = 208;
    projectileZ = 82;
    targetA = 208;
    targetZ = 82;
    targetString += "A";
    projectileString += "A";
  } else if (systemConfigAMPT.EqualTo("p-Pb")) {
    projectileA = 1;
    projectileZ = 1;
    targetA = 208;
    targetZ = 82;
    targetString += "A";
    projectileString += "P";
  } else if (systemConfigAMPT.EqualTo("Pb-p")) {
    projectileA = 208;
    projectileZ = 82;
    targetA = 1;
    targetZ = 1;
    targetString += "P";
    projectileString += "A";
  } else if (systemConfigAMPT.EqualTo("p-p")) {
    projectileA = 1;
    projectileZ = 1;
    targetA = 1;
    targetZ = 1;
    targetString += "P";
    projectileString += "P";
  } else {
    Printf("System %s not defined -> abort...",systemConfigAMPT.Data());
    return;
  }
  
  
  printf(">>>>> AMPT configuration >>>>> \n");
  printf(">>>>>          process: %s \n", processConfigAMPT.Data());
  printf(">>>>>           system: %s \n", systemConfigAMPT.Data());
  printf(">>>>>            b-min: %f \n", bminConfigAMPT);
  printf(">>>>>            b-max: %f \n", bmaxConfigAMPT);
  printf(">>>>>       CMS energy: %f \n", energyConfigAMPT);
  printf(">>>>> number of events: %d \n", neventsConfig);

  //configuration reading done
  //====================================================================

  
  TString fifoname = "ampteventfifo";
  gROOT->ProcessLine(Form(".! rm -rf %s", fifoname.Data()));
  Printf("Running now: .! sh $ALIDPG_ROOT/MC/EXTRA/gen_ampt.sh %s %i %i %i %i %s %s %.2f %.2f %.2f %i %i %i &> gen_ampt.log", fifoname.Data(),
	 projectileA,
	 projectileZ,
	 targetA,
	 targetZ,
	 projectileString.Data(),
	 targetString.Data(),
	 bminConfigAMPT,
	 bmaxConfigAMPT,
	 energyConfigAMPT,
	 neventsConfig,
	 isoft,
	 ntmax);

  gROOT->ProcessLine(Form(".! sh $ALIDPG_ROOT/MC/EXTRA/gen_ampt.sh %s %i %i %i %i %s %s %.2f %.2f %.2f %i %i %i &> gen_ampt.log", fifoname.Data(),
              projectileA,
              projectileZ,
              targetA,
              targetZ,
              projectileString.Data(),
              targetString.Data(),
              bminConfigAMPT,
              bmaxConfigAMPT,
              energyConfigAMPT,
              neventsConfig,
              isoft,
              ntmax));

  Printf("Running AMPT in standalone...DONE");

}
