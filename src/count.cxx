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
#include <stdarg.h>
#include <KpixEvent.h>
#include <KpixSample.h>
#include <Data.h>
#include <DataRead.h>
#include <math.h>
#include <fstream>
#include <string.h>

using namespace std;


// Process the data
int main ( int argc, char **argv ) {
  DataRead               dataRead;
  off_t                  fileSize;
  off_t                  filePos;
  KpixEvent              event;
  KpixSample             *sample;
  
  string                 calState;
  bool                   bucketFound[32][1024][4];
  bool                   chanFound[32][1024];
  bool                   kpixFound[32];
  KpixSample::SampleType type;

  uint                   kpix;
  uint                   channel;
  uint                   bucket;
  uint                   eventCount;
  // Data file is the first and only arg
  if ( argc != 2  ) {
    cout << "\nUsage: ./count data_file\n";
    return(1);
  }
  
  // Open data file
  if ( ! dataRead.open(argv[1])  ) {
    cout << "Error opening data file " << argv[1] << endl;
    return(1);
  }
 
  //////////////////////////////////////////
  // Read Data
  //////////////////////////////////////////
  cout << "Opened data file: " << argv[1] << endl;
  fileSize = dataRead.size();
  filePos  = dataRead.pos();
  
  // Init
  eventCount       	= 0;
  cout << "\rReading File: 0 %" << flush;
  
  //  Initialize all histograms
  uint count = 0;
  bool printalot = false;
  while ( dataRead.next(&event) ) {
      if (count < 10 && printalot) {
	cout << "test" << endl;
	cout << "\tEventNumber = "<< event.eventNumber()<<endl;
      }
      count++;
  }
  cout<< "In total, we have #"
      << count
      << " events :)\n"<<endl;
    
  dataRead.close();
  
  /*
  dataRead.open(argv[1]); 
  cout<<"[dev] kpix , channel, bucket \n";
  while ( dataRead.next(&event) ) {
    eventCount++;
    for (int x=0; x < event.count(); x++) 
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
	    cout<<"[dev] "<<kpix<<", "
		<<channel <<", "
		<<bucket <<"\n";
	  }
      }
  }
  dataRead.close();
  */
  return(0);
	
}
