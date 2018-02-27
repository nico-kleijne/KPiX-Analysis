/* Code to compare calibration files
 * by Uwe.Kraemer@desy.de && Mengqing.Wu@desy.de
 * requiring files to compare: 
 *  - same root file structure: same histo names etc
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
	
	if (argc < 2) {
	  cout << "Wrong arguments: \n"
	       << " [Usage]: ./calib_compare conf.txt [optional]"<< endl;
	  return 0;
	}	
	//-- String replacement for output name
	//-- start mengqing
	ifstream confFile;
	confFile.open(argv[1], std::ifstream::in);
	if (confFile.fail()) {
	  cout<<"Unable to open the parameter files, please check: "<<argv[1]<<endl;
	  exit(1);
	}
	// check how many files to compare by counting how many lines in confFile::
	std::vector<string> infilenames;
	std::vector<string> inlegends;
	string infilename, inlegend;
	
	int nlines=0;
	std::string line;
	while(getline(confFile, line)) {
	  if (!confFile.good()) break;
	  line = trim(line);

	  if (line.empty()||line[0]=='#') continue;	  
	  stringstream ss(line);
	  ss >> infilename >>inlegend;

	  infilenames.push_back(infilename);
	  inlegends.push_back(inlegend);
	  nlines++;
	}

	TFile* inrootf[nlines];
	for (int i=0; i<nlines; i++){
	  //cout<< i<<" "<<infilenames[i]<<endl;
	  inrootf[i] = TFile::Open(infilenames[i].c_str());
	}
	
	//-- end mengqing

	//
	//TFile *f1 = TFile::Open(argv[1]);
	//TFile *f2 = TFile::Open(argv[2]);
	auto f1 = inrootf[0];
	auto f2 = inrootf[1];
	TFile *hfile = new TFile("test.root", "RECREATE");
	TIter next1(f1->GetListOfKeys());
	
	TKey *key1;
	
	TCanvas c1;
	int count = 0;
	while ((key1 = (TKey*)next1())) {
		TIter next2(f2->GetListOfKeys());
		TKey *key2;
		while ((key2 = (TKey*)next2())) {
			std::string title = key1->GetTitle();
			if ((strcmp(key1->GetTitle(), key2->GetTitle()) == 0)  && (strcmp(key2->GetClassName(), "TH1F") == 0) && (title.find("charge") == std::string::npos)) {
					//cout << "Current key1 = " << count << " == " << title << endl;
					TH1F *h1 = (TH1F*)key1->ReadObj();
					TH1F *h2 = (TH1F*)key2->ReadObj();
					THStack *hist_comp = new THStack();
					h1->SetLineWidth(2);
					h2->SetLineWidth(2);
					h1->SetLineColor(kBlack);
					h2->SetLineColor(kBlue);
					int first_h1 = h1->FindFirstBinAbove(0);
					int last_h1 = h1->FindLastBinAbove(0);
					int first_h2 = h2->FindFirstBinAbove(0);
					int last_h2 = h2->FindLastBinAbove(0);
					int xmin;
					int xmax;
					if (first_h1 <= first_h2) {
						xmin = first_h1;
					}
					else {
						xmin = first_h2;
					}
					if (last_h1 >= last_h2) {
						xmax = last_h1;
					}
					else {
						xmax = last_h2;
					}
					//h1->GetFunction("myFunction")->SetBit(TF1::kNotDraw);
					//h2->GetFunction("myFunction")->SetBit(TF1::kNotDraw);
					hist_comp->Add(h1);
					hist_comp->Add(h2);
					hist_comp->Draw("nostack");
					//h1->Draw();
					//h2->Draw("same");
					//hist_comp->Draw("sameaxis");
					//gPad->RedrawAxis();
					hist_comp->GetXaxis()->SetRangeUser(xmin, xmax);
					c1.Write(key1->GetTitle());
					
					count++;
			}
			//else if ((strcmp(key1->GetTitle(), key2->GetTitle()) == 0)  && (strcmp(key2->GetClassName(), "TH1F") == 0) && (title.find("charge") != std::string::npos))
			//{
				    ////cout << "Current key1 = " << count << " == " << title << endl;
					//TH1F *h1 = (TH1F*)key1->ReadObj();
					//TH1F *h2 = (TH1F*)key2->ReadObj();
					//THStack *hist_comp = new THStack();
					//h1->SetLineWidth(2);
					//h2->SetLineWidth(2);
					//h1->SetLineColor(kBlack);
					//h2->SetLineColor(kBlue);
					//double first_h1 = (h1->FindFirstBinAbove(0))* (2000.0/8192) -1;
					//double last_h1 = (h1->FindLastBinAbove(0))* (2000.0/8192) +1;
					//double first_h2 = (h2->FindFirstBinAbove(0))* (2000.0/8192) -1;
					//double last_h2 = (h2->FindLastBinAbove(0))* (2000.0/8192) -1;
					//double xmin;
					//double xmax;
					//if (first_h1 <= first_h2) {
						//xmin = first_h1;
					//}
					//else {
						//xmin = first_h2;
					//}
					//if (last_h1 >= last_h2) {
						//xmax = last_h1;
					//}
					//else {
						//xmax = last_h2;
					//}
					////h1->GetFunction("myFunction")->SetBit(TF1::kNotDraw);
					////h2->GetFunction("myFunction")->SetBit(TF1::kNotDraw);
					//hist_comp->Add(h1);
					//hist_comp->Add(h2);
					//hist_comp->Draw("nostack");
					////h1->Draw();
					////h2->Draw("same");
					////hist_comp->Draw("sameaxis");
					////gPad->RedrawAxis();
					//hist_comp->GetXaxis()->SetRangeUser(xmin, xmax);
					//c1.Write(key1->GetTitle());
					
					//count++;
					
			//}
		}
	}
   f1->Close();
   f2->Close();
   hfile->Close();
   cout << "Saving file to " << "test.root" << endl;
   return 1;
}
