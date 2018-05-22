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


def loopdir(keys):  # loop through all subdirectories of the root file and add all plots+histograms that contain the same string as given by the name, channel and bucket arguments to the global list of all chosen histograms hist_list
	for key_object in keys:
		if ('TDirectory' in key_object.GetClassName()):
			loopdir(key_object.ReadObj().GetListOfKeys())
		else:
			if(args.name == 'everything' and key_object.ReadObj().GetEntries() != 0):
				print key_object.GetName()
				hist_list.append(key_object)
			else:	
				for name in args.name:
					for channel in args.channel:	
						for bucket in args.bucket:
							for kpix in args.kpix:
								if ((name in key_object.GetName()) \
								and (key_object.ReadObj().GetEntries() != 0) \
								and ('c'+str(channel)+'_' in key_object.GetName() or channel == 9999) and ('b'+str(bucket)+'_' in key_object.GetName() or bucket == 9999) and ('k'+str(kpix)+'_' in key_object.GetName() or kpix == 9999)):
									print 'Histogram found: ', key_object.GetName()
									hist_list.append(key_object)
						#if ((name in key_object.GetName()) and (key_object.ReadObj().GetEntries() != 0) and ('c'+str(channel) in key_object.GetName() or channel == 9999) and ('b'+str(args.test1) in key_object.GetName() or args.test1 == 9999 or 'b'+str(args.test2) in key_object.GetName())):
								#print key_object.GetName()
								#hist_list.append(key_object)
	


parser = argparse.ArgumentParser() #Command line argument parser.
parser.add_argument('file_in', help='name of the input file')
parser.add_argument('-n', '--name', dest='name', default=['everything'], nargs='*',  help='used to specify the name of the plot which should be used')
parser.add_argument('-c', '--channel', dest='channel', default=[9999], nargs='*', type=int, help='used to specify the channel of the plot which should be used')
parser.add_argument('-b', '--bucket', dest='bucket', default=[9999], nargs='*', type=int, help='used to specify the bucket of the plot which should be used')
parser.add_argument('-k', '--kpix', dest='kpix', default=[9999], nargs='*', type=int, help='used to specify the bucket of the plot which should be used')
parser.add_argument('-d', '--draw', dest='draw_option', default='hist e', help='specify the drawing option as given by the root draw option, needs to be given as a single string (e.g. hist same)')
parser.add_argument('-o', '--output', dest='output_name', default='test.png', help='specifies the name and type of the output file (e.g. test.png, comparison.root etc...')


args = parser.parse_args()

if ((not args.file_in) or ('root' not in args.file_in)):
	print "Please specify a root file as first argument"
	quit()

root_file = ROOT.TFile(args.file_in)

print 'Looking for histograms'
print 'Name contains ', args.name
print 'Channel [9999 = everything] ',args.channel
print 'Bucket [9999 = everything] ',args.bucket
print 'KPiX [9999 = everything] ',args.kpix

key_root = root_file.GetListOfKeys()
loopdir(key_root)
print 'Number of histograms found is: ', len(hist_list)
#print hist_list

ROOT.gROOT.SetStyle("Default");

if ('same' in args.draw_option):
	drawing_option = args.draw_option.replace('same', 'NOSTACK') #exchange the same with a NOSTACK as I am using THStack
	c1 = ROOT.TCanvas( args.output_name, 'Test', 1600, 1200 )
	c1.cd()
	hist_comp = ROOT.THStack();
	counter = 1
	x_low = None
	x_high = None
	for histogram in hist_list:
		obj = histogram.ReadObj()
		x_axis = obj.GetXaxis()
		y_axis = obj.GetYaxis()
		if (x_low is None):
			x_low = obj.FindFirstBinAbove(0)-10
		elif (x_low > obj.FindFirstBinAbove(0)-10):
			x_low = obj.FindFirstBinAbove(0)-10
		if (x_high is None):
			x_high = obj.FindLastBinAbove(0)+10
		elif (x_high < obj.FindLastBinAbove(0)+10):
			x_high = obj.FindLastBinAbove(0)+10
		print x_low, x_high
		#x_axis.SetRangeUser(x_low, x_high)
		obj.SetLineColor(counter) #First will be black, second red, third green etc.
		#obj.Draw(args.draw_option)
		hist_comp.Add(obj);
		counter +=1
	hist_comp.Draw(drawing_option)
	xaxis = hist_comp.GetXaxis()
	xaxis.SetRangeUser(x_low, x_high)
	c1.SaveAs('/home/lycoris-dev/Documents/'+args.output_name)
	
else:
	for histogram in hist_list:
		c1 = ROOT.TCanvas( histogram.GetName(), 'Test', 1600, 1200 )
		obj = histogram.ReadObj()
		x_axis = obj.GetXaxis()
		y_axis = obj.GetYaxis()
		c1.cd()
		x_axis.SetRangeUser(obj.FindFirstBinAbove(0)-10, obj.FindLastBinAbove(0)+10)
		obj.SetLineColor(4) #Blue
		obj.Draw(args.draw_option)
		c1.Update()
		c1.SaveAs('/home/lycoris-dev/Documents/'+histogram.GetName()+'.png')

raw_input('Press Enter to exit')
	#raw_input('Press Enter to look at the next histogram')


