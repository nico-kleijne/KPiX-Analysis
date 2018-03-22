void debugger(){

  TFile* f1=TFile::Open("");
  TH1F* h1 = (TH1F*)f1->Get("Channel entires_total");
  if (!h1) {
    cout<< "Either your file broken or your histogram name is shitty!"<<endl;
    return;
  }

// debug for the duplicate channel to strip id:
  int nbins = h1->second->GetSize();
  TAxis *xaxis = h1->second->GetXaxis();
  for (int nbin = 1; nbin<nbins-1; nbin++){
    auto strip_evts=h1->second->GetBinContent(nbin);

    if (strip_evts<4) {
      auto strip_ID=xaxis->GetBinCenter(nbin);
      cout<< "[debug] stripId = "<< strip_ID
	  << ", entries = "<<strip_evts<<endl;
    }

  }


 return;
}
