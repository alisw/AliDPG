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
#endif

void ReadKine()
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
  Int_t nev = rl->GetNumberOfEvents();
  int codes[4] = {AliPID::ParticleCode(AliPID::kDeuteron), AliPID::ParticleCode(AliPID::kTriton), AliPID::ParticleCode(AliPID::kHe3), AliPID::ParticleCode(AliPID::kAlpha)};
  for (Int_t iev = 0; iev < nev; iev++)
  {
    rl->GetEvent(iev);
    AliStack *stack = rl->Stack();
    TArrayF mcVertex(3);
    rl->GetHeader()->GenEventHeader()->PrimaryVertex(mcVertex);
    TTree *treek = (TTree *)rl->TreeK();
    Int_t entries = (Int_t)treek->GetEntries();
    printf("Number of particles = %d\n", entries);
    for (Int_t i = 0; i < entries; i++)
    {
      TParticle *part = (TParticle *)stack->Particle(i);
      Int_t pdgCode = part->GetPdgCode();
      for (int iC = 0; iC < 4; ++iC) {
        if (pdgCode == codes[iC]) {
          if (stack->IsSecondaryFromMaterial(i)) {
	    printf("Particle with pdg=%d FOUND IN KINE TREE\n",pdgCode);
            exit(EXIT_SUCCESS);
          }
        }
      }
    }
  }
  exit(EXIT_FAILURE);
}
