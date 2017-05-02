void SetField(const char* rawdata);

void rec(const char *filename="raw.root")
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //
  // Reconstruction script for 2010 RAW data
  //
  /////////////////////////////////////////////////////////////////////////////////////////

  
  // Set the CDB storage location
  AliCDBManager * man = AliCDBManager::Instance();
  man->SetDefaultStorage("alien://folder=/alice/data/2017/OCDB");
  //man->SetDefaultStorage("local:///cvmfs/alice.cern.ch/calibration/data/2015/OCDB");

  //SetField(filename);
  AliReconstruction rec;

  // Set reconstruction flags (skip detectors here if neded with -<detector name>

  rec.SetRunReconstruction("ALL");

  // QA options
  //  rec.SetRunQA("Global:ESDs") ;
  //  rec.SetRunQA(":") ;
  //  rec.SetRunQA("ALL:ALL") ;
  rec.SetRunQA("Global MUON:ALL") ;

  rec.SetQARefDefaultStorage("local://$ALICE_ROOT/QAref") ;

  // AliReconstruction settings
  rec.SetWriteESDfriend(kTRUE);
  rec.SetWriteAlignmentData();
  rec.SetInput(filename);
  rec.SetUseTrackingErrorsForAlignment("ITS");

  // Upload CDB entries from the snapshot (local root file) if snapshot exist
  if (gSystem->AccessPathName("OCDB.root", kFileExists)==0) {
    rec.SetCDBSnapshotMode("OCDB.root");
  }

  // switch off cleanESD
  rec.SetCleanESD(kFALSE);

  //Ignore SetStopOnError
  rec.SetStopOnError(kFALSE);

 // Delete recpoints
  rec.SetDeleteRecPoints("TPC TRD");

  // Set 100% of friends
  rec.SetFractionFriends(1.0);

  AliLog::Flush();
  rec.Run();

}



void SetField(const char* rawdata)
{
  //run,L3_magnetCurrent,Dipole_magnetCurrent
  
  const Int_t bLUT[][3]={
    {211358, 30, 0}, {211383, 30, 0}, {211922,  0, 0}, {212985,-30, 0}, {213084,-30, 0},
    {214301,-30, 0}, {214382,-30,-6}, {214551,-30,-6}, {214695,  0, 0}, {214964,  0, 0},
    {216495, 30, 0}, {216967, 30, 0}, {216969, 30, 0}, {218368, 30, 0}, {218391, 30, 0},
    {218406, 30, 0}, {218577, 30, 0}, {218786,  0, 0}, {219756,  0, 0}, {219851,  0, 0},
    {220170,-30, 0}, {220507,  0, 6}, {220511,  0, 6}, {220515,  0, 6}, {220799,  0, 6},
    {220805,  0, 6}, {220807,  0, 6}, {220810,  0, 6}, {222242, 30, 6}, {222625,-30, 0},
    {222630,-30, 0}, {222650, 30, 6}, {222780, 30, 6}, {222791, 30, 6}, {222802, 30, 6},
    {223923, 30, 6}, {225122,-30,-6}, {225125,-30,-6}, {225143,-30,-6}, {225651,-30,-6},
    {225652,-30,-6}, {226234,-30,-6}, {226242,-30,-6}, {226287,-30,-6}, {226296,-30,-6},
    {226301,-30,-6}, {226349,-30,-6}, {226365,-30,-6}, {226368,-30,-6}, {227399, 30, 6},
    {227403, 30, 6}, {227428, 30, 6}, {227430, 30, 6}, {227432, 30, 6}, {227435, 30, 6},
    {227437, 30, 6}, {227516, 30, 6}, {227527, 30, 6}, {227551, 30, 6}, {227574, 30, 6},
    {227577, 30, 6}, {227578, 30, 6}, {227580, 30, 6}, {227582, 30, 6}, {227595, 30, 6},
    {227599, 30, 6}, {227606, 30, 6}, {227608, 30, 6}, {227610, 30, 6}, {227618, 30, 6},
    {227648, 30, 6}, {227678, 30, 6}, {227694, 30, 6}, {227702, 30, 6}, {227703, 30, 6},
    {227705, 30, 6}, {227706, 30, 6}, {227718, 30, 6}, {227719, 30, 6}, {227722, 30, 6},
    {227750, 30, 6}, {227764, 30, 6}, {227790, 30, 6}, {227809, 30, 6}, {227828,  0, 6},
    {227836,  0, 6}, {229462,  0, 6}, {229483,  0, 6}, {229522,  0, 6}, {229532,  0, 6},
    {229545,  0, 6}, {229596,  0, 6}, {230921, 30, 6}, {234057, 30, 6}, {234061, 30, 6},
    {234062, 30, 6}, {235074, 30, 6}, {235079, 30, 6}, {235083, 30, 6}, {236405, 30, 6},
    {236406, 30, 6}, {236407, 30, 6}, {236635, 30, 6}, {236639, 30, 6}, {237011, 30, 6},
    {237015, 30, 6}, {237608, 30, 6}, {237620, 30, 6}, {237730, 30, 6}, {237871, 30, 6},
    {237872, 30, 6}, {237873, 30, 6}, {237877, 30, 6}, {237891, 30, 6}, {238248, 30, 6},
    {238362, 30, 6}, {238977,-30,-6}, {238978,-30,-6}, {238982,-30,-6}, {238983,-30,-6},
    {238985,-30,-6}, {238988,-30,-6}, {239207, 30, 6}, {239360, 30, 6}, {239370, 30, 6},
    {239546, 30, 6}, {239552, 30, 6}, {239553, 30, 6}, {239558, 30, 6}, {239565, 30, 6},
    {239577, 30, 6}, {239617, 30, 6}, {239655, 30, 6}, {239891, 30, 6}, {239895, 30, 6},
    {239896, 30, 6}, {239899, 30, 6}, {239964, 30, 6}, {239969, 30, 6}, {239970, 30, 6},
    {239971, 30, 6}, {239972, 30, 6}, {239973, 30, 6}, {239990, 30, 6}, {240002, 30, 6},
    {244139, 30, 6}, {244198, 30, 6}, {244205, 30, 6}, {244209, 30, 6}, {244217, 30, 6},
    {244224, 30, 6}, {244226, 30, 6}};


  printf("ATTENTION: Setting magnetic field according to the LUT from the LogBook\n");
  TString rds = rawdata;
  if (!gGrid && rds.BeginsWith("alien://")) {
    TGrid::Connect("alien://");
    if (!gGrid) {
      printf("Failed to connect to alien\n"); 
      exit(1);
    }
  }
  //
  AliRawReader* reader = AliRawReader::Create(rds.Data());
  if (!reader) {
    printf("Failed to connect open %s\n",rds.Data()); 
    exit(1);
  }    
  //
  reader->NextEvent();
  int run = reader->GetRunNumber();
  delete reader;
  printf("Extracted run number %d from %s\n",rds.Data());
  //
  int nRunLUT = sizeof(bLUT)/sizeof(Int_t)/3;
  int id = -1;
  for (id=0;id<nRunLUT;id++) if (bLUT[id][0]==run) break;
  if (id<0||id>=nRunLUT) {
    printf("Did not find the run in the lookup table, reconstruction will rely Bfield from GRP\n");
    return;
  }
  //
  float l3Current = bLUT[id][1];
  float diCurrent = bLUT[id][2];
  printf("Will impose L3=%+.0f / DIP=%+.0f kA currents for run %d\n",l3Current,diCurrent,run);
  printf("ATTENTION: beam info will be absent in field initialization, not good for ZDC\n");

  const float defL3T05 = 30.;
  const float defL3T02 = 12.; 
  const float defDip   = 6.;
  const float zero = 0.077;

  // pick closest field map
  float w05 = TMath::Abs(1-TMath::Abs(l3Current)/defL3T05);
  float w02 = TMath::Abs(1-TMath::Abs(l3Current)/defL3T02);
  AliMagF::BMap_t map = w05<w02 ? (AliMagF::k5kG) : (AliMagF::k2kG);
  float sclL3 = TMath::Abs(l3Current)/(w05<w02 ? defL3T05:defL3T02) * TMath::Sign(1.,l3Current);
  float sclDip = TMath::Abs(diCurrent/defDip) * TMath::Sign(1.,diCurrent);
  if (TMath::Abs(l3Current)<zero) sclL3 = 0;
  if (TMath::Abs(diCurrent)<zero) sclDip = 0;  

  AliMagF* fld =  new AliMagF("bmap","customAliMagF",sclL3,sclDip,map);
  fld->SetBit(AliMagF::kOverrideGRP);
  //
  TGeoGlobalMagField::Instance()->SetField( fld );
  TGeoGlobalMagField::Instance()->Lock();
  //
}
