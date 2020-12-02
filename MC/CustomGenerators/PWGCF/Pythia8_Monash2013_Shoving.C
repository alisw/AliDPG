AliGenerator *
GeneratorCustom(TString sAmplitude = "3")
{
  AliGenerator *py8 = GeneratorPythia8(kPythia8Tune_Monash2013);
  
  Float_t fgAmplitude = atof(sAmplitude);
  
  printf("*** SET Pythia8 Monash Shobing Ropewalk:gAmplitude = %lf\n",fgAmplitude);
  
  (AliPythia8::Instance())->ReadString("SoftQCD:nonDiffractive = on");
  (AliPythia8::Instance())->ReadString("SoftQCD:singleDiffractive = on");
  (AliPythia8::Instance())->ReadString("SoftQCD:doubleDiffractive = on");
  (AliPythia8::Instance())->ReadString("Ropewalk:RopeHadronization = on"); 
  (AliPythia8::Instance())->ReadString("Ropewalk:doShoving = on"); 
  (AliPythia8::Instance())->ReadString("Ropewalk:doFlavour = off"); 
  (AliPythia8::Instance())->ReadString("Ropewalk:rCutOff = 10.0"); 
  (AliPythia8::Instance())->ReadString("Ropewalk:limitMom = on"); 
  (AliPythia8::Instance())->ReadString(Form("Ropewalk:pTcut = %lf",2.0)); 
  (AliPythia8::Instance())->ReadString("Ropewalk:r0 = 0.41"); 
  (AliPythia8::Instance())->ReadString("Ropewalk:m0 = 0.2"); 
  (AliPythia8::Instance())->ReadString(Form("Ropewalk:gAmplitude = %lf",fgAmplitude)); 
  (AliPythia8::Instance())->ReadString("Ropewalk:gExponent = 1.0"); 
  (AliPythia8::Instance())->ReadString("Ropewalk:deltat = 0.1"); 
  (AliPythia8::Instance())->ReadString("Ropewalk:tShove = 1."); 
  (AliPythia8::Instance())->ReadString("Ropewalk:deltay = 0.1"); 
  (AliPythia8::Instance())->ReadString("Ropewalk:tInit = 1.5"); 
  // Enabling setting of vertex information.
  (AliPythia8::Instance())->ReadString("PartonVertex:setVertex = on"); 
  (AliPythia8::Instance())->ReadString("PartonVertex:protonRadius = 0.7"); 
  (AliPythia8::Instance())->ReadString("PartonVertex:emissionWidth = 0.1"); 
  
  return py8;
}
