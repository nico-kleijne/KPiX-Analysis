#include <iostream>
#include <iomanip>
#include <TFile.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TMultiGraph.h>
#include <TApplication.h>
#include <TGraphErrors.h>
#include <TGraph.h>
#include <TStyle.h>
#include <TVector.h>
#include <Data.h>
#include <DataRead.h>
#include <math.h>
#include <fstream>
#include <TKey.h>
#include <TClass.h>
#include <string.h>

int main ( int argc, char **argv ) {

	TH1::SetDefaultSumw2();
	if (argc < 3) {
		cout << "Please add a pedestal file (argument 1) and a measurement file (argument 2)" << endl;
		return(1);
	}	
	
	// String replacement for output name
	

	std::string output_name = argv[2];
	cout <<  output_name << endl;
	size_t position = output_name.find("data/") + 5; 
	output_name.replace(0,position,"");
	cout <<  output_name << endl;
	position = output_name.find(".bin.root");
	size_t end_of_string = output_name.size();
	output_name.replace(position, end_of_string, "_ped_subtr.bin.root");
	cout <<  output_name << endl;
	std::string output_file = "/data/"+output_name;
	const char* output_file_char = output_file.c_str();
	
	// ====================================
	
	TFile *ped = TFile::Open(argv[1]);
	TFile *meas = TFile::Open(argv[2]);
	TFile *hfile = new TFile(output_file_char, "RECREATE");
	TIter next1(ped->GetListOfKeys());
	TKey *key1;
	//TCanvas c1;
	TH1F *monster_hist_k30 = (TH1F*)meas->Get("trig_count_k30_total");
	TH1F *monster_hist_k28 = (TH1F*)meas->Get("trig_count_k28_total");
	TH1F *monster_hist_k26 = (TH1F*)meas->Get("trig_count_k26_total");
	double monster_num[3] = {0};
	for (int c = 900; c<1023; c++)
	{
		monster_num[2] = monster_num[2]+monster_hist_k30->GetBinContent(c);
		monster_num[1] = monster_num[1]+monster_hist_k28->GetBinContent(c);
		monster_num[0] = monster_num[0]+monster_hist_k26->GetBinContent(c);
	}
	cout << "Number of normed monster events in k26, k28, k30 = " << monster_num[0] << ", " << monster_num[1] << ", " << monster_num[2] << endl;
	
	while ((key1 = (TKey*)next1())) {
		TIter next2(meas->GetListOfKeys());
		TKey *key2;
		while ((key2 = (TKey*)next2())) {
			
			if ((strcmp(key1->GetName(), key2->GetName()) == 0)  && (strcmp(key2->GetClassName(), "TH1F") == 0)) {
				//cout << "Current key1 = " << count << " == " << key1->GetClassName() << endl;
				TH1F *h_ped = (TH1F*)key1->ReadObj();
				TH1F *h_meas = (TH1F*)key2->ReadObj();
				//for (int count = 0; count <= h_ped->GetEntries(); ++count)
				//{
					
				//cout << h_ped->GetBinContent(335) << endl;
				TH1F *result = new TH1F(key1->GetName(), key1->GetName(), (h_ped->GetSize())-2,0,h_ped->GetSize());
				string str = key1->GetName();
				string str2 = key2->GetName();
				for (int k = 0; k < h_ped->GetSize(); ++k)
				{
					double sum;
					
					if (str.find("entries_k_28_total") != string::npos && str2.find("entries_k_28_total") != string::npos) 
					{
						sum = (h_meas->GetBinContent(k) - h_ped->GetBinContent(k) - monster_num[1]);
					}
					else if (str.find("entries_k_26_total") != string::npos && str2.find("entries_k_26_total") != string::npos) 
					{
						sum = (h_meas->GetBinContent(k) - h_ped->GetBinContent(k) - monster_num[0]);
					}
					else if (str.find("entries_k_30_total") != string::npos && str2.find("entries_k_30_total") != string::npos) 
					{
						sum = (h_meas->GetBinContent(k) - h_ped->GetBinContent(k));
						cout << str << endl;
						//cout << "DEBUG: sum before monster" << sum << endl;
						sum = sum - monster_num[2];
						//cout <<  "DEBUG: sum after monster " << sum << endl;
					}
					else sum = (h_meas->GetBinContent(k) - h_ped->GetBinContent(k));
					if (sum <= 0)
					{	 
						sum = 0;
					}
					//cout << "DEBUG: sum after monster 2 " << sum << endl;
					result->SetBinContent(k,sum);				
				}
				//cout << total_sum << endl;
				//cout << "Average amount of entries of " << key1->GetTitle() << " = " <<  avg << endl;
				//result->Draw();
				//c1.Write(key1->GetName());
				result->Write(key1->GetName());
				
			}
		}
	}
	ped->Close();
	meas->Close();
	hfile->Close();
	cout << "Saving file to " << output_file << endl;
	cout << "File saved" << endl;
	return 0;
}
