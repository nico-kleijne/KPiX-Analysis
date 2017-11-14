	//-----------------------------------------------------------------------------
	// File          : analysis.cpp
	// Author        : Uwe Krämer (orig. Ryan Herbst) <uwe.kraemer@desy.de>
	// Created       : 06/28/2017
	// Project       : KPiX Analysis
	//-----------------------------------------------------------------------------
	// Description :
	// General analysis of KPiX data.
	//-----------------------------------------------------------------------------
	// Copyright (c) 2012 by SLAC. All rights reserved.
	// Proprietary and confidential to SLAC.
	//-----------------------------------------------------------------------------
	// Modification history :
	// 05/30/2012: created
	// 06/28/2017: large scale rewrite of original calibrationFitter.cpp
	//-----------------------------------------------------------------------------
	#include <iostream>
	#include <iomanip>
	#include <TFile.h>
	#include <TH1F.h>
	#include <TH2F.h>
	#include <TF1.h>
	#include <TTree.h>
	#include <TROOT.h>
	#include "/afs/desy.de/user/k/kraemeru/public/map.cpp"
	#include <TPolyLine3D.h>
	#include <TCanvas.h>
	#include <TMultiGraph.h>
	#include <TApplication.h>
	#include <TGraphErrors.h>
	#include <TGraph.h>
	#include <TStyle.h>
	#include <TVector.h>
	#include <TKey.h>
	#include <TClass.h>
	#include <stdarg.h>
	#include <KpixEvent.h>
	#include <KpixSample.h>
	#include <Data.h>
	#include <DataRead.h>
	#include <math.h>
	#include <fstream>
	#include <XmlVariables.h>
	#include <string.h>
	
	using namespace std;
	
	
	
	
	

	
	// Function to compute calibration charge
	
	
	void addDoubleToXml ( ofstream *xml, uint indent, string variable, Double_t value ) {
	uint x;
	
	if ( !(value != value) ) {
		for (x=0; x < indent; x++) *xml << " ";
		*xml << "<" << variable << ">";
		*xml << value;
		*xml << "</" << variable << ">";
		*xml << endl;
	}
	}
	
	void addStringToXml ( ofstream *xml, uint indent, string variable, string value ) {
	uint x;
	
	for (x=0; x < indent; x++) *xml << " ";
	*xml << "<" << variable << ">";
	*xml << value;
	*xml << "</" << variable << ">";
	*xml << endl;
	}
	
	
	// Process the data
	int main ( int argc, char **argv ) 
	{
		
	TH1::SetDefaultSumw2();
	
	//Produce a TTREE for histogram saving test.
	
	
	
	
	
	
	
	TPolyLine3D *track_plot = new TPolyLine3D(3);
		
		
	DataRead               dataRead;
	off_t                  fileSize;
	off_t                  filePos;
	KpixEvent              event;
	KpixSample             *sample;
	
	string                 calState;
	uint                   lastPct;
	uint                   currPct;
	bool                   bucketFound[32][1024][4];
	bool                   chanFound[32][1024];
	bool                   kpixFound[32];
	uint                   minChan;
	uint                   maxChan;
	uint                   x;
	uint                   range;
	uint                   value;
	uint                   kpix;
	uint                   channel;
	uint                   bucket;
	double                  tstamp;
	string                 serial;
	KpixSample::SampleType type;
	TH1F                   	*hist[32][1024][4][2];
	TH1F					*hist_stamped[32][1024][4][2];
	//TH1F					*hist_charge[32][1024][4][2];
	TH1F					*channel_time[32][1024][4][2];
	TH1F					*channel_entries[32][5];
	TH1F					*channel_entries_no_monster[32][5];
	TH1F					*times_kpix[32][5];
	TH1F					*times_kpix_monster[32][5];
	TH1F					*times_kpix_no_monster[32][5];
	TH1F					*trig_count[32][5];
	TH1F                   	*hist_buck_sum[32][1024];
	
	stringstream           tmp;
	stringstream           tmp_units;
	
	stringstream           tmp2;
	stringstream           tmp2_units;
	
	stringstream           tmp3;
	stringstream           tmp3_units;
	
	stringstream			Folder1;
	stringstream			Folder2;
	stringstream			Folder3;
	
	ofstream               xml;
	ofstream               csv;
	uint                   acquisitionCount;
	string                 outRoot;
	string                 outXml;
	string                 outCsv;
	TFile                  *rFile;
	TCanvas                *c1;
	stringstream           crossString;
	stringstream           crossStringCsv;
	XmlVariables           config;
	bool                   findBadMeanHist;
	bool                   findBadMeanFit;
	bool                   findBadMeanChisq;
	bool                   findBadGainFit;
	bool                   findBadGainChisq;
	double                 meanMin[2];
	double                 meanMax[2];
	double                 meanChisq;
	double                 gainMin[2];
	double                 gainMax[2];
	double                 chargeError[2];
	double                 gainChisq;
	double                 fitMin[2];
	double                 fitMax[2];
	ofstream               debug;
	ofstream				channel_file_bad;
	ofstream				channel_file_bad_fit;
	ofstream				channel_file_noise;
	ofstream				channel_file_calib;
	ofstream 				channel_file_adc_mean;
	uint                   badTimes;
	uint                   goodTimes;
	uint                   badMeanFitCnt;
	uint                   badMeanHistCnt;
	uint                   badMeanChisqCnt;
	uint                   badGainFitCnt;
	uint                   badGainChisqCnt;
	uint                   failedGainFit;
	uint                   failedMeanFit;
	uint                   badChannelCnt;
	uint					noiseSigmaCnt;
	uint                    errorSigmaCnt;
	double 					calib_slope[1024];
	double					calib_y0[1024];
	pixel					pixel_kpix[1024];
	pixel_mapping(pixel_kpix);
	
	
	int 					num_of_channels[32] = {0};
	TH1F*					acq_num_ext[32];
	TH1F* 					AssignedChannelHist[32][1000];
	//TH1F* 					AssignedNumberHist[32][1000];
	TH1F*					event_time[32][1000];
	std::vector<int> monster_channels;
	
	
	double beam_cut = 0.5;
	
	
	
	// Data file is the first and only arg
	//if ( argc != 3 && argc != 4 ) {
		//cout << "Usage: calibrationFitter config_file data_file [debug_file]\n";
		//return(1);
	//}
	
	//if ( argc == 4 ) debug.open(argv[3],ios::out | ios::trunc);
	bool calibration_check = 0;
	if ( argc == 4 ) calibration_check = 1;
	
	// Read configuration
	//if ( ! config.parseFile("config",argv[1]) ) {
		//cout << "Failed to read configuration from " << argv[1] << endl;
		//return(1);
	//}
	//Read root calibration file
	//if ( argc == 4 && ! calibration_file.is_open()) {
		//cout << "Failed to read calibration from " << argv[3] << endl;
		//return(1);
	//}
	
	// Extract configuration values
	findBadMeanHist  = config.getInt("FindBadMeanHist");
	findBadMeanFit   = config.getInt("FindBadMeanFit");
	meanMin[0]       = config.getDouble("GoodMeanMinR0");
	meanMax[0]       = config.getDouble("GoodMeanMaxR0");
	meanMin[1]       = config.getDouble("GoodMeanMinR1");
	meanMax[1]       = config.getDouble("GoodMeanMaxR1");
	findBadMeanChisq = config.getInt("FindBadMeanChisq");
	meanChisq        = config.getInt("GoodMeanChisqMax");
	findBadGainFit   = config.getInt("FindBadGainFit");
	gainMin[0]       = config.getDouble("GoodGainMinR0");
	gainMax[0]       = config.getDouble("GoodGainMaxR0");
	gainMin[1]       = config.getDouble("GoodGainMinR1");
	gainMax[1]       = config.getDouble("GoodGainMaxR1");
	findBadGainChisq = config.getInt("FindBadGainChisq");
	gainChisq        = config.getInt("GoodGainChisqMax");
	fitMin[0]        = config.getDouble("GainFitMinR0");
	fitMax[0]        = config.getDouble("GainFitMaxR0");
	fitMin[1]        = config.getDouble("GainFitMinR1");
	fitMax[1]        = config.getDouble("GainFitMaxR1");
	chargeError[0]   = config.getDouble("GainChargeErrorR0");
	chargeError[1]   = config.getDouble("GainChargeErrorR1");
	
	// Open data file
	if ( ! dataRead.open(argv[1])  ) {
		cout << "Error opening data file " << argv[1] << endl;
		return(1);
	}
	
	// Create output names
	tmp.str("");
	tmp << argv[1] << ".root";
	outRoot = tmp.str();
	tmp.str("");
	tmp << argv[1] << ".xml";
	outXml = tmp.str();
	tmp.str("");
	tmp << argv[1] << ".csv";
	outCsv = tmp.str();
	
	
	//Start of change
	//if (argc == 4) 
	//{
		//TFile *calibration_file = TFile::Open(argv[3]);		
		//TIter cal_iter(calibration_file->GetListOfKeys());
		//TKey *cal_key;
		//int count = 0;
		//while ((cal_key = (TKey*)cal_iter())) 
		//{
			////cout << "Current key1 = " << cal_key->GetClassName() << endl;
			//string title = cal_key->GetTitle();
			//if ((strcmp(cal_key->GetClassName(), "TGraphErrors") == 0) && (title.find("calib") != std::string::npos ))
			//{
				////cout << "test" << endl;
				//TGraphErrors *calib_graph = (TGraphErrors*)cal_key->ReadObj();
				//calib_graph->Fit("pol1","eq","",fitMin[range],fitMax[range]);
				//calib_slope[count] = calib_graph->GetFunction("pol1")->GetParameter(1);
				//calib_y0[count] = calib_graph->GetFunction("pol1")->GetParameter(0);
				////cout << "Title: " << cal_key->GetTitle() << " with Slope = " << calib_slope[count] << endl;
				//++count;
			//}
		//}
	//}
	//end of change
	
	//////////////////////////////////////////
	// Read Data
	//////////////////////////////////////////
	cout << "Opened data file: " << argv[1] << endl;
	fileSize = dataRead.size();
	filePos  = dataRead.pos();
	
	// Init
	currPct          	= 0;
	lastPct          	= 100;
	eventCount       	= 0;
	minChan          	= 0;
	maxChan          	= 0;
	badTimes         	= 0;
	badMeanFitCnt    	= 0;
	badMeanHistCnt   	= 0;
	badMeanChisqCnt  	= 0;
	badGainFitCnt    	= 0;
	badGainChisqCnt 	= 0;
	failedGainFit   	= 0;
	failedMeanFit   	= 0;
	badChannelCnt   	= 0;
	noiseSigmaCnt		= 0;
	errorSigmaCnt		= 0;
	cout << "\rReading File: 0 %" << flush;
	goodTimes       	= 0;
	
	
	// Default canvas
	c1 = new TCanvas("c1","c1");
	
	// Open root file
	rFile = new TFile(outRoot.c_str(),"recreate");
	
	TH1F *channel_entries_total= new TH1F("Channel entries_total", "Channel Entries_total; KPiX_channel_address; # entries", 1024, -0.5, 1023.5);
	TH1F *channel_entries_total_stamped= new TH1F("Channel entries_total_stamped", "Channel_entries_total_stamped; KPiX_channel_address; # entries", 1024, -0.5, 1023.5);
	
	TH1F *channel_entries_total_stamped_400= new TH1F("Channel_entries_total_stamped400", "Channel_entries_total_stamped400; KPiX_channel_address; # entries", 1024, -0.5, 1023.5);
	TH1F *channel_entries_total_stamped_200= new TH1F("Channel_entries_total_stamped200", "Channel_entries_total_stamped200; KPiX_channel_address; # entries", 1024, -0.5, 1023.5);
	TH1F *channel_entries_total_stamped_100= new TH1F("Channel_entries_total_stamped100", "Channel_entries_total_stamped100; KPiX_channel_address; # entries", 1024, -0.5, 1023.5);
	TH1F *channel_entries_total_stamped_50= new TH1F("Channel_entries_total_stamped50", "Channel_entries_total_stamped50; KPiX_channel_address; # entries", 1024, -0.5, 1023.5);
	TH1F *channel_entries_total_stamped_10= new TH1F("Channel_entries_total_stamped10", "Channel_entries_total_stamped10; KPiX_channel_address; # entries", 1024, -0.5, 1023.5);
	
	TH1F *time_kpix= new TH1F("time kpix", "time kpix; Time/bunchClkCount; #entries", 300, -0.5, 8191.5);
	TH1F *time_kpix_selected= new TH1F("time kpix_selected", "time kpix_selected; Time/bunchClkCount; #entries", 300, -0.5, 8191.5);
	TH1F *time_external= new TH1F("time external", "time external; Time/bunchClkCount; #entries/#acq.cycles", 300, -0.5, 8192.5); // one higher because the accuracy is higher
	
	TH1F *time_kpix_k26_selected= new TH1F("time_kpix_k26_selected", "time_kpix_k26_selected; Time/bunchClkCount; #entries", 300, -0.5, 8191.5);
	TH1F *time_kpix_k28_selected= new TH1F("time_kpix_k28_selected", "time_kpix_k28_selected; Time/bunchClkCount; #entries", 300, -0.5, 8191.5);
	TH1F *time_kpix_k30_selected= new TH1F("time_kpix_k30_selected", "time_kpix_k30_selected; Time/bunchClkCount; #entries", 300, -0.5, 8191.5);
	
	TH1F *total= new TH1F("Total_response", "total_response; Charge/ADC; #entries", 300, -0.5, 8191.5);
	TH1F *total_stamped= new TH1F("Total_response_stamped", "total_response_stamped; Charge/ADC; #entries", 300, -0.5, 8191.5);
	
	TH1F *total_selected= new TH1F("Total_response_selected", "total_response_selected; Charge/ADC; #entries", 300, -0.5, 8191.5);
	TH1F *total_stamped_selected= new TH1F("Total_response_selected_stamped", "total_response_selected_stamped; Charge/ADC; #entries", 300, -0.5, 8191.5);
	
	TH1F *beam_ext_time_diff = new TH1F("beam_ext_time_diff", "beam_ext_time_diff; #Delta T/bunchClkCount; # entries", 17000, -8192, 8191.5);
	TH1F *beam_ext_time_diff_selected = new TH1F("beam_ext_time_diff_selected", "beam_ext_time_diff_selected; #Delta T/bunchClkCount; # entries", 17000, -8192, 8191.5);
	TH1F *beam_ext_time_diff_865 = new TH1F("beam_ext_time_diff_865", "beam_ext_time_diff_865; #Delta T/bunchClkCount; # entries", 17000, -8192, 8191.5);
	TH1F *beam_ext_time_diff_801 = new TH1F("beam_ext_time_diff_801", "beam_ext_time_diff_801; #Delta T/bunchClkCount; # entries", 17000, -8192, 8191.5);
	
	TH2F *k28_k30_x_correlation = new TH2F ("x_correlation_k28_k30", "x_correlation_k28_k30; x28/column_width; x30/column_width ", 38,0.5,19,38,0.5,19); 
	TH2F *k28_k30_y_correlation = new TH2F ("y_correlation_k28_k30", "y_correlation_k28_k30; x28/row_width; x30/row_width", 40,-18,13,40,-18,13); 
	TH2F *k30_map = new TH2F ("k30_map", "k30_map; x30/column_width; x30/row_width",38,0.5,19,40,-18,13);
	
	TH2F *k26_k30_x_correlation = new TH2F ("x_correlation_k26_k30", "x_correlation_k26_k30; x26/column_width; x30/column_width ", 38,0.5,19,38,0.5,19); 
	TH2F *k26_k30_y_correlation = new TH2F ("y_correlation_k26_k30", "y_correlation_k26_k30; x26/row_width; x30/row_width", 40,-18,13,40,-18,13); 
	TH2F *k26_map = new TH2F ("k26_map", "k26_map; x26/column_width; x26/row_width",38,0.5,19,40,-18,13);
	
	TH2F *k26_k28_x_correlation = new TH2F ("x_correlation_k26_k28", "x_correlation_k28_k30; x26/column_width; x28/column_width ", 38,0.5,19,38,0.5,19); 
	TH2F *k26_k28_y_correlation = new TH2F ("y_correlation_k26_k28", "y_correlation_k26_k28; x26/row_width; x28/row_width", 40,-18,13,40,-18,13); 
	TH2F *k28_map = new TH2F ("k28_map", "k28_map; x28/column_width; x28/row_width",38,0.5,19,40,-18,13);
	
	TH1F *k26_x = new TH1F ("k26_x", "k26_x; x26/column_width; #entries ",38,0.5,19);
	TH1F *k28_x = new TH1F ("k28_x", "k28_x; x28/column_width; #entries ",38,0.5,19);
	TH1F *k30_x = new TH1F ("k30_x", "k30_x; x30/column_width; #entries ",38,0.5,19);
	
	TH1F *k26_y = new TH1F ("k26_y", "k26_y; y26/row_width; #entries ",40,-18,13);
	TH1F *k28_y = new TH1F ("k28_y", "k28_y; y28/row_width; #entries ",40,-18,13);
	TH1F *k30_y = new TH1F ("k30_y", "k30_y; y30/row_width; #entries ",40,-18,13);
	
	TH1F *ExtTrigPerCycle = new TH1F ("external_triggers_per_cycle", "ext_trig_per_acq.; #ext_triggers_per_acq.cycle; #entries ",50,0.5,99.5);
	
	while ( dataRead.next(&event) ) // event read to check for filled channels and kpix to reduce number of histograms.
	{
		acquisitionCount++;
		if ( acquisitionCount < 10)
		{
			cout << "DEBUG: EVENTNUMBER " << event.eventNumber() << endl;
		}
		for (x=0; x < event.count(); x++) 
		{
	
			//// Get sample
			sample  = event.sample(x);
			kpix    = sample->getKpixAddress();
			channel = sample->getKpixChannel();
			bucket  = sample->getKpixBucket();
			if ( type == KpixSample::Data ) 
			{
				kpixFound[kpix]          = true;
				chanFound[kpix][channel] = true;
				bucketFound[kpix][channel][bucket] = true;
			}
			//cout << "KPIX: " << kpix << endl;
			//cout << "Channel: " << channel << endl;
			kpixFound[0] = false; // for some reason the system finds a kpix in slot 0
		}
	}
	dataRead.close();
	range = 0;
	for (int u = 0; u<32; ++u)
	{
		if (kpixFound[u])
		{
			for (int j =0; j<1024; ++j)
			{
				if (chanFound[u][j] == 0)
				{
					cout << "DEBBUG  KPIX: " << u << " Channel: " << j << endl;
				}
			}
		}
	}
	
	double weight = 1.0/acquisitionCount;
	int monster_counter_k30 = 0;
	int monster_counter_k26 = 0;
	int monster_counter_k28 = 0;
	
	
	for (kpix = 0; kpix < 32; kpix++) // initialization of histograms
	{
		//
		//cout << "DEBUG test " << kpixFound[kpix] << endl;
		if (kpixFound[kpix])
		{
			rFile->cd();
			Folder1.str("");
			Folder1 << "KPiX_" << kpix;
			rFile->mkdir(Folder1.str().c_str());
			TDirectory *kpix_folder = rFile->GetDirectory(Folder1.str().c_str());
			kpix_folder->cd();
			tmp.str("");
			tmp << "Channel_entries_k_" << kpix << "_total";
			channel_entries[kpix][4] = new TH1F(tmp.str().c_str(), "Channel_Entries; KPiX_channel_address; #entries/#acq.cycles", 1024,-0.5, 1023.5);
			tmp.str("");
			tmp << "Channel_entries_k_" << kpix << "_total_no_monster";
			channel_entries_no_monster[kpix][4] = new TH1F(tmp.str().c_str(), "Channel_Entries; KPiX_channel_address; #entries/#acq.cycles", 1024,-0.5, 1023.5);
			tmp.str("");
			tmp << "timestamp_kpix_k_" << kpix  << "_total";
			times_kpix[kpix][4] = new TH1F(tmp.str().c_str(), "timestamp_kpix; time/#bunch_clk_count; #entries/#acq.cycles", 300,-0.5, 8191.5);
			tmp.str("");
			tmp << "timestamp_kpix_k_" << kpix  << "_monster_total";
			times_kpix_monster[kpix][4] = new TH1F(tmp.str().c_str(), "timestamp_kpix; time/#bunch_clk_count; #entries/#acq.cycles", 300,-0.5, 8191.5);
			tmp.str("");
			tmp << "timestamp_kpix_k_" << kpix  << "_no_monster_total";
			times_kpix_no_monster[kpix][4] = new TH1F(tmp.str().c_str(), "timestamp_kpix; time/#bunch_clk_count; #entries/#acq.cycles", 300,-0.5, 8191.5);
			tmp.str("");
			tmp << "trig_count_k" << kpix << "_total";
			trig_count[kpix][4]  = new TH1F (tmp.str().c_str(), "trig_count;  #triggered channels; #entries ",1024, -0.5, 1023.5);
			
			
			
			tmp.str("");
			tmp << "acq_num_ext_k_" << kpix;
			acq_num_ext[kpix] = new TH1F(tmp.str().c_str(), "acq_num_ext; #triggers/acq._cycle; #entries ",5, -0.5, 4.5);
			
			for (bucket = 0; bucket< 4; bucket++)
			{
				tmp.str("");
				tmp << "Channel_entries_k_" << kpix << "_b" << bucket;
				channel_entries[kpix][bucket] = new TH1F(tmp.str().c_str(), "Channel Entries; KPiX_channel_address; #entries/#acq.cycles", 1024,-0.5, 1023.5);
				tmp.str("");
				tmp << "Channel_entries_k_" << kpix << "_b" << bucket << "_no_monster";
				channel_entries_no_monster[kpix][bucket] = new TH1F(tmp.str().c_str(), "Channel Entries; KPiX_channel_address; #entries/#acq.cycles", 1024,-0.5, 1023.5);
				tmp.str("");
				tmp << "timestamp_kpix_k_" << kpix << "_b" << bucket;
				times_kpix[kpix][bucket] = new TH1F(tmp.str().c_str(), "timestamp_kpix; time/#bunch_clk_count; #entries/#acq.cycles", 300,-0.5, 8191.5);
				tmp.str("");
				tmp << "timestamp_kpix_monster_k_" << kpix << "_b" << bucket;
				times_kpix_monster[kpix][bucket] = new TH1F(tmp.str().c_str(), "timestamp_kpix; time/#bunch_clk_count; #entries/#acq.cycles", 300,-0.5, 8191.5);
				tmp.str("");
				tmp << "trig_count_k" << kpix << "_b" << bucket ;
				trig_count[kpix][bucket]  = new TH1F(tmp.str().c_str(), "trig_count;  #triggered channels; #entries ",1024, -0.5,1023.5);
				tmp.str("");
				tmp << "timestamp_kpix_k_" << kpix << "_b" << bucket << "_no_monster";
				times_kpix_no_monster[kpix][bucket] = new TH1F(tmp.str().c_str(), "timestamp_kpix; time/#bunch_clk_count; #entries/#acq.cycles", 300,-0.5, 8191.5);
			}
			Folder1.str("");
			Folder1 << "Events";
			kpix_folder->mkdir(Folder1.str().c_str());
			TDirectory *event_folder = kpix_folder->GetDirectory(Folder1.str().c_str());
			rFile->cd(event_folder->GetPath());
			for (int events = 0; events < 1000; events++)
			{
				tmp.str("");
				tmp << "time_distribution_k_" << kpix << "_evt_" << events;
				//cout << events << endl;
				event_time[kpix][events] = new TH1F(tmp.str().c_str(), "time_distribution; time/BunchClkCount; #entries", 100, -0.5, 8191.5);
				tmp.str("");
				tmp << "assigned_channels_k" << kpix << "_evt_" << events;
				AssignedChannelHist[kpix][events]  = new TH1F (tmp.str().c_str(), "assigned_channels_per_ext_trig;  external_trigger_number; #assigned_channels ",100, -0.5, 99.5);
				//tmp.str("");
				//tmp << "assigned_number_k" << kpix << "_evt_" << events;
				//AssignedNumberHist[kpix][events]  = new TH1F (tmp.str().c_str(), "assigned_NumberOfChannel_per_ext_trig;  #same_assignement; #entries ",40,0,40);
			}
			Folder1.str("");
			Folder1 << "Channels";
			kpix_folder->mkdir(Folder1.str().c_str());
			TDirectory *channels_folder = kpix_folder->GetDirectory(Folder1.str().c_str());
			rFile->cd(channels_folder->GetPath());	
			for (channel = 0; channel < 1024; channel++)
			{
				if (chanFound[kpix][channel])
				{
					Folder1.str("");
					Folder1 << "Channel_" << channel;
					channels_folder->mkdir(Folder1.str().c_str());
					TDirectory *channel_folder = channels_folder->GetDirectory(Folder1.str().c_str());
					rFile->cd(channel_folder->GetPath());
					
					tmp.str("");
					tmp << "hist" << "_c" << dec << setw(4) << setfill('0') << channel;
					tmp << "_k" << dec << kpix;
					
					tmp_units.str("");
					tmp_units << "hist" << "_c" << dec << setw(4) << setfill('0') << channel;
					tmp_units << "_k" << dec << kpix;
					tmp_units << "; Charge/ADC; #entries/#acq.cycles";
					
					hist_buck_sum[kpix][channel] = new TH1F(tmp.str().c_str(),tmp_units.str().c_str(),8192, -0.5, 8191.5);
					
					num_of_channels[kpix] = num_of_channels[kpix] + 1;
					
					for (bucket = 0; bucket < 4; bucket++)
					{
						if (bucketFound[kpix][channel][bucket])
						{
							// Naming of histograms and generating of histograms
							
							
							
							
							tmp.str("");
							tmp << "hist" << "_c" << dec << setw(4) << setfill('0') << channel;
							tmp << "_b" << dec << bucket;
							tmp << "_k" << dec << kpix;
							
							tmp_units.str("");
							tmp_units << "hist" << "_c" << dec << setw(4) << setfill('0') << channel;
							tmp_units << "_b" << dec << bucket;
							tmp_units << "_k" << dec << kpix;
							tmp_units << "; Charge/ADC; #entries/#acq.cycles";
						
							hist[kpix][channel][bucket][0] = new TH1F(tmp.str().c_str(),tmp_units.str().c_str(),8192, -0.5,8191.5);
							
							tmp.str("");
							tmp << "hist_stamped" << "_c" << dec << setw(4) << setfill('0') << channel;
							tmp << "_b" << dec << bucket;
							tmp << "_k" << dec << kpix;
							tmp << "_time_cut";
								
							tmp_units.str("");
							tmp_units << "hist_stamped" << "_c" << dec << setw(4) << setfill('0') << channel;
							tmp_units << "_b" << dec << bucket;
							tmp_units << "_k" << dec << kpix;
							tmp_units << "; Charge/ADC; #entries/#acq.cycles";
						
							hist_stamped[kpix][channel][bucket][0] = new TH1F(tmp.str().c_str(),tmp_units.str().c_str(),8192, -0.5,8191.5);
							
							tmp.str("");
							tmp << "time" << "_c" << dec << setw(4) << setfill('0') << channel;
							tmp << "_b" << dec << bucket;
							tmp << "_r" << dec << range;
							tmp << "_k" << dec << kpix;
							
							tmp_units.str("");
							tmp_units << "time" << "_c" << dec << setw(4) << setfill('0') << channel;
							tmp_units << "_b" << dec << bucket;
							tmp_units << "_k" << dec << kpix;
							tmp_units << "; Time/bunchClkCount; #entries/#acq.cycles";
							
							channel_time[kpix][channel][bucket][0] = new TH1F(tmp.str().c_str(),tmp_units.str().c_str(),300, -0.5,8191.5);
							
							
							tmp.str("");
							tmp << "charge" << "_c" << dec << setw(4) << setfill('0') << channel;
							tmp << "_b" << dec << bucket;
							tmp << "_r" << dec << range;
							tmp << "_k" << dec << kpix;
							
							tmp_units.str("");
							tmp_units << "charge" << "_c" << dec << setw(4) << setfill('0') << channel;
							tmp_units << "_b" << dec << bucket;
							tmp_units << "_k" << dec << kpix;
							tmp_units << "; Charge/fC; #entries/#acq.cycles";
							
							//hist_charge[kpix][channel][bucket][0] = new TH1F(tmp.str().c_str(),tmp_units.str().c_str(),300, -0.5,8191.5);
							
							
						}
					}
				}
			}
		}
	}
	
	//// Process each event
	//// beam spot selection
	dataRead.open(argv[1]);
	int event_num = 0;
	while ( dataRead.next(&event) )
	{

		double 	trigger_counter[32] = {0};
		
		for (x=0; x < event.count(); x++) 
		{
			//// Get sample
			sample  = event.sample(x);
			kpix    = sample->getKpixAddress();
			channel = sample->getKpixChannel();
			bucket  = sample->getKpixBucket();
			value   = sample->getSampleValue();
			type    = sample->getSampleType();
			tstamp  = sample->getSampleTime();
			range   = sample->getSampleRange();
			if ( type == KpixSample::Data ) // If event is of type KPiX data
			{
				channel_entries[kpix][bucket]->Fill(channel, weight);
				channel_entries[kpix][4]->Fill(channel, weight);
				times_kpix[kpix][bucket]->Fill(tstamp, weight);
				times_kpix[kpix][4]->Fill(tstamp, weight);
				trigger_counter[kpix] = trigger_counter[kpix] + (1.0/num_of_channels[kpix]);
				if (event.count() > 800)
				{
					times_kpix_monster[kpix][bucket]->Fill(tstamp, weight);
					times_kpix_monster[kpix][4]->Fill(tstamp, weight);
				}
				else
				{
					channel_entries_no_monster[kpix][bucket]->Fill(channel, weight);
					channel_entries_no_monster[kpix][4]->Fill(channel, weight);
					times_kpix_no_monster[kpix][bucket]->Fill(tstamp, weight);
					times_kpix_no_monster[kpix][4]->Fill(tstamp, weight);
				}
				if (event_num < 1000) event_time[kpix][event_num]->Fill(tstamp);
			}
		}
		event_num++;
		if (trigger_counter[26] > 4) cout << trigger_counter[26] << endl;
		if (kpixFound[26]) acq_num_ext[26]->Fill(trigger_counter[26]);
		if (kpixFound[28]) acq_num_ext[28]->Fill(trigger_counter[28]);
		if (kpixFound[30]) acq_num_ext[30]->Fill(trigger_counter[30]);
	}
	dataRead.close();
	dataRead.open(argv[1]);
	
	event_num = 0;
	while ( dataRead.next(&event) ) 
	{
		std::vector<double> time_ext;
		std::vector<int> channel_hits[32];
		std::vector<int> timestamp[32];
		//std::vector<int> Assignment_number;
		int num_trig_count_k26[5] = {0};
		int num_trig_count_k28[5] = {0};
		int num_trig_count_k30[5] = {0};
		for (x=0; x < event.count(); x++) 
		{
			//cout << "DEBUG: EVENT COUNT " << event.count() << endl;
			//// Get sample
			sample  = event.sample(x);
			kpix    = sample->getKpixAddress();
			channel = sample->getKpixChannel();
			bucket  = sample->getKpixBucket();
			value   = sample->getSampleValue();
			type    = sample->getSampleType();
			tstamp  = sample->getSampleTime();
			range   = sample->getSampleRange();
			
			
			
			
			
			
			if (type == 2)// If event is of type external timestamp
			{
				double time = tstamp + double(value * 0.125);
				time_external->Fill(time, weight);
				time_ext.push_back(time);
				//cout << "DEBUG: channel in timestmap = " << channel << endl;
				//cout << "DEBUG: bucket in timestmap = " << bucket << endl;
				
			}

			
			if ( type == KpixSample::Data ) // If event is of type KPiX data
			{

					channel_hits[kpix].push_back(channel);
					timestamp[kpix].push_back(tstamp);
					time_kpix->Fill(tstamp, weight);

					
					
					hist[kpix][channel][bucket][0]->Fill(value, weight);
					//hist_charge[kpix][channel][bucket][0]->Fill(double(value)/calib_slope[channel]*pow(10,15) , weight);
					hist_buck_sum[kpix][channel]->Fill(value,weight);
					channel_entries_total->Fill(channel, weight);
					channel_time[kpix][channel][bucket][0]->Fill(tstamp, weight);
					total->Fill(value, weight);
					
					
					if (kpix == 30)
					{
						k30_map->Fill(pixel_kpix[channel].x, pixel_kpix[channel].y);
						k30_x->Fill(pixel_kpix[channel].x, weight);
						k30_y->Fill(pixel_kpix[channel].y, weight);
						num_trig_count_k30[bucket] += 1;
						num_trig_count_k30[4] += 1;
						if (channel_entries[kpix][0]->GetBinContent(channel) > beam_cut)
						{
							time_kpix_k30_selected->Fill(tstamp, weight);
						}
						if (event.count() > 800) time_kpix_k30_selected->Fill(tstamp, weight);
					}
					if (kpix == 26)
					{
						k26_map->Fill(pixel_kpix[channel].x, pixel_kpix[channel].y);
						k26_x->Fill(pixel_kpix[channel].x, weight);
						k26_y->Fill(pixel_kpix[channel].y, weight);
						num_trig_count_k26[bucket] += 1;
						num_trig_count_k26[4] += 1;
						if (channel_entries[kpix][0]->GetBinContent(channel) > beam_cut)
						{
							time_kpix_k26_selected->Fill(tstamp, weight);
						}
					}
					if (kpix == 28)
					{
						k28_map->Fill(pixel_kpix[channel].x, pixel_kpix[channel].y);
						k28_x->Fill(pixel_kpix[channel].x, weight);
						k28_y->Fill(pixel_kpix[channel].y, weight);
						num_trig_count_k28[bucket] += 1;
						num_trig_count_k28[4] += 1;
						if (channel_entries[kpix][4]->GetBinContent(channel) > beam_cut)
						{
							time_kpix_k28_selected->Fill(tstamp, weight);
						}
					}
					
					//cout << "DEBUG channel number: " << channel << endl;
					//cout << "DEBUG X coord: " << pixel_kpix[499].x << endl;
					//cout << "DEBUG y coord: " << pixel_kpix[499].y << endl;
					
					
					if (channel_entries[kpix][4]->GetBinContent(channel) > beam_cut)
					{
						total_selected->Fill(value, weight);
						time_kpix_selected->Fill(tstamp, weight);
					}
					double trig_diff = 8200.0;
					double trig_diff_selected = 8200.0;
					double trig_diff_865 = 8200.0;
					double trig_diff_801 = 8200.0;
					int assigned_number;
					//cout << time_ext.size() << endl;
					for (unsigned int j = 0; j < time_ext.size(); ++j)
					{
						
						if (channel_entries[kpix][4]->GetBinContent(channel) > beam_cut)
						{
							if (fabs(trig_diff_selected) >= fabs(tstamp-time_ext.at(j)))
							{
								trig_diff_selected = tstamp-time_ext.at(j);
								//cout << "[DEBUG] TSTAMP " << tstamp << endl;
								//cout << "[DEBUG] T_EXT " << time_ext.at(j) << endl;
								//cout << "[DEBUG] DIFF " << trig_diff_selected << endl;
							}
							
						}
						if (channel == 865)
						{
							if (fabs(trig_diff_865) >= fabs(tstamp-time_ext.at(j)))
							{
								trig_diff_865 = tstamp-time_ext.at(j);
								//cout << "[DEBUG] TSTAMP " << tstamp << endl;
								//cout << "[DEBUG] T_EXT " << time_ext.at(j) << endl;
								//cout << "[DEBUG] DIFF " << trig_diff_selected << endl;
							}
						}
						if (channel == 801)
						{
							if (fabs(trig_diff_801) >= fabs(tstamp-time_ext.at(j)))
							{
								trig_diff_801 = tstamp-time_ext.at(j);
								//cout << "[DEBUG] TSTAMP " << tstamp << endl;
								//cout << "[DEBUG] T_EXT " << time_ext.at(j) << endl;
								//cout << "[DEBUG] DIFF " << trig_diff_selected << endl;
							}
						}
						
						if (fabs(trig_diff) >= fabs(tstamp-time_ext.at(j)))
						{	
								trig_diff = tstamp-time_ext.at(j);
								assigned_number = j;
								//cout << "[DEBUG] TSTAMP " << tstamp << endl;
								//cout << "[DEBUG] T_EXT " << time_ext.at(j) << endl;
								//cout << "[DEBUG] DIFF " << trig_diff << endl;
						}
						
					}
					//cout << assigned_number << endl;
					if (event_num < 1000) 
					{
						AssignedChannelHist[kpix][event_num]->Fill(assigned_number);
					}
					//Assignment_number.push_back(assigned_number);
					if((trig_diff >= 0.5 )  && (trig_diff  <= 3.0) ) //1 and 2.625
					{
						
						
						hist_stamped[kpix][channel][bucket][0]->Fill(value, weight);
						channel_entries_total_stamped->Fill(channel, weight);
						total_stamped->Fill(value, weight);
						if (channel_entries[kpix][4]->GetBinContent(channel) > beam_cut)
						{
							total_stamped_selected->Fill(value, weight);
							
						}
					}
					if((trig_diff >= -200 )  && (trig_diff  <= 200) ) 
					{
						channel_entries_total_stamped_200->Fill(channel, weight);
					}
					if((trig_diff >= -400 )  && (trig_diff  <= 400) )
					{
						channel_entries_total_stamped_400->Fill(channel, weight);
					}
					if((trig_diff >= -100 )  && (trig_diff  <= 100) ) 
					{
						channel_entries_total_stamped_100->Fill(channel, weight);
					}
					if((trig_diff >= -50 )  && (trig_diff  <= 50) ) 
					{
						channel_entries_total_stamped_50->Fill(channel, weight);
					}
					if((trig_diff >= -10 )  && (trig_diff  <= 10) )
					{
						channel_entries_total_stamped_10->Fill(channel, weight);
					}
					
					
					if (channel == 832 || channel == 801 || channel == 771 || channel == 772 || channel == 773 || channel == 774 || channel == 805 || channel == 837 || channel == 867 || channel == 866 || channel == 897 || channel == 928 || channel == 911 || channel == 879 || channel == 878 || channel == 846 || channel == 847 ||  channel == 833 || channel == 834 || channel == 835 || channel == 802 || channel == 803 || channel == 835 || channel == 865 || channel == 864)
					{
						beam_ext_time_diff_selected->Fill(trig_diff_selected, weight);
					}
					if (channel == 865)
					{
						beam_ext_time_diff_865->Fill(trig_diff_865, weight);
					}
					if (channel == 801)
					{
						beam_ext_time_diff_801->Fill(trig_diff_801, weight);
					}
					beam_ext_time_diff->Fill(trig_diff, weight);
				//}
				
			}
		}
		//for (unsigned int k = 0; k < Assignment_number.size(); k++)
		//{
			//cout << "DEBUG: Assignment_Number " << k << " = " << Assignment_number.at(k) << endl;
			//if (event_num < 1000) AssignedNumberHist[kpix][event_num]->Fill(Assignment_number.at(k));
		//}
		
		ExtTrigPerCycle->Fill(time_ext.size());
		for (int q = 0; q<4; ++q)
		{
			if (num_trig_count_k30[q] > 900) 
			{
				monster_counter_k30++;
			}
			if (num_trig_count_k28[q] > 900)
			{
				monster_counter_k28++;	
			}
			if (num_trig_count_k26[q] > 900)
			{
				monster_counter_k26++;
			}
		}
		for (int q = 0; q<4; ++q)
		{
			if (kpixFound[26]) 
			{
				trig_count[26][4]->Fill(num_trig_count_k26[q], weight);
				trig_count[26][q]->Fill(num_trig_count_k26[q], weight);
			}
			if (kpixFound[28]) 
			{
				trig_count[28][4]->Fill(num_trig_count_k28[q], weight);
				trig_count[28][q]->Fill(num_trig_count_k28[q], weight);
			}
			if (kpixFound[230]) 
			{
				trig_count[30][q]->Fill(num_trig_count_k30[q], weight);
				trig_count[30][4]->Fill(num_trig_count_k30[q], weight);
			}
			
			
		}
		// ====== COMMENT WHEN ANALYSING EXTERNAL DATA FILES ==============//
		//for (int q = 0; q < timestamp[28].size(); q++)
		//{
			//for (int l = 0; l < timestamp[30].size(); l++)
			//{
				//if (timestamp[28].at(q) == timestamp[30].at(l))
				//{
					//k28_k30_x_correlation->Fill(pixel_kpix[channel_hits[28].at(q)].x, pixel_kpix[channel_hits[30].at(l)].x);
					//k28_k30_y_correlation->Fill(pixel_kpix[channel_hits[28].at(q)].y, pixel_kpix[channel_hits[30].at(l)].y);
				//}
			//}
		//}
		//for (int q = 0; q < timestamp[26].size(); q++)
		//{
			//for (int l = 0; l < timestamp[30].size(); l++)
			//{
				//if (timestamp[26].at(q) == timestamp[30].at(l))
				//{
					//k26_k30_x_correlation->Fill(pixel_kpix[channel_hits[26].at(q)].x, pixel_kpix[channel_hits[30].at(l)].x);
					//k26_k30_y_correlation->Fill(pixel_kpix[channel_hits[26].at(q)].y, pixel_kpix[channel_hits[30].at(l)].y);
				//}
			//}
		//}
		//for (int q = 0; q < timestamp[26].size(); q++)
		//{
			//for (int l = 0; l < timestamp[28].size(); l++)
			//{
				//if (timestamp[26].at(q) == timestamp[28].at(l))
				//{
					//k26_k28_x_correlation->Fill(pixel_kpix[channel_hits[26].at(q)].x, pixel_kpix[channel_hits[28].at(l)].x);
					//k26_k28_y_correlation->Fill(pixel_kpix[channel_hits[26].at(q)].y, pixel_kpix[channel_hits[28].at(l)].y);
					////for (int k = 0; k < timestamp[30].size(); k++)
					////{
						////if (timestamp[28].at(l) == timestamp[30].at(k))
						////{
							////track_plot->SetPoint(0, pixel_kpix[channel_hits[26].at(q)].x, pixel_kpix[channel_hits[26].at(q)].y, 0);
							////track_plot->SetPoint(1, pixel_kpix[channel_hits[28].at(l)].x, pixel_kpix[channel_hits[28].at(l)].y, 1);
							////track_plot->SetPoint(2, pixel_kpix[channel_hits[30].at(k)].x, pixel_kpix[channel_hits[30].at(k)].y, 2);
						////}
					////}
				//}
			//}
		//}
		
		
	  ////   Show progress
		event_num++;
		filePos  = dataRead.pos();
		currPct = (uint)(((double)filePos / (double)fileSize) * 100.0);
		if ( currPct != lastPct ) 
		{
			cout << "\rReading File: " << currPct << " %      " << flush;
			lastPct = currPct;
		}
	}
	////// Writing of histograms
	//double first = (hist_calib->FindFirstBinAbove(0))* (2000.0/8192) -1;
	//double last = hist_calib->FindLastBinAbove(0)* (2000.0/8192) +1;
							
	//hist_calib->GetXaxis()->SetRangeUser(first, last);
	
	
	
	////TH2.SetOption("COLZ");
	//k28_k30_x_correlation->SetOption("COLZ");
	//k28_k30_y_correlation->SetOption("COLZ");
	//k30_map->SetOption("COLZ");
	//k26_k30_x_correlation->SetOption("COLZ");
	//k26_k30_y_correlation->SetOption("COLZ");
	//k26_map->SetOption("COLZ");
	//k26_k28_x_correlation->SetOption("COLZ");
	//k26_k28_y_correlation->SetOption("COLZ");
	//k28_map->SetOption("COLZ");
	
	
	
	
	cout << endl << "Number of monster events in k26, k28, k30 = " << monster_counter_k26 << ", " << monster_counter_k28 << ", " << monster_counter_k30 << endl;
	cout << "Number of normed monster events in k26, k28, k30 = " << monster_counter_k26*weight << ", " << monster_counter_k28*weight << ", " << monster_counter_k30*weight << endl;
	
	//cout << "Channel entries in background = " << channel_entries[30]->GetEntries()*weight/(1024) << endl;
    //for (kpix = 0; kpix < 32; kpix++)
    //{
		////if (kpixFound[kpix])
		////{
			////rFile->cd();
			////Folder1.str("");
			////Folder1 << "KPiX_" << kpix;
			////rFile->mkdir(Folder1.str().c_str());
			////TDirectory *kpix_folder = rFile->GetDirectory(Folder1.str().c_str());
			
			////channel_entries[kpix]->Write();
			////times_kpix[kpix]->Write();
			////cout << endl << kpix << endl;
			
			////for (channel = 0; channel < 1024; channel++)
			////{
				////if ((chanFound[kpix][channel]))
				////{
					////if (channel_entries[kpix]->GetBinContent(channel) > beam_cut)
					////{
						////Folder1.str("");
						////Folder1 << "Beam_Spot";
						
						////if (kpix_folder->GetDirectory(Folder1.str().c_str()) == 0)
						////{
							////kpix_folder->mkdir(Folder1.str().c_str());
						////}
						////TDirectory *beam_spot_folder = kpix_folder->GetDirectory(Folder1.str().c_str());
						////Folder1.str("");
						////Folder1 << "Channel_" << channel;
						////beam_spot_folder->mkdir(Folder1.str().c_str());
						////TDirectory *channel_folder = beam_spot_folder->GetDirectory(Folder1.str().c_str());
						////rFile->Cd(channel_folder->GetPath());
						////hist_buck_sum[kpix][channel]->Write();
						////for (bucket = 0; bucket < 4; bucket++)
						////{
							////if (bucketFound[kpix][channel][bucket])
							////{	
								//////Folder1.str("");
								//////Folder1 << "Bucket_" << bucket;
								//////channel_folder->mkdir(Folder1.str().c_str());
								//////TDirectory *bucket_folder = channel_folder->GetDirectory(Folder1.str().c_str());
								//////rFile->Cd(bucket_folder->GetPath());
								////double fitrange_lower = 200.;
								////double fitrange_upper = 600.;
								////int firstbin = hist[kpix][channel][bucket][0]->FindFirstBinAbove(0);
								////int lastbin = hist[kpix][channel][bucket][0]->FindLastBinAbove(0);
								////hist[kpix][channel][bucket][0]->GetXaxis()->SetRangeUser(firstbin, lastbin);
								//////TF1 *lg = new TF1("langaus", langaufun, fitrange_lower,fitrange_upper,4);
								//////lg->SetParameters(20., 270., lastbin,hist[kpix][channel][bucket][0]->GetEntries(),30.);
								//////cout << "================" << endl;
								//////cout << "Fitting Langaus to kpix " << kpix << " channel " << channel << " bucket " << bucket << endl;
								//////hist[kpix][channel][bucket][0]->Fit("langaus","ILRQ","",firstbin,lastbin);
								//////hist[kpix][channel][bucket][0]->Fit("landau","ILRQ","",firstbin,lastbin);
								
								////hist_stamped[kpix][channel][bucket][0]->GetXaxis()->SetRangeUser(hist_stamped[kpix][channel][bucket][0]->FindFirstBinAbove(0), hist_stamped[kpix][channel][bucket][0]->FindLastBinAbove(0));
								//////hist[kpix][channel][bucket][0]->Fit("landau","ILRQ","",firstbin,lastbin);
								////hist_stamped[kpix][channel][bucket][0]->Write();
								//////hist_charge[kpix][channel][bucket][0]->Write();
								////channel_time[kpix][channel][bucket][0]->Write();
								
								//////===================//
								////// UNCOMMENT IF FIT TO BUCKET 0 IS WANTED. MAKES THE PROGRAM MUCH SLOWER //
								//////====================//
								
								//////if (bucket == 0)
								//////{
									//////TF1 *lg = new TF1("langaus", langaufun, fitrange_lower,fitrange_upper,4);
									//////lg->SetParameters(20., 270.,hist[kpix][channel][bucket][0]->GetEntries(),30.);
									//////cout << "================" << endl;
									//////cout << "Fitting Langaus to kpix " << kpix << " channel " << channel << " bucket " << bucket << endl;
									//////hist[kpix][channel][bucket][0]->Fit("langaus","ILRQ","",fitrange_lower,fitrange_upper);
									////////hist[kpix][channel][bucket][0]->Fit("landau","ILQ");
								//////}
								////hist[kpix][channel][bucket][0]->Write();
								
							////}
						////}
					////}
					////else if (channel_entries_total->GetBinContent(channel) <= beam_cut)
					////{
						
						////Folder1.str("");
						////Folder1 << "Background";
						
						////if (kpix_folder->GetDirectory(Folder1.str().c_str()) == 0)
						////{
							////kpix_folder->mkdir(Folder1.str().c_str());
						////}
						////TDirectory *beam_spot_folder = kpix_folder->GetDirectory(Folder1.str().c_str());
						////Folder1.str("");
						////Folder1 << "Channel_" << channel;
						////beam_spot_folder->mkdir(Folder1.str().c_str());
						////TDirectory *channel_folder = beam_spot_folder->GetDirectory(Folder1.str().c_str());
						////rFile->Cd(channel_folder->GetPath());
						////hist_buck_sum[kpix][channel]->Write();
						////for (bucket = 0; bucket < 4; bucket++)
						////{
							////if (bucketFound[kpix][channel][bucket])
							////{	
									//////Folder1.str("");
									//////Folder1 << "Bucket_" << bucket;
									//////channel_folder->mkdir(Folder1.str().c_str());
									//////TDirectory *bucket_folder = channel_folder->GetDirectory(Folder1.str().c_str());
									//////rFile->Cd(bucket_folder->GetPath());
									////hist[kpix][channel][bucket][0]->GetXaxis()->SetRangeUser(hist[kpix][channel][bucket][0]->FindFirstBinAbove(0), hist[kpix][channel][bucket][0]->FindLastBinAbove(0));
									////hist_stamped[kpix][channel][bucket][0]->GetXaxis()->SetRangeUser(hist_stamped[kpix][channel][bucket][0]->FindFirstBinAbove(0), hist_stamped[kpix][channel][bucket][0]->FindLastBinAbove(0));
									//////hist[kpix][channel][bucket][0]->Fit("landau","q");
									////hist[kpix][channel][bucket][0]->Write();
									////hist_stamped[kpix][channel][bucket][0]->Write();
									//////hist_charge[kpix][channel][bucket][0]->Write();
									////channel_time[kpix][channel][bucket][0]->Write();
								
							////}
						////}
					////}
					////cout << "\rSaving File at channel: " << channel << flush;
				////}
			////}
		////}
		
		//if (kpixFound[kpix])
		//{
			////channel_entries[kpix]->Write();
			////times_kpix[kpix]->Write();
			//for (channel = 0; channel < 1024; channel++)
			//{
				//if ((chanFound[kpix][channel]))
				//{
					//for (bucket = 0; bucket < 4; bucket++)
					//{
						//if (bucketFound[kpix][channel][bucket])
						//{	
							//hist[kpix][channel][bucket][0]->GetXaxis()->SetRangeUser(hist[kpix][channel][bucket][0]->FindFirstBinAbove(0), hist[kpix][channel][bucket][0]->FindLastBinAbove(0));
							//hist[kpix][channel][bucket][0]->Write();
							//hist_stamped[kpix][channel][bucket][0]->GetXaxis()->SetRangeUser(hist[kpix][channel][bucket][0]->FindFirstBinAbove(0), hist[kpix][channel][bucket][0]->FindLastBinAbove(0));
							//hist_stamped[kpix][channel][bucket][0]->Write();
							//channel_time[kpix][channel][bucket][0]->Write();
							
						//}
					//}
					//hist_buck_sum[kpix][channel]->Write();
				//}
			//}
		//}
	//}
	//for (int k = 0; k < 1024; k++)
	//{
		//cout << "DEBUG channel number: " << k << endl;
		//cout << "DEBUG X coord: " << pixel_kpix[k].x << endl;
		//cout << "DEBUG y coord: " << pixel_kpix[k].y << endl << endl;
	//}
	
	cout << endl;
	cout << "Writing root plots to " << outRoot << endl;
	cout << "Writing xml data to " << outXml << endl;
	cout << "Writing csv data to " << outCsv << endl;
	cout << endl;
	rFile->Write();
	gROOT->GetListOfFiles()->Remove(rFile);
	
	rFile->Close();
	
	
	//cout << pixel_kpix[872].x << endl;
	//cout << pixel_kpix[872].y << endl;
	//cout << pixel_kpix[810].x << endl;
	//cout << pixel_kpix[810].y << endl;
	
	
	dataRead.close();
	return(0);
}
