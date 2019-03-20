/// Macro to check the AliPhysics version in the grid jobs
///   (extracted from ALIEN_JDL_PACKAGES ot ALIEN_PACKAGES)
/// against the version passed as argument
/// Example:  IsAliPhysicsMoreRecentThan("v5-09-20")


Bool_t IsAliPhysicsMoreRecentThan(TString version){

  Int_t vvv,nnn1,nnn2;
  sscanf(version,"v%d-%d-%d",&vvv,&nnn1,&nnn2);
  
  // check if ALIEN_JDL_PACKAGES exists
  // if not, try with ALIEN_PACKAGES
  if(!gSystem->Getenv("ALIEN_JDL_PACKAGES"))
    if(gSystem->Getenv("ALIEN_PACKAGES")){
      gSystem->Setenv("ALIEN_JDL_PACKAGES", gSystem->Getenv("ALIEN_PACKAGES"));
      printf("Using ALIEN_PACKAGES instead of ALIEN_JDL_PACKAGES\n");
    }

  if(gSystem->Getenv("ALIEN_JDL_PACKAGES")){
    TString packg=gSystem->Getenv("ALIEN_JDL_PACKAGES");
    Int_t pos1=packg.Index("AliPhysics");
    TString subs=packg(pos1,packg.Length()-pos1);
    Int_t pos2=subs.Index("VO_ALICE");
    if(pos2<=0) pos2=subs.Length();
    TString aliph=subs(0,pos2);
    Int_t ver,n1,n2;
    Char_t str2[20];
    sscanf(aliph.Data(),"AliPhysics::v%d-%d-%02d%s",&ver,&n1,&n2,str2);
    if(ver<vvv) return kFALSE;
    else if(ver>vvv) return kTRUE;
    else if(ver==vvv){
      if(n1<nnn1) return kFALSE;
      else if(n1>nnn1) return kTRUE;
      else if(n1==nnn1){
	if(n2<=nnn2) return kFALSE;
	else return kTRUE;
      }
    }
  }else{
    printf("ALIEN_PACKAGES and ALIEN_JDL_PACKAGES not defined\n");
    printf("  --> Cannot check AliPhysics version\n");
    return kTRUE;
  }
}

Bool_t IsAliPhysicsMoreRecentThanOrEqualTo(TString version){

  Int_t vvv,nnn1,nnn2;
  sscanf(version,"v%d-%d-%d",&vvv,&nnn1,&nnn2);
  
  // check if ALIEN_JDL_PACKAGES exists
  // if not, try with ALIEN_PACKAGES
  if(!gSystem->Getenv("ALIEN_JDL_PACKAGES"))
    if(gSystem->Getenv("ALIEN_PACKAGES")){
      gSystem->Setenv("ALIEN_JDL_PACKAGES", gSystem->Getenv("ALIEN_PACKAGES"));
      printf("Using ALIEN_PACKAGES instead of ALIEN_JDL_PACKAGES\n");
    }

  if(gSystem->Getenv("ALIEN_JDL_PACKAGES")){
    TString packg=gSystem->Getenv("ALIEN_JDL_PACKAGES");
    Int_t pos1=packg.Index("AliPhysics");
    TString subs=packg(pos1,packg.Length()-pos1);
    Int_t pos2=subs.Index("VO_ALICE");
    if(pos2<=0) pos2=subs.Length();
    TString aliph=subs(0,pos2);
    Int_t ver,n1,n2;
    Char_t str2[20];
    sscanf(aliph.Data(),"AliPhysics::v%d-%d-%02d%s",&ver,&n1,&n2,str2);
    if(ver==vvv && n1==nnn1 && n2==nnn2) return kTRUE;
    if(ver<vvv) return kFALSE;
    else if(ver>vvv) return kTRUE;
    else if(ver==vvv){
      if(n1<nnn1) return kFALSE;
      else if(n1>nnn1) return kTRUE;
      else if(n1==nnn1){
	if(n2<=nnn2) return kFALSE;
	else return kTRUE;
      }
    }
  }else{
    printf("ALIEN_PACKAGES and ALIEN_JDL_PACKAGES not defined\n");
    printf("  --> Cannot check AliPhysics version\n");
    return kTRUE;
  }
}

Bool_t IsAliPhysicsEqualTo(TString version){
  Int_t vvv,nnn1,nnn2;
  sscanf(version,"v%d-%d-%d",&vvv,&nnn1,&nnn2);
  
  // check if ALIEN_JDL_PACKAGES exists
  // if not, try with ALIEN_PACKAGES
  if(!gSystem->Getenv("ALIEN_JDL_PACKAGES"))
    if(gSystem->Getenv("ALIEN_PACKAGES")){
      gSystem->Setenv("ALIEN_JDL_PACKAGES", gSystem->Getenv("ALIEN_PACKAGES"));
      printf("Using ALIEN_PACKAGES instead of ALIEN_JDL_PACKAGES\n");
    }

  if(gSystem->Getenv("ALIEN_JDL_PACKAGES")){
    TString packg=gSystem->Getenv("ALIEN_JDL_PACKAGES");
    Int_t pos1=packg.Index("AliPhysics");
    TString subs=packg(pos1,packg.Length()-pos1);
    Int_t pos2=subs.Index("VO_ALICE");
    if(pos2<=0) pos2=subs.Length();
    TString aliph=subs(0,pos2);
    Int_t ver,n1,n2;
    Char_t str2[20];
    sscanf(aliph.Data(),"AliPhysics::v%d-%d-%02d%s",&ver,&n1,&n2,str2);
    if(ver==vvv && n1==nnn1 && n2==nnn2) return kTRUE;
    else return kFALSE;
  }else{
    printf("ALIEN_PACKAGES and ALIEN_JDL_PACKAGES not defined\n");
    printf("  --> Cannot check AliPhysics version\n");
    return kFALSE;
  }
}
