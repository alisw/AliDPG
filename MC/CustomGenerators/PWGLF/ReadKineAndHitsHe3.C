#if !defined(__CINT__) || defined(__MAKECINT__)
#include <Riostream.h>
#include <AliRun.h>
#include <TParticle.h>
#include <TString.h>
#include <TTree.h>
#include <TArrayF.h>
#include <AliStack.h>
#include <AliRunLoader.h>
#include <TMath.h>
#include <AliHeader.h>
#include <AliGenEventHeader.h>
#include <TH1F.h>
#include <TNtuple.h>
#include <TFile.h>
#include <AliESD.h>
#include <AliESDVertex.h>
#include <AliITShit.h>
#include <AliITSgeomTGeo.h>
#include <AliTPCTrackHitsV2.h>
#include <AliTPC.h>
#endif

void ReadKineAndHitsHe3()
{
  if (gAlice)
  {
    delete gAlice;
    gAlice = 0x0;
  }

  AliRunLoader *rl = AliRunLoader::Open("galice.root");
  if (rl == 0x0)
  {
    std::cerr << "Can not open session" << std::endl;
    return;
  }

  rl->LoadHeader();
  rl->LoadKinematics();
  rl->LoadHits();
  Int_t nev = rl->GetNumberOfEvents();
  int triggerPDG = AliPID::ParticleCode(AliPID::kHe3);
  for (Int_t iev = 0; iev < nev; iev++){
    rl->GetEvent(iev);
    AliStack *stack = rl->Stack();
    TArrayF mcVertex(3);
    rl->GetHeader()->GenEventHeader()->PrimaryVertex(mcVertex);
    TTree *treek = (TTree *)rl->TreeK();
    TTree *treeHits = (TTree *)rl->GetTreeH("ITS",kFALSE);
    TTree *treeHtpc = (TTree *)rl->GetTreeH("TPC",kFALSE);
    TClonesArray *arritsh=new TClonesArray("AliITShit",10000);
    treeHits->SetBranchAddress("ITS",&arritsh);
    AliTPCTrackHitsV2 *htpc= new AliTPCTrackHitsV2;
    TBranch* branchHtpc=treeHtpc->GetBranch("TPC2");
    branchHtpc->SetAddress(&htpc);

    Int_t entries = (Int_t)treek->GetEntries();
    printf("Number of particles = %d\n", entries);
    for (Int_t iPart = 0; iPart < entries; iPart++){
      TParticle *part = (TParticle *)stack->Particle(iPart);
      Int_t pdgCode = part->GetPdgCode();
      if (pdgCode == triggerPDG && stack->IsSecondaryFromMaterial(iPart)) {
	Double_t rap=part->Y();
	if(TMath::Abs(rap)<1.){
	  Float_t vx = part->Vx();
	  Float_t vy = part->Vy();
	  Float_t vz = part->Vz();
	  Float_t r  = TMath::Sqrt(vx * vx + vy * vy);
	  Double_t minRadius=99999.;
	  Double_t maxRadius=0.;
	  Int_t hitInLay=0;
	  Int_t nPrimTracks =(Int_t) treeHits->GetEntries();
	  Int_t nPrimTracksTPC =(Int_t) treeHtpc->GetEntries();
	  for(Int_t iPrimTrack=0; iPrimTrack<nPrimTracks; iPrimTrack++){
	    Int_t nBytes = treeHits->GetEvent(iPrimTrack);
	    if (nBytes <= 0) continue;
	    Int_t nHits = arritsh->GetEntriesFast();
	    for (Int_t ih=0; ih<nHits; ih++) {
	      AliITShit *itsHit=(AliITShit*)arritsh->UncheckedAt(ih);
	      if(itsHit->GetTrack()==iPart){
		Double_t xg,yg,zg,tof;
		itsHit->GetPositionG(xg,yg,zg,tof);
		Int_t lay,lad,det;
		AliITSgeomTGeo::GetModuleId(itsHit->GetModule(),lay,lad,det);
		Double_t rhit= TMath::Sqrt(xg*xg+yg*yg);
		if(rhit<minRadius) minRadius=rhit;
		if(rhit>maxRadius) maxRadius=rhit;
		hitInLay|=(1<<(lay-1));
		//		    printf("Hit %d %d  track %d module %d lay %d coord = %f %f %f rad=%f\n",iPrimTrack,ih,itsHit->GetTrack(),itsHit->GetModule(),lay,xg,yg,zg,rhit);
	      }
	    }
	  }
	  Int_t nITShits=0;
	  for(Int_t iLay=0; iLay<6; iLay++) if(hitInLay & (1<<iLay)) nITShits++;
	  Int_t nTPChits=0;
	  for(Int_t iPrimTrack=0; iPrimTrack<nPrimTracksTPC; iPrimTrack++){
	    Int_t nBytes = branchHtpc->GetEvent(iPrimTrack);
	    if (nBytes <= 0) continue;
	    TClonesArray* arrtpch=htpc->GetArray();
	    Int_t nHtpc = arrtpch->GetEntriesFast();
	    for (Int_t ih=0; ih<nHtpc; ih++) {
	      AliTPChit *tpcHit=(AliTPChit*)arrtpch->UncheckedAt(ih);
	      if(tpcHit->GetTrack()==iPart){
		Double_t xg=tpcHit->X();
		Double_t yg=tpcHit->Y();
		Double_t zg=tpcHit->Z();
		Double_t rhit= TMath::Sqrt(xg*xg+yg*yg);
		if(rhit>50) { // Skip hits at interaction point
		  //		      printf("Hit %d in TPC = %f %f\n",ih,rhit,zg);
		  if(rhit<minRadius) minRadius=rhit;
		  if(rhit>maxRadius) maxRadius=rhit;
		  nTPChits++;
		}
	      }
	    }
	  }
	  printf("Particle with pdg=%d and y=%f and r=%f  z=%f  FOUND IN KINE TREE\n",pdgCode,rap,r,vz);
	  printf("nITS hits = %d  nTPChits = %d min hit radius = %f  max hit radius = %f\n",nITShits,nTPChits,minRadius,maxRadius);
	  if(nITShits>=2 && maxRadius>140){
	    printf("--> accepted\n");
	    delete arritsh;
	    delete htpc;
	    exit(EXIT_SUCCESS);
	  }else{
	    printf("--> not enough hits and length\n");
	  }
	}
      }
    }
    delete arritsh;
    delete htpc;
  }
  exit(EXIT_FAILURE);
}
