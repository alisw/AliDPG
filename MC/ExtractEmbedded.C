#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AliPID.h"
#include "AliESDEvent.h"
#include "AliESDtrack.h"
#include "AliMCEvent.h"
#include "AliESDkink.h"
#include "AliESDcascade.h"
#include "AliESDv0.h"
#include "AliESDVertex.h"
#include "AliESDTrdTrigger.h"
#include "AliESDAD.h"
#include "AliESDACORDE.h"
#include "AliESDCaloTrigger.h"
#include "AliMultiplicity.h"
#include "AliESDFIT.h"
#include "AliESDTZERO.h"
#include "AliESDVZERO.h"
#include "AliESDFMD.h"
#include "AliESDZDC.h"
#include "AliESDHeader.h"
#include "AliESDRun.h"
#include "AliESDPmdTrack.h"
#include "AliESDMuonGlobalTrack.h"
#include "AliESDMuonPad.h"
#include "AliESDMuonTrack.h"
#include "AliESDMuonCluster.h"
#include "AliESDTOFHit.h"
#include "AliESDTOFCluster.h"
#include "AliESDTOFMatch.h"
#include "AliESDHLTDecision.h"
#include "TFile.h"
#include "TTree.h"
#include "TClonesArray.h"
#include "TString.h"
#endif

void FilterEvent(AliESDEvent *evIn, AliESDEvent *evOut, Bool_t keepMixed=kFALSE);
Bool_t RemoveBKGFromGalice(const char* outgaliceFName="galice_EMB.root",const char* inpgaliceFName="galice.root");
Bool_t ExtractEmbedded(Bool_t keepMixed = kFALSE, const char* outESDFName="AliESDs_EMB.root", const char* inpESDFName="AliESDs.root");
Bool_t KeepTrack(int lbl);
Bool_t KeepTrack(AliESDtrack* trc);


//_______________________________________________________________________________
Bool_t ExtractEmbedded(Bool_t keepMixed,const char* outESDFName, const char* inpESDFName)
{
  TFile* flIn = TFile::Open(inpESDFName);
  if (!flIn || flIn->IsZombie()) {
    printf("Error: failed to open input file %s\n",inpESDFName);
    return kFALSE;
  }
  TTree* esdTreeIn = (TTree*)flIn->Get("esdTree");
  if (!esdTreeIn) {
    printf("Error: no esdTree in the input file %s\n",inpESDFName);
    return kFALSE;
  }
  AliESDEvent* esdIn = new AliESDEvent();
  esdIn->ReadFromTree(esdTreeIn);

  TFile* flOut = TFile::Open(outESDFName,"recreate");
  if (!flOut || flOut->IsZombie()) {
    printf("Error: failed to open output file %s\n",outESDFName);
    return kFALSE;
  }
  TTree* esdTreeOut = new TTree("esdTree", "Tree with ESD objects (derived from Embedded MC)");  
  AliESDEvent* esdOut = new AliESDEvent();
  esdOut->CreateStdContent();
  esdOut->AddObject(new AliESDHLTDecision);
  esdTreeOut->GetUserInfo()->Add(esdOut);
  esdOut->WriteToTree(esdTreeOut);
  
  int nev = esdTreeIn->GetEntries();

  for (int iev=0;iev<nev;iev++) {
    esdTreeIn->GetEntry(iev);
    FilterEvent(esdIn, esdOut, keepMixed);
    esdTreeOut->Fill();
    printf("Event#%2d %3d tracks -> %3d tracks\n",iev,
	   esdIn->GetNumberOfTracks(), esdOut->GetNumberOfTracks());
  }
  TList *inpUI = esdTreeIn->GetUserInfo();
  TList *outUI = esdTreeOut->GetUserInfo();
  int ne  = inpUI->GetEntries();
  for (int ie=0;ie<ne;ie++) {
    TObject* obj = inpUI->At(ie);
    if (!strcmp(obj->GetName(),"ESDObjectsConnectedToTree")) continue;
    if (!strcmp(obj->GetName(),"AliESDEvent")) continue;
    outUI->Add(obj);
  }
  outUI->SetOwner(inpUI->IsOwner());
  inpUI->SetOwner(kFALSE);
  inpUI->Clear();

  flOut->cd();
  esdTreeOut->Write(esdTreeOut->GetName(),TObject::kOverwrite);
  delete esdTreeOut;
  flOut->Close();
  delete flOut;
  //
  delete esdTreeIn;
  flIn->Close();
  delete flIn;

  return kTRUE;
}

void FilterEvent(AliESDEvent *evIn, AliESDEvent *evOut, Bool_t keepMixed)
{
  // copy wanted tracks and remember their indices
  evOut->Reset();
  int ntr = evIn->GetNumberOfTracks();
  int trStatus[ntr];
  memset(trStatus,-1,sizeof(int)*ntr);
  
  // check tracks
  for (int itr=0;itr<ntr;itr++) {
    AliESDtrack* trc = evIn->GetTrack(itr);
    trc->SetESDEvent(evIn); // don't use connect tracks since it may fill some extra structures
    if ( KeepTrack(trc) ) {
      trStatus[itr] = -10; // flag track to save
    }
  }  
  // filter V0's
  int nv0 = evIn->GetNumberOfV0s();
  for (int iv=0;iv<nv0;iv++) {
    AliESDv0* v0 = evIn->GetV0(iv);
    int ndisc = (trStatus[v0->GetPindex()]==-1) + (trStatus[v0->GetNindex()]==-1);
    if (ndisc==2) continue; // all prongs are from the BG
    if (ndisc!=0) {
      if (!keepMixed) continue; // discard V0s made of signal and bg track
      trStatus[v0->GetPindex()] = -10; // keep both tracks
      trStatus[v0->GetNindex()] = -10;	
    }
    // copy V0
    evOut->AddV0(v0);
  }
  // filter Cascades
  int ncasc = evIn->GetNumberOfCascades();
  for (int ic=0;ic<ncasc;ic++) {
    AliESDcascade* casc = evIn->GetCascade(ic);
    int ndisc = (trStatus[casc->GetPindex()]==-1) + (trStatus[casc->GetNindex()]==-1) + (trStatus[casc->GetBindex()]==-1);
    if (ndisc==3) continue; // all prongs are from the BG
    if (ndisc!=0) {
      if (!keepMixed) continue; // discard cascades made of signal and bg track
      trStatus[casc->GetPindex()] = -10; // keep all tracks
      trStatus[casc->GetNindex()] = -10;
      trStatus[casc->GetBindex()] = -10;
    }
    // copy Cascade
    evOut->AddCascade(casc);
  }
  // filter kinks
  int nknk = evIn->GetNumberOfKinks(), nkinkAdd=0;
  int knkStatus[nknk];
  memset(knkStatus,-1,sizeof(int)*nknk);

  for (int ik=0;ik<nknk;ik++) {
    const AliESDkink *kink = evIn->GetKink(ik);
    if (kink->GetIndex(0)>=ntr || kink->GetIndex(1)>=ntr) {
      // protection for the data produced with aliroot before v5-09-25, where a fix for kink indices was introduced
      printf("bad kink %d: %d %d Ntr: %d -> %d\n",ik,kink->GetIndex(0),kink->GetIndex(1),ntr, ntr<kink->GetIndex(0)||ntr<kink->GetIndex(1));
      continue;
    }
    int ndisc = (trStatus[kink->GetIndex(0)]==-1) + (trStatus[kink->GetIndex(1)]==-1);
    //    printf("kink %d: nd: %d %d %d Ntr: %d -> %d\n",ik,ndisc,kink->GetIndex(0),kink->GetIndex(1),ntr, ntr<kink->GetIndex(0)||ntr<kink->GetIndex(1));
    if (ndisc==2) continue; // all prongs are from the BG
    if (ndisc!=0) {
      if (!keepMixed) continue; // discard kinks made of signal and bg track
      trStatus[kink->GetIndex(0)] = -10; // keep all tracks
      trStatus[kink->GetIndex(0)] = -10;
    }
    // copy Kink
    evOut->AddKink(kink);
    knkStatus[ik] = nkinkAdd++;
  }

  // filter calo clusters, cells
  /* // need to decide what kind of clusters we filter out
  Int_t ncl = evIn->GetNumberOfCaloClusters();
  for (Int_t n=0; n<ncl; n++) {
    AliESDCaloCluster *cluster = evIn->GetCaloCluster(n);
  }
  */
      
  // filter tracks
  //
  int ntrAdd = 0;
  for (int itr=0;itr<ntr;itr++) {
    if (trStatus[itr]==-1) continue; // discarded
    const AliESDtrack* trcIn = evIn->GetTrack(itr);
    evOut->AddTrack(trcIn);
    trStatus[itr] = ntrAdd; // memorrise added track ID
    AliESDtrack* trcOut = evOut->GetTrack(ntrAdd);
    trcOut->SetFriendNotStored(kTRUE);
    trcOut->SetFriendTrackID(-1);
    trcOut->SetEMCALcluster(AliESDtrack::kEMCALNoMatch);
    int kinkID[3]={0}, kAdd = 0;
    for (int ik=0;ik<3;ik++) {
      int kID = trcIn->GetKinkIndex(ik);
      if (kID==0) continue;
      int kIDA = TMath::Abs(kID)-1;
      if (knkStatus[kIDA]==-1) continue; // kink was not stored
      kinkID[kAdd++] = kID<0 ? -(knkStatus[kIDA]+1) : (knkStatus[kIDA]+1);
    }
    trcOut->SetKinkIndexes(kinkID); // reset indices
    ntrAdd++;
  }
  // filter TOF clusters and hits used but saved tracks
  TClonesArray* esdTOFHitArrIn = evIn->GetESDTOFHits();
  TClonesArray* esdTOFClArrIn = evIn->GetESDTOFClusters();
  TClonesArray* esdTOFHitArrOut = evOut->GetESDTOFHits();
  TClonesArray* esdTOFClArrOut = evOut->GetESDTOFClusters();
  int ntofHits = esdTOFHitArrIn->GetEntries();
  int ntofCls  = esdTOFClArrIn->GetEntries();
  int ntofClAdd=0, ntofHAdd=0;
  for (int ict=0;ict<ntofCls;ict++) {
    AliESDTOFCluster* tofClusIn = (AliESDTOFCluster*)esdTOFClArrIn->At(ict);
    tofClusIn->SetEvent(evIn);
    int ntm = tofClusIn->GetNMatchableTracks();
    AliESDTOFCluster tofClusOut;
    tofClusOut.SetEvent(evOut);
    for (int itm=0;itm<ntm;itm++) {
      int trIDin = tofClusIn->GetTrackIndex(itm);
      if (trStatus[trIDin]==-1) continue; // discarded track
      const AliESDTOFMatch* mtcIn = tofClusIn->GetTOFMatch(itm);
      double timesTOF[AliPID::kSPECIESC];
      mtcIn->GetIntegratedTimes(timesTOF);
      if (!tofClusOut.Update(trStatus[trIDin], mtcIn->GetDx(), mtcIn->GetDy(),
			     mtcIn->GetDz(), mtcIn->GetTrackLength(), timesTOF )) {
	// register in the track
	AliESDtrack* trcMatched = evOut->GetTrack(trStatus[trIDin]);
	trcMatched->ReplaceTOFClusterID(ict,ntofClAdd);	
      }      
    }
    if (!tofClusOut.GetNMatchableTracks()) continue; // no matches
    AliESDTOFCluster* clnew = new ((*esdTOFClArrOut)[ntofClAdd]) AliESDTOFCluster(tofClusOut);
    clnew->SetESDID(ntofClAdd);
    // add hits
    for (int ih=0;ih<tofClusIn->GetNTOFhits();ih++) {
      int ihID = tofClusIn->GetHitIndex(ih);
      AliESDTOFHit* tofHIn = (AliESDTOFHit*)esdTOFHitArrIn->At(ihID);
      AliESDTOFHit* tofHNew = new ((*esdTOFHitArrOut)[ntofHAdd]) AliESDTOFHit(*tofHIn);
      tofHNew->SetESDTOFClusterIndex(ntofClAdd);
      clnew->AddESDTOFHitIndex(ntofClAdd);
      ntofHAdd++;
    }
    ntofClAdd++;
  }

  // copy ALL TRD online tracklets and tracks
  for (int i=0;i<evIn->GetNumberOfTrdTracks();i++) evOut->AddTrdTrack(evIn->GetTrdTrack(i));
  for (int i=0;i<evIn->GetNumberOfTrdTracklets();i++) evOut->AddTrdTracklet(evIn->GetTrdTracklet(i));

  // copy ALL Muon clusters
  for (int i=0;i<evIn->GetNumberOfMuonClusters();i++) {
    const AliESDMuonCluster* clIn = evIn->GetMuonCluster(i);
    AliESDMuonCluster* clOut = evOut->NewMuonCluster();
    *clOut = *clIn;
  }
  // copy ALL Muon tracks
  for (int i=0;i<evIn->GetNumberOfMuonTracks();i++) {
    const AliESDMuonTrack* trIn = evIn->GetMuonTrack(i);
    AliESDMuonTrack* trOut = evOut->NewMuonTrack();
    *trOut = *trIn;
  }
  // copy ALL Muon pads
  for (int i=0;i<evIn->GetNumberOfMuonPads();i++) {
    const AliESDMuonPad* pdIn = evIn->GetMuonPad(i);
    AliESDMuonPad* pdOut = evOut->NewMuonPad();
    *pdOut = *pdIn;
  }
  // copy ALL Global Muon tracks
  for (int i=0;i<evIn->GetNumberOfMuonGlobalTracks();i++) {
    const AliESDMuonGlobalTrack* trIn = evIn->GetMuonGlobalTrack(i);
    AliESDMuonGlobalTrack* trOut = evOut->NewMuonGlobalTrack();
    *trOut = *trIn;
  }

  // filter PMD tracks
  for (int i=0;i<evIn->GetNumberOfPmdTracks();i++) {
    const AliESDPmdTrack* trIn = evIn->GetPmdTrack(i);
    if ( KeepTrack(trIn->GetClusterTrackNo()) ) evOut->AddPmdTrack(trIn);
  }

  *(const_cast<AliESDRun*>(evOut->GetESDRun())) = *evIn->GetESDRun();

  *(const_cast<AliESDHeader*>(evOut->GetHeader())) = *evIn->GetHeader();

  *(const_cast<AliESDZDC*>(evOut->GetESDZDC())) = *evIn->GetESDZDC();

  *(const_cast<AliESDFMD*>(evOut->GetFMDData())) = *evIn->GetFMDData();

  *(const_cast<AliESDVZERO*>(evOut->GetVZEROData())) = *evIn->GetVZEROData();

  *(const_cast<AliESDTZERO*>(evOut->GetESDTZERO())) = *evIn->GetESDTZERO();

  *(const_cast<AliESDFIT*>(evOut->GetESDFIT())) = *evIn->GetESDFIT();

  *(const_cast<AliESDVertex*>(evOut->GetPrimaryVertexTPC())) = *evIn->GetPrimaryVertexTPC();
  { // must be set after after copying the tracks
    AliESDVertex* vtx = (AliESDVertex*)evOut->GetPrimaryVertexTPC();
    int nindIn = vtx->GetNIndices(), nindOut=0;
    UShort_t indOut[nindIn],*indIn=vtx->GetIndices();
    for (int i=0;i<nindIn;i++) if ( trStatus[indIn[i]]!=-1 ) indOut[nindOut++] = trStatus[indIn[i]];
    vtx->SetIndices(nindOut, indOut);
  }
    
  *(const_cast<AliESDVertex*>(evOut->GetPrimaryVertexSPD())) = *evIn->GetPrimaryVertexSPD();

  *(const_cast<AliESDVertex*>(evOut->GetPrimaryVertexTracks())) = *evIn->GetPrimaryVertexTracks();
  { // must be set after after copying the tracks
    AliESDVertex* vtx = (AliESDVertex*)evOut->GetPrimaryVertexTracks();
    int nindIn = vtx->GetNIndices(), nindOut=0;
    UShort_t indOut[nindIn],*indIn=vtx->GetIndices();
    for (int i=0;i<nindIn;i++) if ( trStatus[indIn[i]]!=-1 ) indOut[nindOut++] = trStatus[indIn[i]];
    vtx->SetIndices(nindOut, indOut);
  }
  
  *(const_cast<AliMultiplicity*>(evOut->GetMultiplicity())) = *evIn->GetMultiplicity();
  *(const_cast<AliESDCaloTrigger*>(evOut->GetCaloTrigger("PHOS"))) = *evIn->GetCaloTrigger("PHOS");
  *(const_cast<AliESDCaloTrigger*>(evOut->GetCaloTrigger("EMCAL"))) = *evIn->GetCaloTrigger("EMCAL");
  *(const_cast<AliESDACORDE*>(evOut->GetACORDEData())) = *evIn->GetACORDEData();
  *(const_cast<AliESDAD*>(evOut->GetADData())) = *evIn->GetADData();
  *(const_cast<AliESDTrdTrigger*>(evOut->GetTrdTrigger())) = *evIn->GetTrdTrigger();

  TClonesArray *plpVtx = (TClonesArray*)evOut->GetPileupVerticesSPD();
  for (int i=0;i<evIn->GetNumberOfPileupVerticesSPD();i++) {
    new ((*plpVtx)[i]) AliESDVertex( *evIn->GetPileupVertexSPD(i) );
  }
  // must be set after after copying the tracks
  plpVtx = (TClonesArray*)evOut->GetPileupVerticesTracks();
  for (int iv=0;iv<evIn->GetNumberOfPileupVerticesTracks();iv++) {
    new ((*plpVtx)[iv]) AliESDVertex( *evIn->GetPileupVertexTracks(iv) );
    AliESDVertex* vtx = (AliESDVertex*)evOut->GetPileupVertexTracks(iv);
    // fix indices
    int nindIn = vtx->GetNIndices(), nindOut=0;
    UShort_t indOut[nindIn],*indIn=vtx->GetIndices();
    for (int i=0;i<nindIn;i++) if ( trStatus[indIn[i]]!=-1 ) indOut[nindOut++] = trStatus[indIn[i]];
    vtx->SetIndices(nindOut, indOut);
  }
  
  // fix track indices for V0
  nv0 = evOut->GetNumberOfV0s();
  for (int iv=0;iv<nv0;iv++) {
    AliESDv0* v0 = evOut->GetV0(iv);
    for (int i=0;i<2;i++) v0->SetIndex(i,trStatus[v0->GetIndex(i)]);
  }
  // fix track indices for Cascades
  ncasc = evOut->GetNumberOfCascades();
  for (int ic=0;ic<ncasc;ic++) {
    AliESDcascade* casc = evOut->GetCascade(ic);
    casc->AliESDv0::SetIndex(0, trStatus[casc->GetNindex()]);
    casc->AliESDv0::SetIndex(1, trStatus[casc->GetPindex()]);
    casc->SetIndex(trStatus[casc->GetBindex()]);
  }
  // fix track indices for Kinks
  nknk = evOut->GetNumberOfKinks();
  for (int ik=0;ik<nknk;ik++) {
    AliESDkink *kink = evOut->GetKink(ik);
    kink->SetIndex(trStatus[kink->GetIndex(0)],0);
    kink->SetIndex(trStatus[kink->GetIndex(1)],1);
  }
  if (evIn->GetTOFHeader()) evOut->SetTOFHeader(evIn->GetTOFHeader());

  evOut->SetDetectorStatus(evIn->GetDetectorStatus());
  evOut->SetDAQDetectorPattern(evIn->GetDAQDetectorPattern());
  evOut->SetNumberOfTPCClusters(evIn->GetNumberOfTPCClusters());
  evOut->SetNumberOfESDTracks(evIn->GetNumberOfESDTracks());  

  evOut->SetEventNumberInFile(evIn->GetEventNumberInFile());
  evOut->SetEventSpecie(evIn->GetEventNumberInFile());

}

//_______________________________________________________________________________
Bool_t KeepTrack(AliESDtrack* trc)
{
  int lbl = trc->GetLabel();
  return KeepTrack(lbl);
}

//_______________________________________________________________________________
Bool_t KeepTrack(int lbl)
{
  return TMath::Abs(lbl) >= AliMCEvent::BgLabelOffset() ? kFALSE : kTRUE;
}

//_______________________________________________________________________________
Bool_t RemoveBKGFromGalice(const char* outgaliceFName,const char* inpgaliceFName)
{
  TFile* fl = TFile::Open(inpgaliceFName);
  if (!fl || fl->IsZombie()) {
    printf("Error: failed to open input file %s\n",inpgaliceFName);
    return kFALSE;
  }
  if (!fl->Cp(outgaliceFName)) {
    printf("Error: failed to open output file %s\n",outgaliceFName);
    return kFALSE;
  }
  fl->Close();
  delete fl;
  
  TString bgObjName = "embeddingBKGPaths";
  fl = TFile::Open(outgaliceFName,"update");
  if (fl->Get(bgObjName.Data())) {
    printf("Removing bg.event reference %s\n",bgObjName.Data());
    bgObjName += ";*";
    fl->Delete(bgObjName.Data());
  }
  fl->Close();
  delete fl;
  return kTRUE;
}
