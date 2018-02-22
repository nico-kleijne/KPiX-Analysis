#include <iostream>
#include <iomanip>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <TMultiGraph.h>
#include <TApplication.h>
#include <TGraphErrors.h>
#include <TGraph.h>
#include <TStyle.h>
#include <THStack.h>
#include <Data.h>
#include <DataRead.h>
#include <math.h>
#include <fstream>
#include <TKey.h>
#include <TClass.h>
#include <string.h>

int main ( int argc, char **argv ) {
	
	if (argc < 3) {
		cout << "Please add a pedestal file (argument 1) and a measurement file (argument 2)" << endl;
		return(1);
	}	
	
	// String replacement for output name
	
	//cout << "TEST" << endl;
	std::string output_name = argv[2];
	std::string output_name2 = argv[1]; 
	cout <<  output_name << endl;
	size_t position = output_name.find("calib/") + 6; 
	size_t position2 = output_name2.find("calib/") + 6; 
	output_name.replace(0,position,"");
	output_name2.replace(0,position2,"");
	cout <<  output_name << endl;
	cout <<  output_name2 << endl;
	position = output_name.find(".bin.root");
	position2 = output_name2.find(".bin.root");
	size_t end_of_string = output_name.size();
	size_t end_of_string2 = output_name2.size();
	output_name2.replace(position2, end_of_string2, "");
	output_name.replace(position, end_of_string, "");
	cout <<  output_name << endl;
	cout <<  output_name2 << endl;
	std::string output_name_final = output_name + "_compare_" + output_name2 + ".bin.root";
	std::string output_file = "/afs/desy.de/user/k/kraemeru/kpix_daq/data/" + output_name_final ;
	const char* output_file_char = output_file.c_str();
	cout << "Saving file to " << output_file << endl;
	
	TFile *f1 = TFile::Open(argv[1]);
	TFile *f2 = TFile::Open(argv[2]);
	TFile *hfile = new TFile(output_file_char, "RECREATE");
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
   	cout << "Saving file to " << output_file << endl;
}
