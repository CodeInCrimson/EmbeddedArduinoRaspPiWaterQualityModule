import serial
import time

import os
from time import sleep
from datetime import datetime

if __name__ == '__main__':

    ser = serial.Serial('/dev/ttyUSB0',9600,timeout=1)
    sent=False

    file = open("/home/pi/ship_project.csv", "a")
    if os.stat("/home/pi/ship_project.csv").st_size == 0:
        file.write("Date/Time,Latitude,Longitude,Water Temperature (*C),Water Temperature (*F),Water Flow (L/hour),Impurities in water (ppm),Atmospheric Temperature (*C),Atmospheric Pressure (Pa),Altitude of Ship (m),pH of Water\n")
        
    while True:
        time.sleep(1)
        data = ser.readline().decode('utf-8').rstrip()
        print (data)
        #print(list(data))
        lengthofstring = len(data)
        print("length of string: ",lengthofstring)
        #print((data[-1]))
        if lengthofstring ==102:
            dd=data.split(",")
            print(dd)
            lengthofsplittedstring=len(dd)
            print("length of dd: ",lengthofsplittedstring)
            #for i in range(0,lengthofsplittedstring):
            if lengthofsplittedstring is 16:
                
                now = datetime.now()
                
                latf=(float(dd[1])+((float(dd[2]))/60.0))
                lonf=(float(dd[4])+((float(dd[5]))/60.0)) 
                
                
                lats=str(latf)
                lons=str(lonf)
                #print("LATITUDE STR: ",lats)
                #print("LONGITUDE STR: ",lons)
                
                latitude=str(lats + "*" + dd[3])
                longitude=str(lons + "*" + dd[6])
                print("LATITUDE: ",latitude)
                print("LONGITUDE: ",longitude)
                
                Temp_water_C = dd[7]
                Temp_water_F = dd[8]
                print("Temperature of Water in Degree Celcius: ",Temp_water_C)
                print("Temperature of Water in Degree Fahrenheit: ",Temp_water_F)
                
                Water_Flow = dd[9]
                print("Water Flow: ",Water_Flow," L/hour")
                
                Impurities = dd[10]
                print("Impurities in Water: ",Impurities," Parts per million (ppm)")
                
                Temperature_Atmosphere = dd[11]
                print("Atmospheric Temperature: ",Temperature_Atmosphere)
            
                Pressure_Atmosphere = dd[12]
                print("Atmospheric Pressure: ",Pressure_Atmosphere," Pa")
            
                Altitude = dd[13]
                print("Altitude of ship is: ",Altitude," m")
            
                pH = dd[14]
                print("pH of Water: ",pH)
                
                
                file= open("/home/pi/ship_project.csv", "a")
                time.sleep(0.5)
                file.write(str(now)+","+str(latitude)+","+str(longitude)+","+str(Temp_water_C)+","+str(Temp_water_F)+","+str(Water_Flow)+","+str(Impurities)+","+str(Temperature_Atmosphere)+","+str(Pressure_Atmosphere)+","+str(Altitude)+","+str(pH)+"\n")
                file.flush()
                time.sleep(5)
                file.close()
                
                
                strnow = str(now)
                ss=strnow[15]
                ww= str(ss)
                intnow=int(ww)
                if((intnow%2) is 0):
                    os.system("python /home/pi/drive-uploader.py >/home/pi/logs/cronlogs1 @>&1")
            else:
                print("Splitted string length not appropriate")
        else:
            print("Input string length not appropriate")
                            
            
                
