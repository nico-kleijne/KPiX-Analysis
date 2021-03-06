#!usr/bin/python

################
# Mengqing Wu <mengqing.wu@desy.de> 2018-Mar-22
# ----
# simple code to read a txt file to a cxx header file:
#  just to get an unordered_map for other code to use
#  AIM: get rid of relevant/absolute path in a code, to get package flexible 
################

import os,sys

if not len(sys.argv)>=3: raise(Exception, "Usage: python stripMapMaker.py [infile1.txt] [infile2.txt] [fheader.h]")
inpath1 = sys.argv[1]
inpath2 = sys.argv[2]
outpath = sys.argv[3]

infile1 = open(inpath1, 'r') # xxx.txt
infile2 = open(inpath2, 'r') # xxx.txt
fheader = open(outpath, 'w') # xxx.h

fheader.write("#include <unordered_map>\n")
fheader.write("unordered_map<uint, uint> kpix_left(){\n")
fheader.write("  unordered_map<uint, uint> m1;// key is kpix channel id, value is strip id\n")

for line in infile1.readlines():
	if line.strip() and line[0].isdigit():
		fileline = line.split()
		#print line.split()[0], line.split()[1]
		fheader.write("  m1.insert(make_pair("+line.split()[0]+","+line.split()[1]+"));\n")
       
infile1.close()

fheader.write("  return m1;\n}\n")

fheader.write("")
fheader.write("unordered_map<uint, uint> kpix_right(){\n")
fheader.write("  unordered_map<uint, uint> m1;// key is kpix channel id, value is strip id\n")

for line in infile2.readlines():
	if line.strip() and line[0].isdigit():
		fileline = line.split()
		#print line.split()[0], line.split()[1]
		fheader.write("  m1.insert(make_pair("+line.split()[0]+","+line.split()[1]+"));\n")
       
infile2.close()

fheader.write("  return m1;\n}\n")
fheader.close()
