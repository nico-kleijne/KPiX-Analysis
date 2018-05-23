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
					for refuse in args.refuse:
						for channel in args.channel:	
							for bucket in args.bucket:
								for kpix in args.kpix:
									if ((name in key_object.GetName() and refuse not in key_object.GetName()) \
									and (key_object.ReadObj().GetEntries() != 0) \
									and ('c'+str(channel)+'_' in key_object.GetName() or channel == 9999) and ('b'+str(bucket)+'_' in key_object.GetName() or bucket == 9999) and ('k'+str(kpix) in key_object.GetName() or kpix == 9999)):
										print 'Histogram found: ', key_object.GetName()
										hist_list.append(key_object)
							#if ((name in key_object.GetName()) and (key_object.ReadObj().GetEntries() != 0) and ('c'+str(channel) in key_object.GetName() or channel == 9999) and ('b'+str(args.test1) in key_object.GetName() or args.test1 == 9999 or 'b'+str(args.test2) in key_object.GetName())):
									#print key_object.GetName()
									#hist_list.append(key_object)
	


mystyle = ROOT.TStyle("mystyle", "My Style")

#set the background color to white
mystyle.SetFillColor(10)
mystyle.SetFrameFillColor(10)
mystyle.SetCanvasColor(10)
mystyle.SetPadColor(10)
mystyle.SetTitleFillColor(0)
mystyle.SetStatColor(10)

#dont put a colored frame around the plots
mystyle.SetFrameBorderMode(0)
mystyle.SetCanvasBorderMode(0)
mystyle.SetPadBorderMode(0)
mystyle.SetLegendBorderSize(0)
#
##use the primary color palette
##mystyle.SetPalette(1,0)
#
##set the default line color for a histogram to be black
#mystyle.SetHistLineColor(1)
#
##set the default line color for a fit function to be red
#mystyle.SetFuncColor(2)
#
##make the axis labels black
#mystyle.SetLabelColor(1,"xyz")
#
##set the default title color to be black
#mystyle.SetTitleColor(1)
#
##set the margins
##mystyle.SetPadBottomMargin(0.18)
##mystyle.SetPadTopMargin(0.08)
##mystyle.SetPadRightMargin(0.08)
##mystyle.SetPadLeftMargin(0.17)
#
##set axis label and title text sizes
#mystyle.SetLabelFont(42,"xyz")
#mystyle.SetLabelSize(0.06,"xyz")
#mystyle.SetLabelOffset(0.015,"xyz")
#mystyle.SetTitleFont(42,"xyz")
mystyle.SetTitleSize(0.04,"xyz")
mystyle.SetTitleOffset(1.2,"yz")
#mystyle.SetTitleOffset(1.0,"x")
#mystyle.SetStatFont(42)
#mystyle.SetStatFontSize(0.07)
#mystyle.SetTitleBorderSize(0)
#mystyle.SetStatBorderSize(0)
#mystyle.SetTextFont(42)

##set legend text size etc.
mystyle.SetLegendTextSize(0.03)
#
##set line widths
mystyle.SetFrameLineWidth(2)
mystyle.SetFuncWidth(2)
mystyle.SetHistLineWidth(2)
#
##set the number of divisions to show
#mystyle.SetNdivisions(506, "xy")
#
##turn off xy grids
#mystyle.SetPadGridX(0)
#mystyle.SetPadGridY(0)
#
##set the tick mark style
#mystyle.SetPadTickX(1)
#mystyle.SetPadTickY(1)
#
##turn off stats
#mystyle.SetOptStat(0)
#mystyle.SetOptFit(0)
#
##marker settings
#mystyle.SetMarkerStyle(20)
#mystyle.SetMarkerSize(0.7)
mystyle.SetLineWidth(2) 

#done
mystyle.cd()
#ROOT.gROOT.SetBatch(ROOT.kFALSE) # do not show histogram when drawing
ROOT.gROOT.ForceStyle()
ROOT.gStyle.ls()













parser = argparse.ArgumentParser() #Command line argument parser.
parser.add_argument('file_in', help='name of the input file')
parser.add_argument('-n', '--name', dest='name', default=['everything'], nargs='*',  help='used to specify the name of the plot which should be used')
parser.add_argument('-c', '--channel', dest='channel', default=[9999], nargs='*', type=int, help='used to specify the channel of the plot which should be used')
parser.add_argument('-b', '--bucket', dest='bucket', default=[9999], nargs='*', type=int, help='used to specify the bucket of the plot which should be used')
parser.add_argument('-k', '--kpix', dest='kpix', default=[9999], nargs='*', type=int, help='used to specify the bucket of the plot which should be used')
parser.add_argument('-d', '--draw', dest='draw_option', default='hist e', help='specify the drawing option as given by the root draw option, needs to be given as a single string (e.g. hist same)')
parser.add_argument('-o', '--output', dest='output_name', default='test.png', help='specifies the name and type of the output file (e.g. test.png, comparison.root etc...')
parser.add_argument('-r', '--refuse', dest='refuse', default= ['nothing'], nargs='*', help='add string that should be exluded from histogram search')

args = parser.parse_args()
print ''
if ((not args.file_in) or ('root' not in args.file_in)):
	print "Please specify a root file as first argument"
	quit()

root_file = ROOT.TFile(args.file_in)

print 'Looking for histograms'
print '----------------------'
print 'Name contains ', args.name
print 'Channel [9999 = everything] ',args.channel
print 'Bucket [9999 = everything] ',args.bucket
print 'KPiX [9999 = everything] ',args.kpix

key_root = root_file.GetListOfKeys()
loopdir(key_root)
print 'Number of histograms found is: ', len(hist_list)
print hist_list



if ('same' in args.draw_option):
	drawing_option = args.draw_option.replace('same', 'NOSTACK') #exchange the same with a NOSTACK as I am using THStack
	c1 = ROOT.TCanvas( args.output_name, 'Test', 1200, 900 )
	c1.cd()
	c1.SetFillColor(0)
	legend = ROOT.TLegend(0.65,0.88,0.85,0.8)
	hist_comp = ROOT.THStack()
	counter = 1
	x_title = None
	y_title = None
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
		hist_comp.Add(obj)
		legend.AddEntry(obj, histogram.GetName())
		counter +=1
		x_title = x_axis.GetTitle()
		y_title = y_axis.GetTitle()
	hist_comp.Draw(drawing_option)
	
	xaxis = hist_comp.GetXaxis()
	xaxis.SetTitle(x_title)
	xaxis.SetRange(x_low, x_high)
	
	yaxis = hist_comp.GetYaxis()
	yaxis.SetTitle(y_title)
	legend.Draw()
	c1.SaveAs('/home/lycoris-dev/Documents/'+args.output_name)
	c1.Close()
else:
	c1 = ROOT.TCanvas( 'test', 'Test', 1200,900 ) #
	for histogram in hist_list:
		
		obj = histogram.ReadObj()
		x_axis = obj.GetXaxis()
		y_axis = obj.GetYaxis()
		c1.cd()
		#print obj.GetEntries()
		x_high = obj.FindLastBinAbove(0)+10
		x_low = obj.FindFirstBinAbove(0)-10
		x_axis.SetRange(x_low, x_high)
		obj.SetLineColor(4) #Blue
		obj.Draw('hist')
		c1.SaveAs('/home/lycoris-dev/Documents/'+histogram.GetName()+'.png')
	c1.Close()


#raw_input('Press Enter to look at the next histogram')


