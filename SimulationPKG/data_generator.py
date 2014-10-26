#
# Labhack: UDP Personal Monitoring 
# 10/26/2014
#
# GNU GPL License
#

import os
import os.path
import time
import random

i=1
epoch = 1414285557
altitude = 304.8
bpm = 100
o2 = 99
respiration = 14
temp = 98.8
path = 'C:\\Users\\devin_000\\Desktop\\'

ifstream = path + '4798.txt'
ofstream = path + 'outbreakdata2.txt'	
	
if os.path.isfile(ifstream):
	fhi = open(ifstream, 'r')
	fho = open(ofstream, 'w')
	
	
	while i < 5000:
		altitude = altitude + (2.5 * random.random() - 1.25)
		
		bpm = bpm + (4 * random.random() - 2)
		if bpm > 255:
			bpm = 255
		if bpm < 0:
			bpm = 0
		
		o2 = o2 + (.4 * random.random() - .2)
		if o2 > 100: 
			o2 = 100
		if o2 < 50:
			o2 = 50
			
		temp = temp + (.3 * random.random() - .15)
		if temp > 105: 
			temp = 105
		if temp < 95:
			temp = 95
		
		respiration = respiration + (1.6 * random.random() - .8)
		if respiration > 50: 
			respiration = 50
		if respiration < 0:
			respiration = 0

		data = fhi.readline()	
		data = data.split(",")	
		data[3] = data[3].strip('\n')
		
		datastr = str(epoch) + " " + str(data[2]) + " " + str(data[3]) + " " + str("{0:.2f}".format(altitude)) + " " + str(int(bpm))
		datastr += " " + str(int(o2)) + " " + str(int(respiration)) + " " + str("{0:.2f}".format(temp)) + "\n"
		fho.writelines(datastr)
		data = fhi.readline()
		epoch = epoch + 5
		i = i + 1
fhi.close()
fho.close()