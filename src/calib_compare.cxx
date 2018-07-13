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
#include "TLegend.h"

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

void TH1FStack(std::vector<TH1F*> th1f_arr);



//void loopdir (TDirectory *dir, int nlines, TFile* rootfiles[nlines], std::vector<string> inlegends, TCanvas c1, int count, std::vector<TH1F*> th1fs) {
	////loop on all keys of dir including possible subdirs
	////print a message for all keys with a class TH1
	
	//TIter next (dir->GetListOfKeys());
	//TKey* key;
	//while ((key = (TKey*)next())) {
		//if (strcmp(key->GetClassName(),"TH1F") ==0) {
			
			//th1fs.clear();
			/////*START: Legend definition*/
			//TLegend leg(0.55,0.30,0.85, 0.55);// was: 0.65,0.25,0.85,0.55 (change for tri-plot)
			//leg.SetTextSize(0.04); 
			//leg.SetTextFont(42); 
			//leg.SetTextColor(1);
			//leg.SetBorderSize(0);  //no border for legend 
			//leg.SetFillColor(0);  // white
			//leg.SetFillStyle(0); // transparent
			///*END: Legend definition*/
			
			//th1fs.push_back((TH1F*)key->ReadObj());
			//std::string title = key->GetTitle();
			//std::string name = key->GetName();
			
			///* get all plots from other files with same object name
			//* if not exist: nullptr in the vector, keep same size as the legend vector
			//*/
			//for (int iobj=1; iobj<nlines; iobj++){
			////TObject* iobj_ptr = rootfiles[iobj]->Get(name.c_str());
			//TH1F* iobj_ptr = (TH1F*)rootfiles[iobj]->Get(name.c_str());
			//th1fs.push_back(iobj_ptr);
			//}
			//if (th1fs.size()!=inlegends.size())
			//cout<<"[warn] Num of plots ("<< th1fs.size() <<") != Num of legends ("<< inlegends.size() <<")"<<endl;
			
			////--start - TH1F: plot all plots to canvas c1 and write to output file
			//THStack *hist_comp = new THStack(title.c_str(),title.c_str());
			//// Attention: below method to find xmin/max ONLY for same bin setting histograms!
			//vector<int> xminbin, xmaxbin;
			
			//for (unsigned int hh=0; hh<th1fs.size();hh++){
			
			//if (!th1fs[hh]) continue; // jump over nullptr
			
			//th1fs[hh]->SetLineWidth(2);
			//th1fs[hh]->SetLineColor(1+hh);
			
			//leg.AddEntry(th1fs[hh], inlegends[hh].c_str(), "lp");
			
			//xminbin.push_back(th1fs[hh]->FindFirstBinAbove(0));
			//xmaxbin.push_back(th1fs[hh]->FindLastBinAbove(0));
			
			//hist_comp->Add(th1fs[hh]);
			
			//}
			
			//hist_comp->Draw("nostack");
			//int xmin=*min_element(xminbin.begin(), xminbin.end()),
			//xmax=*max_element(xmaxbin.begin(), xmaxbin.end());
			
			//hist_comp->GetXaxis()->SetRangeUser(xmin, xmax);
			//leg.Draw("same");
			//c1.Update();
			//c1.Write(title.c_str());
			////--end - TH1F: plot all plots to canvas c1 and write to output file
		
			//count++;
			////printf (" key : %s is a %s in %s\n", key->GetName(),key->GetClassName(),dir->GetPath());
		//}
		//if (!strcmp(key->GetClassName(),"TDirectoryFile")) {
			//dir->cd(key->GetName());
			//TDirectory *subdir = gDirectory;
			//loopdir(subdir);
			//dir->cd();
		//}
	//}
//}

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
	
	if (argc != 2) {
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
	std::vector<TObject*> plots;
	std::vector<TH1F*> th1fs;
	// loop over all keys of 1st file as baseline:
	int count=0;
	hfile->cd();
	//loopdir(rootfiles[0], nlines, rootfiles, inlegends, c1, count);
	
	while ( ( key0 = (TKey*)next0()) ) {
		th1fs.clear();
		if ((strcmp(key0->GetClassName(), "TH1F") != 0)) {
		cout<<" key.GetName = "<< key0->GetName()<<"; title = "<<key0->GetTitle()<<endl;
		continue;
		}
	
		///*START: Legend definition*/
		TLegend leg(0.55,0.30,0.85, 0.55);// was: 0.65,0.25,0.85,0.55 (change for tri-plot)
		leg.SetTextSize(0.04); 
		leg.SetTextFont(42); 
		leg.SetTextColor(1);
		leg.SetBorderSize(0);  //no border for legend 
		leg.SetFillColor(0);  // white
		leg.SetFillStyle(0); // transparent
		/*END: Legend definition*/
		
		th1fs.push_back((TH1F*)key0->ReadObj());
		std::string title = key0->GetTitle();
		std::string name = key0->GetName();
		
		/* get all plots from other files with same object name
		* if not exist: nullptr in the vector, keep same size as the legend vector
		*/
		for (int iobj=1; iobj<nlines; iobj++){
		//TObject* iobj_ptr = rootfiles[iobj]->Get(name.c_str());
		TH1F* iobj_ptr = (TH1F*)rootfiles[iobj]->Get(name.c_str());
		th1fs.push_back(iobj_ptr);
		}
		if (th1fs.size()!=inlegends.size())
		cout<<"[warn] Num of plots ("<< th1fs.size() <<") != Num of legends ("<< inlegends.size() <<")"<<endl;
		
		//--start - TH1F: plot all plots to canvas c1 and write to output file
		THStack *hist_comp = new THStack(title.c_str(),title.c_str());
		// Attention: below method to find xmin/max ONLY for same bin setting histograms!
		vector<int> xminbin, xmaxbin;
		
		for (unsigned int hh=0; hh<th1fs.size();hh++){
		
		if (!th1fs[hh]) continue; // jump over nullptr
		
		th1fs[hh]->SetLineWidth(2);
		th1fs[hh]->SetLineColor(1+hh);
		
		leg.AddEntry(th1fs[hh], inlegends[hh].c_str(), "lp");
		
		xminbin.push_back(th1fs[hh]->FindFirstBinAbove(0));
		xmaxbin.push_back(th1fs[hh]->FindLastBinAbove(0));
		
		hist_comp->Add(th1fs[hh]);
		
		}
		
		hist_comp->Draw("nostack");
		int xmin=*min_element(xminbin.begin(), xminbin.end()),
		xmax=*max_element(xmaxbin.begin(), xmaxbin.end());
		
		hist_comp->GetXaxis()->SetRangeUser(xmin, xmax);
		leg.Draw("same");
		c1.Update();
		c1.Write(title.c_str());
		//--end - TH1F: plot all plots to canvas c1 and write to output file
		
		count++;
	}
	
	for (auto & fi:rootfiles ){
		fi->Close();
	}

  hfile->Close();
  cout << "Saving file to " << outfilename << endl;
  return 1;
}

