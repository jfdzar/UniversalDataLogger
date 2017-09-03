# Logger
# Created by: jfdzar
# Script to get the serial msg of arduino and save it in a csv
# 
import pandas as pd
import numpy as np
import time

import Logger

path = ''
filename = 'log_file'

myLogger = Logger.logger("COM8")
myLogger.debug = False
myLogger.connect()

log_list = []

try:
    while myLogger.connected:
        log_list.append(myLogger.readlog())
        df = pd.DataFrame(log_list)
        #Save Data Frame as CSV
        df.to_csv(path+filename+'.csv')
        # Save Data Frame as Excel File
        writer = pd.ExcelWriter(path+filename+'.xlsx', engine='xlsxwriter')
        df.to_excel(writer, sheet_name='Sheet1')
        writer.save()
        
        print(df.tail(1))
except KeyboardInterrupt:
    pass

print("Finished")