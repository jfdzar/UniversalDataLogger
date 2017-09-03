
import pandas as pd
import numpy as np

import serial

ser = serial.Serial("COM8", 9600, timeout=1)

Ready = False
while (not Ready):
    line = ser.readline()
    print (str(line)[2:].split(';'))