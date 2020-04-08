void cloneraw(const char* fileName) {

  // --- "Analysis part" ---
  // This has to be done in the analysis task that produces the TEntryList

  // ESD file and tree
  TFile fesd("AliESDs.root");
  TTree * tesd = (TTree*)fesd.Get("esdTree");

  // ESD event
  AliESDEvent * esd = new AliESDEvent();// The signal ESD object is put here
  esd->ReadFromTree(tesd);

  // TEntryList
  TEntryList fLocalList;
  fLocalList.SetTreeName("RAW");
  fLocalList.SetFileName(fileName);

  // Event loop
  Int_t nev = tesd->GetEntriesFast();  
  for (Int_t iev=0; iev<nev; iev++) {

    // Get ESD
    tesd->GetEntry(iev);

    AliESDVertex * vert = esd->GetPrimaryVertex();

    if (strstr(vert->GetTitle(),"VertexerTracks")) {
      cout << iev <<" OK" << endl;
      fLocalList.Enter(iev);
    }
  }

  fLocalList.OptimizeStorage();

  // --- End of "Analysis part"

  // --- "Clone part"

  // Simple example how we can clone the tree. Of course in rallity we have to use
  // chain of RAW files that corresponds exactly to the chain of ESDs. Since the GUID of
  // the RAW file is not stored in the ESD tag file, we will have to add it.

  // RAW file and tree
  TFile fraw("raw.root");
  TTree * traw = (TTree*)fraw.Get("RAW");

  traw->SetEntryList(&fLocalList);

  // We have to use CopyTree with empty selection, because only in this case
  // it goes through TreePlayer and takes into account the TEntryList
  TFile fraw1("raw1.root","recreate");
  TTree * traw1 = traw->CloneTree(0); //
  traw->CopyTree("");
  fraw1.Write();
  // --- End of "Clone" part
}
