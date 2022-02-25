void draw(float binw = 0.5){
  FILE *f = fopen("valuesToBePlotted","r");

  float range = 180;

  TH1F *h = new TH1F("h","|#theta - 90#circ| < 5#circ, |z_{vertex}| < 0.1 cm;radius (cm); <density> (g/cm^{3})",range/binw,0,range);
  h->SetStats(0);

  int n=1;
  float a;
  while(fscanf(f,"%f",&a) ==1){
    h->SetBinContent(n,a/binw);
    printf("%d %f\n",n,a);
    n++;
  }

  h->Draw();

  TFile *fout = new TFile("mb.root","RECREATE");
  h->Write();
  fout->Close();
}
