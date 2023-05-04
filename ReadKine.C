#include <TFile.h>
#include <TTree.h>
#include <TClonesArray.h>
#include <TParticle.h>
#include <TDirectoryFile.h>
#include <TMath.h>
#include <TLeaf.h>
#include <TH1F.h>
#include <iostream>
using namespace std;


void ReadKine()
{
  // Read the kinematics from the file
  // and print them out
  // 
  // Usage: root -l ReadKine.C
  //
  
  // Files
  TFile *f = TFile::Open("Kinematics.root");
  TFile outFile = TFile("ResoMCstudy.root", "RECREATE");

  Int_t counter = 0;
  Int_t particleCounter = 0;
  Int_t resonanceCounter = 0;
  Int_t nDresoToRightDecay = 0;
  std::vector <Int_t> nDresoToWrongDecay = {0, 0, 0};
  Int_t nDau = 10;

  for (auto key : *f->GetListOfKeys()) {

    counter++;
    //if (counter > 5) break; 
    // Directory
    cout << "Directory " << key->GetName() << endl;
    auto dir = (TDirectoryFile*)f->Get(key->GetName());

    // Tree
    auto tree = (TTree*)dir->Get("TreeK");
    TParticle *particles = nullptr;
    if (!tree) continue;

    // Setting branches
    tree->SetBranchAddress("Particles", &particles);
    

    bool isReso{false};
    Int_t nPartAfterReso = 0;
    for (Int_t iPart = 0; iPart < tree->GetEntriesFast(); iPart++) {
      tree->GetEntry(iPart);
      cout << "Part " << iPart << " - Part PDG: " << particles->GetPdgCode() << endl;
      particleCounter++;

      if (TMath::Abs(particles->GetPdgCode()) == 435 || TMath::Abs(particles->GetPdgCode()) == 10433) {
        cout << "========================" << endl;
        cout << iPart <<" Resonance found!" << endl;
        cout << "\tPDG code: " << particles->GetPdgCode() << endl;
        cout << "\tVertex: " << particles->Vx() << " " << particles->Vy() << " " << particles->Vz() << endl;
        cout << "\tMass: " << particles->GetMass() << endl;
        resonanceCounter++;
        isReso = true;


        // Check mothers
        Int_t imo = particles->GetFirstMother();
        cout << "\tMother: " << imo << endl;


        // Check daughters
        Int_t nDau = particles->GetNDaughters();
        cout << "\tNumber of daughters: " << nDau << endl;
        Int_t firstDau = particles->GetFirstDaughter();
        Int_t lastDau = particles->GetLastDaughter();
        for (Int_t iDau=firstDau; iDau<lastDau; ++iDau) {
          cout << "\t\tDaughter " << iDau << endl;
        }

      }
      if (isReso && nPartAfterReso < nDau) {
        if (nPartAfterReso == 1) {
            // Check the decay channel of the resonance
            if (TMath::Abs(particles->GetPdgCode()) == 411) {
              cout << "\t\tD+ decay found!" << endl;
              cout << "\t\t\tMother: " << particles->GetFirstMother() << endl;
              nDau = 7;
              nDresoToRightDecay++; // D+ decay
            } else if (TMath::Abs(particles->GetPdgCode()) == 421) {
              cout << "\t\tD0 decay found!" << endl;
              cout << "\t\t\tMother: " << particles->GetFirstMother() << endl;
              nDau = 5;
              nDresoToWrongDecay[0]++; // D0 decay
            } else if  (TMath::Abs(particles->GetPdgCode()) == 413) {
              cout << "\t\tD*+ decay found!" << endl;
              cout << "\t\t\tMother: " << particles->GetFirstMother() << endl;
              nDau = 9;
              nDresoToWrongDecay[1]++; // D* decay
            } else if (TMath::Abs(particles->GetPdgCode()) == 423) {
              cout << "\t\tD*(2007)0 decay found!" << endl;
              cout << "\t\t\tMother: " << particles->GetFirstMother() << endl;
              nDresoToWrongDecay[2]++; // D*(2007)0 decay
            } else {
              cout << "WARNING: Unrecognized D-reso meson decay: " << particles->GetPdgCode() << endl;
            }
        }
        cout << "\t\t--> PDG code: " << particles->GetPdgCode() << endl;
        cout << "\t\t--> Vertex: " << particles->Vx() << " " << particles->Vy() << " " << particles->Vz() << endl;
        cout << "\t\t--> Mother: " << particles->GetFirstMother() << endl;
        cout << "\t\t--> Status: " << particles->GetStatusCode() << endl;
        nPartAfterReso++;
        cout << "\t\tnPartAfterReso: " << nPartAfterReso << endl;
      }
      if (nPartAfterReso == nDau) {
        isReso=false;
        nPartAfterReso = 0;
        cout << "========================" << endl;
      }
    }

  }
  cout << "\n\n========================" << endl;
  cout << "Total number of events: " << counter << endl;
  cout << "Total number of particles: " << particleCounter << endl;
  cout << "Total number of resonances: " << resonanceCounter << endl;
  cout << "Total number of Ds to D+ decay  " << nDresoToRightDecay << endl;
  cout << "Total number of Ds to D0 decay: " << nDresoToWrongDecay[0] << endl;
  cout << "Total number of Ds to D*+ decay: " << nDresoToWrongDecay[1] << endl;
  cout << "Total number of Ds to D*(2007)0 decay: " << nDresoToWrongDecay[2] << endl;
  cout << "========================" << endl;
  outFile.Close();
}

