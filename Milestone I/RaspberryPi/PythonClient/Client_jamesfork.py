#!user/bin/python

import spidev 
import time 
import requests
from requests.exceptions import HTTPError
import os 
from flask import Flask
from flask import jsonify

def createSPI(device): 

    spi = spidev.SpiDev() 
    spi.open(0, device)
    spi.max_speed_hz=1000000
    spi.mode = 0
    return spi

if __name__ == '__main__': 
    try: 
        while True:


            sendingList = []

            spi1 = createSPI(0)
            spi2 = createSPI(1)

            #get light value like usual
            currLightValueList = spi1.readbytes(2)
            currLightValue = currLightValueList[0]
            
            #Send and Retreive Updated Data (When we send, the server computes the new average automatically)
            url = 'http://192.168.43.140:5000/submitLightValue/'
            url += str(currLightValue) 
            response = requests.get(url)

            responseSplit = response.text.split()

            average = responseSplit[2]
            average = average[:-1]

            #send to LCD MC
            averageToFLOAT = float(average)
            averageToINT = int(averageToFLOAT)
            print(averageToINT)
            sendingList.append(averageToINT)
            spi2.xfer(sendingList)

            spi1.close()
            spi2.close()

            time.sleep(1)
   
    except KeyboardInterrupt: 
        spi1.close()
        spi2.close()
        exit()


