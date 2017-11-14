//-----------------------------------------------------------------------------
// File          : cspadGui.cpp
// Author        : Ryan Herbst  <rherbst@slac.stanford.edu>
// Created       : 04/12/2011
// Project       : CSPAD
//-----------------------------------------------------------------------------
// Description :
// Server application for GUI
//-----------------------------------------------------------------------------
// Copyright (c) 2011 by SLAC. All rights reserved.
// Proprietary and confidential to SLAC.
//-----------------------------------------------------------------------------
// Modification history :
// 04/12/2011: created
//----------------------------------------------------------------------------
#include <UdpLink.h>
#include <KpixControl.h>
#include <ControlServer.h>
#include <Device.h>
#include <iomanip>
#include <fstream>
#include <iostream>
#include <signal.h>
using namespace std;

// Run flag for sig catch
bool stop;
bool wmqInkpixGui = false;

// Function to catch cntrl-c
void sigTerm (int) { 
   cout << "Got Signal!" << endl;
   stop = true; 
}

int main (int argc, char **argv) {
   ControlServer cntrlServer;
   string        defFile;
   int           port;
   stringstream  cmd;

   if ( argc > 1 ) defFile = argv[1];
   else defFile = "";

   // Catch signals
   signal (SIGINT,&sigTerm);

   try {
      UdpLink       udpLink; 
      KpixControl   kpix(&udpLink,defFile,32);
      int           pid;

      // Setup top level device
      //if (wmqInkpixGui)
      //kpix.setDebug(true);

      // Create and setup PGP link
      udpLink.setMaxRxTx(500000);
      udpLink.setDebug(true);
      udpLink.open(8192,1,"192.168.1.16");
      udpLink.openDataNet("127.0.0.1",8099);
      udpLink.enableSharedMemory("kpix",1);
      usleep(100);

      // Setup control server
      if (wmqInkpixGui) cntrlServer.setDebug(true);
      cntrlServer.enableSharedMemory("kpix",1);
      port = cntrlServer.startListen(0);
      cntrlServer.setSystem(&kpix);
      cout << "Control id = 1" << endl;

      //kpix.parseXmlString("<system><command><HardReset/></command></system>");
      //kpix.parseXmlString("<system><command><ReadStatus/>\n</command></system>");
      /*kpix.parseXmlFile("xml/cal-desy3_uwe.xml");

      std::cout<<"[eudaq:dev] run count = "<< kpix.getVariable("RunCount")->getInt() <<std::endl;
      int mytest;
      cin >> mytest;
      kpix.getVariable("RunCount")->setInt(mytest);
      std::cout<<"[eudaq:dev] run count = "<< kpix.getVariable("RunCount")->getInt() <<std::endl;
      */
      //return(0);
      // Fork and start gui
      stop = false;
      switch (pid = fork()) {
//      switch (pid=3){ // then it goes directly to default -- test change by wmq
         // Error
         case -1:
            cout << "Error occured in fork!" << endl;
            return(1);
            break;

         // Child
         case 0:
	    usleep(100);
            cout << "Starting GUI" << endl;
            cmd.str("");
            cmd << "cntrlGui localhost " << dec << port;
//            cmd<< "cntrlGui localhost "<<dec<<port-1;
            cout<< "dev: print the cmd -- " <<cmd.str()<<endl;//wmq
            system(cmd.str().c_str());
	    //system("echo dev: muhahaha");
            cout << "GUI stopped" << endl;
	    //cout<<"[dev] pid: "<<getppid()<< "; SIGINT==" << SIGINT<< endl;
            kill(getppid(),SIGINT);
            break;

         // Server
         default:
            cout << "Starting server at port " << dec << port << endl;
            /*int iamcounter=0;
            while ( ! stop )  {// test by wmq
	      	cntrlServer.receive(100);
	        //cout<<"[dev]: fake receiving..."<<endl;
	        iamcounter++;
	        cout<<"[kpixGui32-dev]: while looping => "<<iamcounter<<endl;
	        }*/
            //cout<<"[kpixGui32:dev] &stop=="<<&stop<<endl;
            while(! stop)  cntrlServer.receive(100);
            cntrlServer.stopListen();
            cout << "Stopped server" << endl;
            break;
      }

   } catch ( string error ) {
      cout << "Caught Error: " << endl;
      cout << error << endl;
      cntrlServer.stopListen();
   }
}

