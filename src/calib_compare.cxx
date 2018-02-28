/* Code to compare calibration files @DESY 2018-02-27
 * by Uwe.Kraemer@desy.de && Mengqing.Wu@desy.de
 * requiring files to compare: 
 *  - only compare TH1F with same objectnames, 
 *    see https://root.cern.ch/doc/master/classTDirectory.html
 */

#include <iostream>
#include <iomanip>
#include <math.h>
#include <fstream>
#include <string>
#include <sstream>

#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "TMultiGraph.h"
#include "TApplication.h"
#include "TGraphErrors.h"
#include "TGraph.h"
#include "TStyle.h"
#include "THStack.h"
#include "TKey.h"
#include "TClass.h"
#include "TObject.h"

#include "Data.h"
#include "DataRead.h"

using namespace std;

string trim(const string& str){
  /* 
   * simiple trimming to erase whitespaces at begin/end of a string; 
   */
  auto first = str.find_first_not_of(' ');
  if (string::npos == first)
    return str;
  
  size_t last = str.find_last_not_of(' ');
  return str.substr(first, (last - first + 1));
}


int main ( int argc, char **argv ) {
  bool printalot=true;
  ifstream confFile;
  std::vector<string> infilenames;
  std::vector<string> inlegends;
  string infilename, inlegend;
  int nlines=0;
  string line;
  string outfilename;

  TCanvas c1;
  int count = 0;

  if (argc < 2 || argc > 3) {
    cout << "Error: wrong arguments... \n"
	 << " [Usage]: ./calib_compare conf.txt [output.root]"<< endl;
    return 0;
  }

  if (argc == 3) outfilename = argv[2];
  else outfilename = "/scratch/data/tracker_test/analysis/comparison/calib_compare.root";
  if (printalot) cout << "[info] "<< outfilename <<endl;
  TFile *hfile = new TFile(outfilename.c_str(), "RECREATE");
    
  confFile.open(argv[1], std::ifstream::in);
  if (confFile.fail()) {
    cout<<"Unable to open the parameter files, please check: "<<argv[1]<<endl;
    exit(1);
  }
  
  // check how many files to compare by counting how many lines in confFile::
  while(getline(confFile, line)) {
    // break loop if eof or any unexpected error:
    if (!confFile.good()) break;
    line = trim(line);
    // skip empty line and commented line:
    if (line.empty()||line[0]=='#') continue;	  
    stringstream ss(line);
    ss >> infilename >>inlegend;
    
    infilenames.push_back(infilename);
    inlegends.push_back(inlegend);
    nlines++;
  }

  if (printalot) cout<<"[info] Number of samples to compare = "<< nlines <<endl;
  if (nlines==0) {
    cout<<"Error: no input files, check your config file"<<endl;
    return 0;
  }
  
  TFile* rootfiles[nlines];
  for (int i=0; i<nlines; i++){
    if (printalot) cout<< i<<" "<<infilenames[i]<<endl;
    rootfiles[i] = TFile::Open(infilenames[i].c_str(), "READ");
  }
  

  TKey* key0;
  TIter next0(rootfiles[0]->GetListOfKeys());
  //std::vector<TObject*> plots;
  std::vector<TH1F*> th1fs;

  // loop over all keys of 1st file as baseline:
  hfile->cd();
  while ( ( key0 = (TKey*)next0()) ) {
    th1fs.clear();
    if ((strcmp(key0->GetClassName(), "TH1F") != 0)) {
      //cout<<" key.GetName = "<< key0->GetName()<<"; title = "<<key0->GetTitle()<<endl;
      continue;
    }
      
    th1fs.push_back((TH1F*)key0->ReadObj());
    std::string title = key0->GetTitle();
    std::string name = key0->GetName();

    // get all the same name plots from other files:
    for (int iobj=1; iobj<nlines; iobj++){
      //TObject* iobj_ptr = rootfiles[iobj]->Get(name.c_str());
      TH1F* iobj_ptr = (TH1F*)rootfiles[iobj]->Get(name.c_str());
      if(iobj_ptr) th1fs.push_back(iobj_ptr);
      else;
    }

    // plot all plots to canvas c1 and write to output file
    THStack *hist_comp = new THStack(title.c_str(),title.c_str());
    // Attention: below method to find xmin/max ONLY for same bin setting histograms!
    vector<int> xminbin, xmaxbin;
    int ncolor = 0;
    for (auto &ith1f: th1fs){
      ith1f->SetLineWidth(2);
      ith1f->SetLineColor(1+ncolor);

      xminbin.push_back(ith1f->FindFirstBinAbove(0));
      xmaxbin.push_back(ith1f->FindLastBinAbove(0));

      hist_comp->Add(ith1f);
	
      ncolor++;
    }
    hist_comp->Draw("nostack");
    int xmin=*min_element(xminbin.begin(), xminbin.end()),
      xmax=*max_element(xmaxbin.begin(), xmaxbin.end());
    
    hist_comp->GetXaxis()->SetRangeUser(xmin, xmax);
    c1.Update();
    c1.Write(title.c_str());
    	
  }
  for (auto & fi:rootfiles ){
    fi->Close();
  }
  
  hfile->Close();
  cout << "Saving file to " << outfilename << endl;
  return 1;
}

