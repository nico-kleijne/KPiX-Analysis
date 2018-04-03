#!usr/bin/python

import numpy as np

#if there is no channel between A610 and A919 etc. change the +22 to +20 and change the middle and top start channels by -1 and -2 respectively

def block_map_left (kpix_num, strip_num, channel_elements, slope):
	if "+" in slope:
		for k in xrange(15):
			for h in xrange(10):
				#writefile.write( "%d %d \n" %(kpix_num, strip_num))
				channel_elements[kpix_num]=strip_num
				kpix_num=kpix_num+1
				strip_num=strip_num+1
			kpix_num=kpix_num+22
			strip_num=strip_num-20
	if "-" in slope:
		for k in xrange(15):
			for h in xrange(10):
				#writefile.write( "%d %d \n" %(kpix_num, strip_num))
				channel_elements[kpix_num]=strip_num
				kpix_num=kpix_num+1
				strip_num=strip_num-1
			kpix_num=kpix_num+22
		
		
def block_map_right (kpix_num, strip_num, channel_elements, slope):
	if "+" in slope:
		for k in xrange(15):
			for h in xrange(10):
				#writefile.write( "%d %d \n" %(kpix_num, strip_num))
				channel_elements[kpix_num]=strip_num
				kpix_num=kpix_num+1
				strip_num=strip_num-1
			kpix_num=kpix_num+22
			strip_num=strip_num+20
	if "-" in slope:	
		for k in xrange(15):
			for h in xrange(10):
				#writefile.write( "%d %d \n" %(kpix_num, strip_num))
				channel_elements[kpix_num]=strip_num
				kpix_num=kpix_num+1
				strip_num=strip_num+1
			kpix_num=kpix_num+22
		
		
with open("kpix_to_tracker.txt", "w") as writefile:
	writefile.write( "\n%s \n" %( "==============="))
	writefile.write( "%s \n" %( "Left kpix"))
	writefile.write( "%s %s \n" %( "KPIX_channel","tracker_strip-number"))
	
	## ================= EXPLANATION ==================
	## I split the entire kpix into 7 blocks shown by the jumps in the strip channel numbering from the
	## gds file a right side, a left side with a bottom, a middle and a top block each having 15 rows
	## with 10 channels per row. The center area is looked at as its own block because of the unique
	## structure
	channel_elements_left = np.zeros(1024)
	
	# block 1 (bottom right)
	block_map_left(0, 910, channel_elements_left, '+')
	# block 2 (middle right)
	block_map_left(11, 610, channel_elements_left, '+')
	# block 3 (top right)
	block_map_left(22, 769, channel_elements_left, '-')
	# block 4 (bottom left)
	block_map_left(544, 299, channel_elements_left, '-')
	# block 5 (middle left)
	block_map_left(555, 440, channel_elements_left, '+')
	# block 6 (top left)
	block_map_left(566, 140, channel_elements_left, '+')
	# block 7 (center) #only 2 rows and only center area
	kpix_channel = 491
	strip = 460
	for k in xrange(2):
		for h in xrange(10):
			channel_elements_left[kpix_channel]=strip
			kpix_channel=kpix_channel+1
			strip=strip+1
		kpix_channel=kpix_channel+22
		strip=strip-20
	counter = 0
	
	## Writing it into a txt file sorted by the kpix channel number
	for element in channel_elements_left:
		# non assigned channels also show as 0 but to make sure there is no misunderstanding with the actual kpix channel that is assigned to strip 0 (i.e. 1014) they are all taken out.
		if (element == 0) and (counter != 1014):
			writefile.write( "%d %d \n" %(counter, 9999))
		elif (counter == 1014) or (element != 0):
			writefile.write( "%d %d \n" %(counter, element))
		counter = counter + 1
	writefile.write( "\n%s \n" %( "==============="))
	writefile.write( "%s \n" %( "Right kpix"))
	writefile.write( "%s %s \n" %( "KPIX_channel","tracker_strip_number"))
	channel_elements_right = np.zeros(1024)	
	
	# block 1 (top left)
	block_map_right(0, 929, channel_elements_right, '+')
	# block 2 (middle left)
	block_map_right(11, 1229, channel_elements_right, '+')
	# block 3 (bottom left)
	block_map_right(22, 1070, channel_elements_right, '-')
	# block 4 (top right)
	block_map_right(544, 1540, channel_elements_right, '-')
	# block 5 (middle right)
	block_map_right(555, 1399, channel_elements_right, '+')
	# block 6 (bottom right)
	block_map_right(566, 1699, channel_elements_right, '+')
	# block 7 (center) #only 2 rows and only center area
	kpix_channel = 491
	strip = 1379
	for k in xrange(2):
		for h in xrange(10):
			channel_elements_right[kpix_channel]=strip
			kpix_channel=kpix_channel+1
			strip=strip-1
		kpix_channel=kpix_channel+22
		strip=strip+20
	counter = 0
	## Writing it into a txt file sorted by the kpix channel number
	for element in channel_elements_right:
		#if (element < 920):
		#	print counter," ", element
		if (element == 0):
			writefile.write( "%d %d \n" %(counter, 9999))
		else:
			writefile.write( "%d %d \n" %(counter, element))
		counter += 1
	
	
	### Pointing out all channels that are not connected to a strip
	#writefile.write( "\n%s \n" %( "==============="))
	#writefile.write( "%s \n" %("The following channels on the left KPiX were not connected to any strip"))
	#counter = 0
	#for element in channel_elements_left:
		## non assigned channels also show as 0 but to make sure there is no misunderstanding with the actual kpix channel that is assigned to strip 0 (i.e. 1014) they are all taken out.
		#if (element == 0) and (counter != 1014):
			#writefile.write( "%d \n" %(counter))
		#counter += 1
	#writefile.write( "\n%s \n" %( "==============="))
	#writefile.write( "%s \n" %("The following channels on the right KPiX were not connected to any strip"))
	#counter = 0
	#for element in channel_elements_right:
		## non assigned channels also show as 0 but to make sure there is no misunderstanding with the actual kpix channel that is assigned to strip 0 (i.e. 1014) they are all taken out.
		#if (element == 0):
			#writefile.write( "%d \n" %(counter))
		#counter += 1
