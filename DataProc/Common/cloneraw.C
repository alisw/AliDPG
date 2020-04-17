void cloneraw(const char* fileName, const char* chunkName) {

  // --- "Analysis part" ---
  // This has to be done in the analysis task that produces the TEntryList

  // from env variables
  Int_t runNumber = atoi(gSystem->Getenv("ALIEN_JDL_LPMRUNNUMBER"));
  Int_t year = atoi(gSystem->Getenv("ALIEN_JDL_LPMANCHORYEAR"));
  TString period = gSystem->Getenv("ALIEN_JDL_LPMPRODUCTIONTAG");
  TString esdDirectory = gSystem->Getenv("ALIEN_JDL_ESDREFNAME");
  
  // ESD file and tree
  TGrid::Connect("alien://");
  TString filename(TString::Format("alien:///alice/data/%d/%s/%09d/%s/%s/AliESDs.root", year, period.Data(), runNumber, esdDirectory.Data(), chunkName));
  cout << "Opening ESD file " << filename << endl;
  TFile* fesd = TFile::Open(TString::Format("%s", filename.Data()));
  //  TFile fesd("AliESDs.root");
  TTree * tesd = (TTree*)fesd->Get("esdTree");

  // ESD event
  AliESDEvent * esd = new AliESDEvent();// The signal ESD object is put here
  esd->ReadFromTree(tesd);

  // TEntryList
  TEntryList fLocalList;
  fLocalList.SetTreeName("RAW");
  fLocalList.SetFileName(fileName);

  // reading file with mapping
  ifstream inFile;
  inFile.open(TString::Format("ListOfFiles_%s_sorted.txt", period.Data()));
  if (!inFile) {
    cerr << "Unable to open file for mapping";
    exit(1);   // call system to stop
  }
  string line;
  bool found = false;
  while (getline(inFile, line)) {
    //cout << "line = " << line << endl;
    if (line.find(chunkName) != std::string::npos) {
      TString strTmp(line);
      TObjArray* arr = strTmp.Tokenize(":");
      TObjString* objstr = (TObjString*)arr->At(2);
      //cout << "token 2: " << objstr->String() << endl;
      Int_t nevToFilter = (objstr->String()).Atoi();
      fLocalList.Enter(nevToFilter);
      cout << "Found event " << nevToFilter << " for chunk " << chunkName << endl; 
      found = true;
      delete arr;
    }
    else if (found == true) break;
  }	

  fesd->Close();

  fLocalList.OptimizeStorage();

  // --- End of "Analysis part"

  // --- "Clone part"

  // Simple example how we can clone the tree. Of course in rallity we have to use
  // chain of RAW files that corresponds exactly to the chain of ESDs. Since the GUID of
  // the RAW file is not stored in the ESD tag file, we will have to add it.

  // RAW file and tree
  TFile fraw(fileName);
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
