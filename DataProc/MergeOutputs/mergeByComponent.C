
void CopyCPass(const char* alienFileList="alien.list", const char* outputFileList="local.list", Int_t timeOut=10);
void MergeCPass(const Char_t *list, TString component, TString outputFileName="CalibObjects.root");
void MakeFileList(const char *searchdir, const char *pattern, const char* outputFileName="calib.list", Int_t timeOut=10);

void mergeByComponent(TString       component,
                      const Char_t* fileList="calib.list",
                      const Char_t* apath=0, 
                      const Char_t* apattern=0, 
                      Int_t         fileDownloadTimeOut=10, 
                      const Char_t* localFileList="calib.list",
                      const Char_t* mergedFileName="CalibObjects.root" )
{
  // merging procedure
  // component can be COPY, MAKEALIENLIST, component name or ALL
  // ALL will merge the full file
  // selecting components will only merge the selected top level 
  //   objects in the file
  // COPY only copies the the alien files from fileList then 
  //   saves a list of local downloaded files in localFileList.
  // MAKEALIENLIST produces a list of files on alien, uses root
  //   to connect in case alien_xx utilities are not available 
  //   liek on the alien nodes
  /* load libs */
  printf("Executing mergeByComponent.C\n");
  gROOT->Macro("$ALIDPG_ROOT/DataProc/Common/LoadLibraries.C");
  TH1::AddDirectory(0);

  /* copy only */ 
  if (component == "COPY") {
    if (!apath || ! apattern){
      printf("Alien find path or pattern not specified");
      exit(1);
    }
    CopyCPass(fileList, localFileList, fileDownloadTimeOut);
    return;
  }
  /* make file list only */ 
  if (component == "MAKEALIENLIST") {
    if (!apath || ! apattern){
      printf("Alien find path or pattern not specified");
      exit(1);
    }
    MakeFileList(apath, apattern, fileList);
    return;
  }

  /* merge component */
  MergeCPass(fileList, component, mergedFileName);
}

//___________________________________________________________________

void MergeCPass(const Char_t *list, TString component, TString outputFileName)
{
  //AliTPCcalibTimeGain::SetMergeEntriesCut(2000000);
  //AliTPCcalibGainMult::SetMergeEntriesCut(2000000);
  //AliTPCcalibAlign::SetMergeEntriesCut(10000000);
  //AliTPCcalibTracks::SetMergeEntriesCut(10000000);
  //AliTPCcalibTime::SetResHistoMergeCut(10000000);

   AliTPCcalibTimeGain::SetMergeEntriesCut(500000);
   AliTPCcalibGainMult::SetMergeEntriesCut(500000);
   AliTPCcalibAlign::SetMergeEntriesCut(5000000);
   AliTPCcalibTracks::SetMergeEntriesCut(5000000);

  AliFileMerger merger;
  /* select what to merge */
  merger.SetNoTrees(kFALSE);
  if (component == "ALL")
    merger.AddReject("esdFriend");
  else
    merger.AddAccept(component.Data());
  //
  // temporary solution: reject THn and THnSparse of  TPCAlign directory, since they
  // will be merged using their owner AliTPCcalibAlign object
  {
    merger.AddReject("AliTPCcalibAlign.alignTPC.fClusterDelta_0");
    merger.AddReject("AliTPCcalibAlign.alignTPC.fClusterDelta_1");
    merger.AddReject("AliTPCcalibAlign.alignTPC.fTrackletDelta_0");
    merger.AddReject("AliTPCcalibAlign.alignTPC.fTrackletDelta_1");
    merger.AddReject("AliTPCcalibAlign.alignTPC.fTrackletDelta_2");
    merger.AddReject("AliTPCcalibAlign.alignTPC.fTrackletDelta_3");
  }
  //
  /* merge */
  merger.IterTXT(list, outputFileName.Data(), kFALSE);
  /* notify */
  gSystem->Exec(TString::Format("touch %s_merge_done", component.Data()));
  return;
}

//___________________________________________________________________
void MakeFileList(const char *searchdir, const char *pattern, const char* outputFileName, Int_t timeOut)
{
  gSystem->Setenv("XRDCLIENTMAXWAIT",TString::Format("%d",timeOut));
  gEnv->SetValue("XNet.RequestTimeout", timeOut);
  gEnv->SetValue("XNet.ConnectTimeout", timeOut);
  gEnv->SetValue("XNet.TransactionTimeout", timeOut);
  TFile::SetOpenTimeout(timeOut);

  TGrid::Connect("alien");

  TString command;
  command = TString::Format("find %s %s", searchdir, pattern);
  cerr<<"command: "<<command<<endl;
  TGridResult *res = gGrid->Command(command);
  if (!res) return;
  TIter nextmap(res);
  TMap *map = 0;

  ofstream outputFile;
  outputFile.open(outputFileName);

  //first identify the largest file and put it at the beginning
  Int_t largestFileSize=0;
  TString largestFile;
  TObject* largestObject;
  while((map=(TMap*)nextmap()))
  {
    TObjString *objs = dynamic_cast<TObjString*>(map->GetValue("turl"));
    TObjString *objsSize = dynamic_cast<TObjString*>(map->GetValue("size"));
    if (!objs || !objs->GetString().Length()) continue;
    if (!objsSize || !objsSize->GetString().Length()) continue;

    Int_t currentFileSize=objsSize->GetString().Atoi();
    if (currentFileSize>largestFileSize) 
    {
      largestFileSize=currentFileSize;
      largestFile=objs->GetString();
      largestObject=map;
    }
  }
  outputFile << largestFile.Data()<< endl;
  res->Remove(largestObject);
  
  //then write the rest of the entries to the file
  nextmap.Reset();
  while((map=(TMap*)nextmap()))
  {
    TObjString *objs = dynamic_cast<TObjString*>(map->GetValue("turl"));
    if (!objs || !objs->GetString().Length())
    {
      delete res;
      break;
    }

    TString src=TString::Format("%s",objs->GetString().Data());
    outputFile << src.Data()<< endl;
  }
  outputFile.close();
  return;
}

//___________________________________________________________________
void CopyCPass(const char* alienFileList, const char* outputFileList, Int_t timeOut)
{
  //copy all the alien files to local
  gSystem->Setenv("XRDCLIENTMAXWAIT",TString::Format("%d",timeOut));
  gEnv->SetValue("XNet.RequestTimeout", timeOut);
  gEnv->SetValue("XNet.ConnectTimeout", timeOut);
  gEnv->SetValue("XNet.TransactionTimeout", timeOut);
  TFile::SetOpenTimeout(timeOut);

  ifstream inputFile;
  inputFile.open(alienFileList);
  ofstream outputFile;
  outputFile.open(outputFileList);

  if (!inputFile.is_open())
  {
    printf("input file %s not found! exiting...\n",alienFileList);
    exit(1);
  }

  Int_t counter=0;
  while( inputFile.good())
  {
    TString src("");
    src.ReadLine(inputFile);
    if (src.IsNull()) continue;
    if (!counter && !gGrid) {
      TUrl url(src);
      if (TString(url.GetProtocol()) == "alien") {
        TGrid::Connect("alien");
      }
    }
    TString dst=src;
    dst.ReplaceAll("alien:///","");
    dst.ReplaceAll("/","_");
    if (dst.Length() > 240) {
      dst = src.MD5();
      dst.Append("_");
      dst.Append(gSystem->BaseName(src));
      cout << "Note: long file name " << src << " will be copied locally as " << dst << endl;
    }
    Bool_t result = TFile::Cp(src.Data(),dst.Data(),kTRUE);
    AliSysInfo::AddStamp(dst.Data(),counter, result);
    if (result) 
    {
      outputFile << dst.Data()<< endl;
      counter++;
    }
  }
  cout<<counter<<" files copied!"<<endl;

  inputFile.close();
  outputFile.close();
  gSystem->Exec("touch copy_done");
}

