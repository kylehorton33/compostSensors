## import libraries
import serial
import time
import datetime
import pandas as pd
import matplotlib.pyplot as plt

## set COM and wait for connection (Arudino Serial Monitor must be closed)
comm_error = True
while comm_error:
    try:
        ser = serial.Serial('COM3', 9600)
        print('Connected to COM3!')
        comm_error = False
    except:
        print('Waiting for connection to COM3...')
        time.sleep(3)

## read 100 cycles from serial and save to dataframe
number_of_cycles = 5
counter = 0
df = pd.DataFrame(columns=['timestamp','temperature'])   # set up dataframe
start_time = datetime.datetime.now()                     # log start time of data collection

while (counter < number_of_cycles):
    try:
        serial_data = ser.readline()
        decoded_serial_data = float(serial_data[0:len(serial_data)-2].decode("utf-8"))
        time_now = datetime.datetime.now()
        print("Counter: {}  Time: {}  Serial:  {}".format(counter,time_now,decoded_serial_data))
        df = df.append(pd.Series([time_now,decoded_serial_data], index=df.columns ), ignore_index=True)
        #time.sleep(2)
        counter+=1
        
    except:
        print("keyboard interrupt")
        break

## plot data and save figure to png
df.plot(kind='line',x='timestamp',y='temperature',color='blue')
plt.savefig(start_time.strftime("%Y%m%d%H%M%S")+'.png')
