#
# Labhack: UDP Personal Monitoring 
# 10/26/2014
#
# GNU GPL License
#

from os import listdir
import os.path
import subprocess
import time
import struct
import shutil

#data sources - adapt to server
pathSource = "/home/devinws/Data/"
pathDestination = "/home/devinws/Processed/"
pathArchive = "/home/devinws/Archive/"

#arrays for data processing
#inefficient
epoch_last = 0
lat_last = 0
lon_last = 0
alt_last = 0
ecg_last = 0
o2_last = 0
resp_last = 0
temp_last = 0
ip_last = 0

#run server-side continously
while True :
	if os.listdir(pathSource) == []:
		continue
	files = listdir(pathSource)
	# We used this for debugging: # print (files)
	fname = pathSource + str(files[0]) 
	
	deltaLatitude = 0
	deltaLongitude = 0
	deltaAltitude = 0
	
	if os.path.isfile(fname):
		fih = open(fname, "r")
		
		data = fih.read()
		data = data.split(" ")	

		fih.close()
		
		data[7] = data[7].strip('\n')
		data[7] = data[7].strip('\r')

		epoch = data[0]
		lat = data[1]
		lon = data[2]
		alt = data[3]
		ecg = data[4]
		o2 = data[5]
		resp = data[6]
		temp = data[7]
		ip = data[8]
		
		deltaLatitude = float(lat)-float(lat_last)
		deltaLongitude = float(lon)-float(lon_last)
		deltaAltitude = float(alt)-float(alt_last)
		
		rgbEcg = 'FFFFFF'
		if int(ecg) > 180:
			rgbEcg = 'FF0000'
		elif int(ecg) >= 160:
			rgbEcg = 'FF8000'
		elif int(ecg) > 140:
			rgbEcg = 'FFC800'
		elif int(ecg) >= 120:
			rgbEcg = 'FFFF00'
		elif int(ecg) > 40:
			rgbEcg = '00FF00'
		elif int(ecg) <= 40:
			regEcg = 'FF0000'
			
		if int(o2) > 95:
			rgbo2 = '00FF00'
		else:
			rgbo2 = 'FF0000'
			
		if int(resp) > 30:
			rgbResp = 'FF0000'
		elif int(resp) > 8:
			rgbResp = '00FF00'	
		else:
			rgbResp = 'FF0000'	
			
		if float(temp) > 100:
			rgbTemp = 'FF0000'
		elif float(temp) > 96:
			rgbTemp = '00FF00'	
		else:
			rgbTemp = 'FF0000'	
			
		out = 'time:{0}'.format(epoch) + ':FFFFFF\n'
		out += 'latitude:{0}'.format(lat) + ':FFFFFF\n'
		out += 'deltaLatitude:{0}'.format(deltaLatitude) + ':FFFFFF\n'
		out += 'longitude:{0}'.format(lon) + ':FFFFFF\n'
		out += 'deltaLongitude:{0}'.format(deltaLongitude) + ':FFFFFF\n'
		out += 'altitude:{0}'.format(alt) + ':FFFFFF\n'
		out += 'deltaAltitude:{0}'.format(deltaAltitude) + ':FFFFFF\n'
		out += 'ECG:{0}'.format(ecg) + ':{0}\n'.format(rgbEcg)
		out += 'O2:{0}'.format(o2) + ':{0}\n'.format(rgbo2)		
		out += 'resp:{0}'.format(resp) + ':{0}\n'.format(rgbResp)				
		out += 'temp:{0}'.format(temp) + ':{0}\n'.format(rgbTemp)
		out += 'IP:{0}'.format(ip) + ':FFFFFF\n'

		foh = open(pathDestination + files[0], 'w+')
		foh.write(out)
		foh.close()
		
		if os.listdir(pathSource) != []:
			subprocess.call(["rm", pathSource + files[0]])

		epoch_last = 0
		lat_last = 0
		lon_last = 0
		alt_last = 0
		ecg_last = 0
		o2_last = 0
		resp_last = 0
		temp_last = 0
		ip_last = 0

		print(out)
