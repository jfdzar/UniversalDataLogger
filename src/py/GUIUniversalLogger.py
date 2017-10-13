# Logger
# Created by: jfdzar
# Script to get the serial msg of arduino and save it in a csv
#
import pandas as pd
import numpy as np
import time
import Logger

from tkinter import *

def read_logger():
    myLogger = Logger.logger("COM8")
    myLogger.debug = False
    myLogger.connect()
    
    if myLogger.connected:
        data = myLogger.readlog()
        t1.delete(1.0,END)
        t1.insert(CURRENT,data['A0'])
        
def disconnect():
    myLogger.disconnect()

window=Tk()

b_connect=Button(window,text='Connect',command=read_logger)
b_connect.grid(row=0,column=0)
b_disconnect=Button(window,text='Disconnect',command=disconnect)
b_disconnect.grid(row=0,column=1)


t1=Text(window,height=1,width=20)
t1.grid(row=0,column=3)

window.mainloop()
