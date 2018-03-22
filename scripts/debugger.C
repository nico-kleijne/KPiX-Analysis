void debugger(){

  TFile* f1=TFile::Open("/scratch/data/tracker_test/2018_03_22_11_16_41.bin.root");
  TH1F* h1 = (TH1F*)f1->Get("General/Channel_entries_total");
  if (!h1) {
    cout<< "Either your file broken or your histogram name is shitty!"<<endl;
    return;
  }

// debug for the duplicate channel to strip id:
  int nbins = h1->GetSize();
  TAxis *xaxis = h1->GetXaxis();
  for (int nbin = 1; nbin<nbins-1; nbin++){
    auto strip_evts=h1->GetBinContent(nbin);

    if (strip_evts<3.8 && strip_evts!=0) {
      auto strip_ID=xaxis->GetBinCenter(nbin);
      cout<< "[debug] stripId = "<< strip_ID
	  << ", entries = "<<strip_evts<<endl;
    }

  }


 return;
}
