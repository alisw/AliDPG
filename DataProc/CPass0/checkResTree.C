#if !(defined(__CLING__)  || defined(__CINT__)) || defined(__ROOTCLING__) || defined(__ROOTCINT__)
#include <TTree.h>
#include <TFile.h>
#endif

void checkResTree(const char* fname = "ResidualTrees.root", const char* tname = "delta")
{
  TFile tf(fname);
  TTree* tr = (TTree*)tf.Get(tname); // we want macro to crash if the tree is not there
  int nev = tr->GetEntries();
  int rdb = 0;
  bool corrupted = kFALSE;
  for (int i=0;i<nev;i++) {
    rdb = tr->GetEntry(i);
    if (rdb<=0 || rdb>10000) {
      printf("CORRUPTED Residuals file, ev %d got %d bytes\n",i,rdb);
      corrupted = kTRUE;
    } 
  }
  if (corrupted) {
    printf("The residual tree file is corrupted\n");
  }
  else {
    printf("The residual tree file is OK\n");
  }
}
