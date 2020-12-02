#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TH1.h>
#include <TFile.h>
#include <TKey.h>
#include <TCanvas.h>
#include <TList.h>
#include <TMath.h>
#include <TFolder.h>
#include <TLatex.h>
#include <TString.h>
#include <TPaveStats.h>
#include "AliPerformanceObject.h"
#include "AliPhysicsSelection.h"
#endif



TString prefix="";
TFile* fileOut=0x0;
Int_t correlationCase=0;
// 0 = partially correlated
// 1 = fully uncorrelated


void ProcessFile(TString fname, TString dirToAnalyse);
void ProcessDirectory(TDirectoryFile* df);
void ProcessList(TList* ls);
void ProcessArray(TObjArray* arr);
void ProcessPhysSel(TList *lst);
void WriteHisto(TObject* obj);
void WriteProfile(TObject* obj);
Bool_t AreIdenticalHistos(TH1* hA, TH1* hB);
Bool_t CompareHistos(TH1* hA, TH1* hB);
void DrawRatio(TH1* hR);

void CompareAllHistos(TString filename="QAresults_v5-09-37.root", TString filename2="QAresults_v5-09-36.root", TString dirToAnalyse="", Bool_t areIndependentSamples=kFALSE){

  if(areIndependentSamples) correlationCase=1;

  fileOut=new TFile("TmpA.root","recreate");
  ProcessFile(filename,dirToAnalyse);
  fileOut->Close();
  delete fileOut;
  fileOut=new TFile("TmpB.root","recreate");
  ProcessFile(filename2,dirToAnalyse);
  fileOut->Close();
  delete fileOut;

  TFile* fileA=new TFile("TmpA.root");
  TFile* fileB=new TFile("TmpB.root");
  
  Int_t nkeys=fileA->GetNkeys();
  TList* lkeys=fileA->GetListOfKeys();
  for(Int_t j=0; j<nkeys; j++){
    TKey* k=(TKey*)lkeys->At(j);
    TString cname=k->GetClassName();
    TString oname=k->GetName();
    if(cname.BeginsWith("TH")){
      TH1* hA=static_cast<TH1*>(fileA->Get(oname.Data()));
      TH1* hB=static_cast<TH1*>(fileB->Get(oname.Data()));
      if(hA && hB){
        Bool_t ok=AreIdenticalHistos(hA,hB);
        if(ok) printf("%s       ---> IDENTICAL\n",oname.Data());
        else{
          Bool_t ok=CompareHistos(hA,hB);
          if(ok) printf("%s       ---> COMPATIBLE\n",oname.Data());
          else printf("********  %s     ---> BAD *****\n",oname.Data());
        }
      }else{
        if(!hA) printf("%s    ---> MISSING in first file\n",hA->GetName());
        if(!hB) printf("%s    ---> MISSING  in second file\n",hA->GetName());
      }
    }
  }
}

void ProcessFile(TString fname, TString dirToAnalyse){

  TFile *fileBase=TFile::Open(fname.Data());
  Int_t nkeys=fileBase->GetNkeys();
  TList* lkeys=fileBase->GetListOfKeys();
  for(Int_t j=0; j<nkeys; j++){
    prefix="";
    TKey* k=(TKey*)lkeys->At(j);
    TString cname=k->GetClassName();
    TString oname=k->GetName();
    printf("****** KEY %d: %s (class %s)   ******\n",j,oname.Data(),cname.Data());
    if(cname=="TDirectoryFile"){
      TDirectoryFile* df=(TDirectoryFile*)fileBase->Get(oname.Data());
      TString dirName=df->GetName();
      if(dirToAnalyse.Length()>0 && !dirName.Contains(dirToAnalyse.Data())){
	printf("Skip TDirectoryFile %s\n",dirName.Data());
      }else{
	prefix.Append(Form("%s_",df->GetName()));
	ProcessDirectory(df);
      }
    }else if(cname=="TList"){
      TList* ls=(TList*)fileBase->Get(oname.Data());
      TString lsName=ls->GetName();
      if(lsName=="cstatsout") ProcessPhysSel(ls);
      if(dirToAnalyse.Length()>0 && !lsName.Contains(dirToAnalyse.Data())){
	printf("Skip TList %s\n",lsName.Data());
      }else{
	prefix.Append(Form("%s_",ls->GetName()));
	ProcessList(ls);
      }
    }else if(cname.BeginsWith("TH")){
      TObject* o=(TObject*)fileBase->Get(oname.Data());
      WriteHisto(o);
    }
  }
}


void ProcessPhysSel(TList *lst){
  AliPhysicsSelection* psel = (AliPhysicsSelection*)lst->FindObject("AliPhysicsSelection");
  TH2F* hst=(TH2F*)psel->GetStatistics("");
  WriteHisto(hst);
  return;
}

void ProcessDirectory(TDirectoryFile* df){
  printf("--- Process TDirectoryFile %s ---\n",df->GetName());

  Int_t nkeys=df->GetNkeys();
  TList* lkeys=df->GetListOfKeys();
  for(Int_t j=0; j<nkeys; j++){
    TKey* k=(TKey*)lkeys->At(j);
    TString cname=k->GetClassName();
    TString oname=k->GetName();
    Int_t oSize=k->GetObjlen();
    if(cname=="TList" || cname=="THashList"){
      TList* ls=(TList*)df->Get(oname.Data());
      prefix.Append(Form("%s_",ls->GetName()));	
      ProcessList(ls);
    }else if(cname=="TObjArray"){
      TObjArray* arr=(TObjArray*)df->Get(oname.Data());
      prefix.Append(Form("%s_",arr->GetName()));	
      ProcessArray(arr);
    }else if(cname.Contains("THnSpars")){
      printf("   --> DIRECTORY %s CONTAINS A THNSPARSE: %s\n",df->GetName(),oname.Data());
    }else if(cname.Contains("TNtuple")){
      printf("   --> DIRECTORY %s CONTAINS A TNtuple: %s\n",df->GetName(),oname.Data());      
    }else if(cname.Contains("TTree")){
      printf("   --> DIRECTORY %s CONTAINS A TTree: %s\n",df->GetName(),oname.Data());      
    }else if(cname.BeginsWith("TH")){
      TObject* o=(TObject*)df->Get(oname.Data());
      WriteHisto(o);
    }else if(cname.BeginsWith("TProfile")){
      TObject* o=(TObject*)df->Get(oname.Data());
      WriteProfile(o);
    }
  }
  return;
}



void ProcessList(TList* ls){

  printf(" * Process TList %s *\n",ls->GetName());
  Int_t nent=ls->GetEntries();
  for(Int_t j=0; j<nent; j++){
    TObject* o=(TObject*)ls->At(j);
    TString clname=o->ClassName();
    if(clname.Contains("THnSpars")){
      printf("   --> LIST %s CONTAINS A THNSPARSE: %s \n",ls->GetName(),o->GetName());
    }else if(clname.Contains("TNtupl")){
      printf("   --> LIST %s CONTAINS A TNtuple: %s\n",ls->GetName(),o->GetName());
    }else if(clname.Contains("TTree")){
      printf("   --> LIST %s CONTAINS A TTree: %s\n",ls->GetName(),o->GetName());
    }else if(clname.BeginsWith("TH")){
      WriteHisto(o);
      //      printf("Histo %s size %.0f\n",o->GetName(),hSize/1e6);
    }else if(clname.BeginsWith("TProfile")){
      WriteProfile(o);
    }else if(clname=="TList"){
      TList* lsInside=(TList*)ls->At(j);
      ProcessList(lsInside);
    }else if(clname=="AliPerformanceTPC" || clname=="AliPerformanceMatch" ||  clname=="AliPerformanceDEdx"){
      prefix.Append(Form("%s_",clname.Data()));
      AliPerformanceObject* tpc = (AliPerformanceObject*)ls->At(j);
      TFolder *fold = tpc->GetAnalysisFolder();
      TList* ltpc=(TList*)fold->GetListOfFolders();
      ProcessList(ltpc);
    }else{
      printf("Class not found: %s\n",clname.Data());
    }
  }
  return;
}


void ProcessArray(TObjArray* arr){

  printf(" * Process TObjArray %s *\n",arr->GetName());
  Int_t nent=arr->GetEntries();
  for(Int_t j=0; j<nent; j++){
    TObject* o=(TObject*)arr->At(j);
    if(!o) continue;
    TString clname=o->ClassName();
    if(clname.Contains("THnSpars")){
      printf("   --> ARRAY %s CONTAINS A THNSPARSE: %s \n",arr->GetName(),o->GetName());
    }else if(clname.Contains("TNtupl")){
      printf("   --> ARRAY %s CONTAINS A TNtuple: %s\n",arr->GetName(),o->GetName());
    }else if(clname.Contains("TTree")){
      printf("   --> ARRAY %s CONTAINS A TTree: %s\n",arr->GetName(),o->GetName());
    }else if(clname.BeginsWith("TH")){
      WriteHisto(o);
      //      printf("Histo %s size %.0f\n",o->GetName(),hSize/1e6);
    }else if(clname.BeginsWith("TProfile")){
      WriteProfile(o);
    }else if(clname=="TObjArray"){
      TObjArray* arrInside=(TObjArray*)arr->At(j);
      prefix.Append(Form("%s_",arrInside->GetName()));	
      ProcessArray(arrInside);
    }else{
      printf("Class not found: %s\n",clname.Data());
    }
  }
}


void WriteHisto(TObject* obj){
  TH1* hA=static_cast<TH1*>(obj);
  TDirectory *current=gDirectory;
  fileOut->cd();
  hA->Write(Form("%s%s",prefix.Data(),hA->GetName()));
  current->cd();
  return;
}

void WriteProfile(TObject* obj){
  return;
}


Bool_t AreIdenticalHistos(TH1* hA, TH1* hB){

  Long_t nEventsA=hA->GetEntries(); //temporary: we should use the number of analyzed events
  Long_t nEventsB=hB->GetEntries(); //temporary: we should use the number of analyzed events
  if(nEventsA!=nEventsB) {
    printf(" -> Different number of entries: A --> %ld, B --> %ld\n", nEventsA, nEventsB);
    return kFALSE;
  }
  for(Int_t ix=1; ix<=hA->GetNbinsX(); ix++){
    for(Int_t iy=1; iy<=hA->GetNbinsY(); iy++){
      for(Int_t iz=1; iz<=hA->GetNbinsZ(); iz++){
	Double_t cA=hA->GetBinContent(ix,iy,iz);
	Double_t cB=hB->GetBinContent(ix,iy,iz);
	if(TMath::Abs(cA-cB)>0.001*TMath::Abs(cA)) return kFALSE;
      }
    }
  }
  return kTRUE;
}

Bool_t CompareHistos(TH1* hA, TH1* hB){
  // implement here some simple checks that the two histograms are statistically compatible

  Int_t nEventsA=hA->GetEntries();
  Int_t nEventsB=hB->GetEntries();
  if(nEventsA==0 && nEventsB==0) return kTRUE;
  if(nEventsA==0 && nEventsB!=0) return kFALSE;
  if(nEventsA!=0 && nEventsB==0) return kFALSE;

  Double_t chi2=0;
  Double_t meandiff=0;
  Int_t nBins=0;
  for(Int_t ix=1; ix<=hA->GetNbinsX(); ix++){
    for(Int_t iy=1; iy<=hA->GetNbinsY(); iy++){
      for(Int_t iz=1; iz<=hA->GetNbinsZ(); iz++){
	Double_t cA=hA->GetBinContent(ix,iy,iz);
	Double_t eA=0;
	if(cA<0) printf("Negative counts!!! cA=%f in bin %d %d %d\n",cA,ix,iy,iz);
	else eA=TMath::Sqrt(cA);
	Double_t cB=hB->GetBinContent(ix,iy,iz);
	Double_t eB=0;
	if(cB<0) printf("Negative counts!!! cB=%f in bin %d %d %d\n",cB,ix,iy,iz);
	else eB=TMath::Sqrt(cB);
	Double_t diff=cA-cB;
	if(cA>0 && cB>0){
	  Double_t correl=0.;
	  if(correlationCase==0){
	    // estimate degree of correlation from number of events in histogram
	    // assume that the histogram with less events is a subsample of that with more events
	    correl=TMath::Sqrt(cA/cB);
	    if(cA>cB) correl=1./correl;
	  }
	  Double_t sigma2=eA*eA+eB*eB-2*correl*eA*eB; // to be improved
	  meandiff+=(cA/nEventsA-cB/nEventsB);
	  if(sigma2>0) chi2+=diff*diff/sigma2;
	  nBins++;
	}
      }
    }
  }
  if(nBins>1){
    printf(" -> Different contents: %s  chi2/nBins=%f   meanreldiff=%f \n",hA->GetName(),chi2/nBins,meandiff);
    TCanvas* c=new TCanvas(hA->GetName(),hA->GetName(),1200,600);
    c->Divide(2,1);
    c->cd(1);
    TString hcln=hA->ClassName();
    TString optD="";
    if(hcln.Contains("TH2")) optD="box";
    hA->SetLineColor(1);
    hA->SetMarkerColor(1);
    TH1* hAc=(TH1*)hA->DrawClone(optD.Data());
    hB->SetLineColor(2);
    hB->SetMarkerColor(2);
    TH1* hBc=(TH1*)hB->DrawClone(Form("%ssames",optD.Data()));
    c->Update();
    TPaveStats* stA=(TPaveStats*)hAc->GetListOfFunctions()->FindObject("stats");
    if(stA){
      stA->SetLineColor(1);
      stA->SetTextColor(1);
      stA->SetY1NDC(0.68);
      stA->SetY2NDC(0.88);
    }
    TPaveStats* stB=(TPaveStats*)hBc->GetListOfFunctions()->FindObject("stats");
    if(stB){
      stB->SetLineColor(2);
      stB->SetTextColor(2);
      stB->SetY1NDC(0.45);
      stB->SetY2NDC(0.65);
    }
   
    c->cd(2);
    if(hcln.Contains("TH3")){
      TH1D* hXa=((TH3*)hA)->ProjectionX(Form("%s_xA",hA->GetName()));
      TH1D* hXb=((TH3*)hB)->ProjectionX(Form("%s_xB",hB->GetName()));
      TH1D* hYa=((TH3*)hA)->ProjectionY(Form("%s_yA",hA->GetName()));
      TH1D* hYb=((TH3*)hB)->ProjectionY(Form("%s_yB",hB->GetName()));
      TH1D* hZa=((TH3*)hA)->ProjectionZ(Form("%s_zA",hA->GetName()));
      TH1D* hZb=((TH3*)hB)->ProjectionZ(Form("%s_zB",hB->GetName()));
      hXa->Divide(hXb);
      hYa->Divide(hYb);
      hZa->Divide(hZb);
      TPad* rpad=(TPad*)gPad;
      rpad->Divide(1,3);
      rpad->cd(1);
      DrawRatio(hXa);
      rpad->cd(2);
      DrawRatio(hYa);
      rpad->cd(3);
      DrawRatio(hZa);
    }else{
      hA->Divide(hB);
      for(Int_t k=1;k<=hA->GetNbinsX();k++) hA->SetBinError(k,0.000000001);
      hA->SetMinimum(TMath::Max(0.98,0.95*hA->GetBinContent(hA->GetMinimumBin())-hA->GetBinError(hA->GetMinimumBin())));
      hA->SetMaximum(TMath::Min(1.02,1.05*hA->GetBinContent(hA->GetMaximumBin())+hA->GetBinError(hA->GetMaximumBin())));
      hA->SetStats(0);
      if(hcln.Contains("TH2")) hA->Draw("colz");
      else if(hcln.Contains("TH1")) DrawRatio(hA);
      else hA->Draw();
    }
    c->cd(1);
    TString outc="";
    Int_t colt=1;
    Bool_t retVal=kTRUE;
    if(chi2/nBins<1.5 && TMath::Abs(meandiff)<0.01){
      outc="Compatible";
      colt=kGreen+1;
    }else{
      outc="BAD";
      colt=2;
      retVal=kFALSE;
    }
    TLatex* toutc=new TLatex(0.2,0.85,outc.Data());
    toutc->SetNDC();
    toutc->SetTextColor(colt);
    toutc->SetTextFont(62);
    toutc->Draw();
    c->SaveAs(Form("%s.png",hA->GetName()));
    delete c;
    return retVal;
  }
  return kTRUE;
}

void DrawRatio(TH1* hR){
  hR->SetMarkerStyle(20);
  hR->SetMarkerSize(0.5);
  hR->SetMinimum(TMath::Max(0.98,0.95*hR->GetBinContent(hR->GetMinimumBin())-hR->GetBinError(hR->GetMinimumBin())));
  hR->SetMaximum(TMath::Min(1.02,1.05*hR->GetBinContent(hR->GetMaximumBin())+hR->GetBinError(hR->GetMaximumBin())));
  hR->SetStats(0);
  hR->GetYaxis()->SetTitle("Ratio");
  hR->Draw("P");
  return;
}
