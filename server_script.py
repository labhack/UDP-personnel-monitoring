import os
import os.path
import time
import struct

filenum = 1
pathSource = '/home/devinws/Data/'
pathDestination = '/home/devinws/Processed/'

epoch = []
lat = []
lon = []
alt = []
ecg = []
o2 = []
resp = []
temp = []

while True:
	fname = pathSource + str(filenum) + ".ebola"
	
	deltaLatitude = 0
	deltaLongitude = 0
	deltaAltitude = 0
	
	if os.path.isfile(fname):
		fih = open(fname, "r")
		
		data = fih.read()
		data = data.split(" ")	

		fih.close()
		
		data[7] = data[7].strip('\n')
		
		epoch.append(data[0])
		lat.append(data[1])
		lon.append(data[2])
		alt.append(data[3])
		ecg.append(data[4])
		o2.append(data[5])
		resp.append(data[6])
		temp.append(data[7])
		
		deltaLatitude = float(lat[filenum-1])-float(lat[filenum-2])
		deltaLongitude = float(lon[filenum-1])-float(lon[filenum-2])
		deltaAltitude = float(alt[filenum-1])-float(alt[filenum-2])
		
		if int(ecg[filenum-1]) > 180:
			rgbEcg = 'FF0000'
		elif int(ecg[filenum-1]) >= 160:
			rgbEcg = 'FF8000'
		elif int(ecg[filenum-1]) > 140:
			rgbEcg = 'FFC800'
		elif int(ecg[filenum-1]) >= 120:
			rgbEcg = 'FFFF00'
		elif int(ecg[filenum-1]) > 40:
			rgbEcg = '00FF00'
		elif int(ecg[filenum-1]) <= 40:
			regEcg = 'FF0000'
			
		if int(o2[filenum-1]) > 95:
			rgbo2 = '00FF00'
		else:
			rgbo2 = 'FF0000'
			
		if int(resp[filenum-1]) > 30:
			rgbResp = 'FF0000'
		elif int(resp[filenum-1]) > 8:
			rgbResp = '00FF00'	
		else:
			rgbResp = 'FF0000'	
			
		if float(temp[filenum-1]) > 100:
			rgbTemp = 'FF0000'
		elif float(temp[filenum-1]) > 96:
			rgbTemp = '00FF00'	
		else:
			rgbTemp = 'FF0000'	
			
		out = 'time:{0}'.format(epoch[filenum-1]) + ':FFFFFF\n'
		out += 'latitude:{0}'.format(lat[filenum-1]) + ':FFFFFF\n'
		out += 'deltaLatitude:{0}'.format(deltaLatitude) + ':FFFFFF\n'
		out += 'longitude:{0}'.format(lon[filenum-1]) + ':FFFFFF\n'
		out += 'deltaLongitude:{0}'.format(deltaLongitude) + ':FFFFFF\n'
		out += 'altitude:{0}'.format(alt[filenum-1]) + ':FFFFFF\n'
		out += 'deltaAltitude:{0}'.format(deltaAltitude) + ':FFFFFF\n'
		out += 'ECG:{0}'.format(ecg[filenum-1]) + ':{0}\n'.format(rgbEcg)
		out += 'O2:{0}'.format(o2[filenum-1]) + ':{0}\n'.format(rgbo2)		
		out += 'resp:{0}'.format(resp[filenum-1]) + ':{0}\n'.format(rgbResp)				
		out += 'temp:{0}'.format(temp[filenum-1]) + ':{0}\n'.format(rgbTemp)

		foh = open(pathDestination + str(filenum) + ".ebola", 'w+')
		foh.write(out)
		foh.close()
		
		print(out)
		filenum = filenum + 1
