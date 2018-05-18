#!usr/bin/python

import numpy as np
import matplotlib.pyplot as plt
import string
import matplotlib as mpl
import matplotlib.lines as mlines
import matplotlib.ticker as mticker
from mpl_toolkits.mplot3d import Axes3D
from matplotlib import cm
import ROOT
import argparse
import argcomplete
from operator import add



hist_list= []  #Global list of all chosen histograms histograms


def loopdir(keys):  # loop through all subdirectories of the root file and add all plots+histograms that contain the same string as given in args.contains to the global list of all chosen histograms hist_list
	for key_object in keys:
		if ('TDirectory' in key_object.GetClassName()):
			loopdir(key_object.ReadObj().GetListOfKeys())
		else:
			if(args.contains == 'everything' and key_object.ReadObj().GetEntries() != 0):
				print key_object.GetName()
				hist_list.append(key_object)
			else:
				if (args.contains in key_object.GetName() and key_object.ReadObj().GetEntries() != 0):
					print key_object.GetName()
					hist_list.append(key_object)
	


parser = argparse.ArgumentParser() #Command line argument parser.
parser.add_argument('file_in', help='name of the input file')
parser.add_argument('-c', '--contains', dest='contains', default='everything', help='use to specify which plot should be used')

args = parser.parse_args()

if ((not args.file_in) or ('root' not in args.file_in)):
	print "Please specify a root file as first argument"
	quit()

root_file = ROOT.TFile(args.file_in)

key_root = root_file.GetListOfKeys()
loopdir(key_root)
print len(hist_list)



ROOT.gROOT.SetStyle("Default");
for histogram in hist_list:
	obj = histogram.ReadObj()
	x_axis = obj.GetXaxis()
	y_axis = obj.GetYaxis()
	c1 = ROOT.TCanvas( histogram.GetName(), 'Test', 800, 600 )
	c1.cd()
	x_axis.SetRangeUser(obj.FindFirstBinAbove(0)-10, obj.FindLastBinAbove(0)+10)
	obj.Draw('hist')
	c1.Update()
raw_input('Press Enter to exit')
	#raw_input('Press Enter to look at the next histogram')


