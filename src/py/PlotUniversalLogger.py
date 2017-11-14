import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import random


data_log = []

data_entry = {
    "Timestamp": 0,
    "T1":0,
    "T2":0}

data_log.append(data_entry)
df = pd.DataFrame(data_log)

plt.ion()
ax = plt.gca()
ax.set_autoscale_on(True)


line_t1, = ax.plot(df['Timestamp'], df['T1'],label="T1")
line_t2, = ax.plot(df['Timestamp'], df['T2'],label="T2")

i=0

ax.set_title('Temperature Logger')
ax.set_xlabel('Timestamp (s)')
ax.set_ylabel('Temperature (C)')
box = ax.get_position()

# Shrink current axis's height by 10% on the bottom
ax.set_position([box.x0, box.y0 + box.height * 0.05,
                 box.width, box.height * 0.95])

# Put a legend below current axis
ax.legend(loc='upper center', bbox_to_anchor=(0.5, -0.1),
          fancybox=True, shadow=False, ncol=5)

while 1:
    i=i+1
    deviation = int(i/15)
    data_entry = {
    "Timestamp": i,
    "T1":(5+random.uniform(1,deviation)),
    "T2":(6+random.uniform(1,deviation))}
    
    data_log.append(data_entry)
    df = pd.DataFrame(data_log)

    line_t1.set_xdata(df['Timestamp'])
    line_t1.set_ydata(df['T1'])
    line_t2.set_xdata(df['Timestamp'])
    line_t2.set_ydata(df['T2'])
    
    ax.relim()
    ax.set_xlim(0,120)
    ax.set_ylim(0,round(max(df.drop(['Timestamp'],axis=1).max()),0)+5)
    
    if df['Timestamp'].iloc[-1] > 120:
        ax.set_xlim(df['Timestamp'].iloc[-1]-120,df['Timestamp'].iloc[-1])
        
    #ax.autoscale_view(True,True,True)
    plt.draw()
    plt.pause(0.1)
