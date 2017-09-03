# Logger Class
# Created by: jfdzar
# 

import serial
import time
import pandas as pd
import numpy as np


class logger:
    serid = 100
    num_of_loggers = 0
    baud = 9600
    serial_timeout = 1
    connect_timeout = 1
    debug = False
    
    def __init__(self, serialport):
        self.serialport = serialport
        self.connected = False
        self.moving = False
        self.pumping = False

    def connect(self):        
        try:
            if (self.debug): print ("Trying to connect to: " + self.serialport)
            self.ser = serial.Serial(self.serialport, self.baud, timeout=self.connect_timeout)
            time.sleep(self.connect_timeout)
            self.connected = True
            self.ser.readline()
            self.ser.readline()
            if (self.debug): print ("Connected!")
            return True
        except Exception as e:
            if (self.debug): print ("Error trying to connect to: " + self.serialport + " - " + str(e))
            self.connected = False
            return False

    def disconnect(self):
        if self.connected:
            if (self.debug): print ("Closing serial connection")
            self.connected = False
            self.ser.close()
        else:
            if (self.debug): print ("Disconnected called while not connected")
            
    def readlog(self):
        try:
            Msg = self.ser.readline()
            if (self.debug): print (Msg)
            Msg = str(Msg)[2:].split(';')
            item = {
                "Timestamp": time.time(),
                "A0": int(Msg[0]),
                "A1": int(Msg[1]),
                "A2": int(Msg[2]),
                "A3": int(Msg[3]),
                "A4": int(Msg[4]),
                "A5": int(Msg[5]),
                "F_DI5": int(Msg[6]),
                "F_DI6": int(Msg[7]),
                "F_DI7": int(Msg[8])}
            return item
        except:
            if (self.debug): print ("Error Reading Serial")

    def sendcmd(self, cmnd, waitresponse):
        if (self.connected):
            id = self.serid
            self.serid += 1
            cmnd = "#{} {}".format(id,cmnd)
            cmndString = bytes(cmnd + "\n",'ascii')
            if (self.debug): print ("Serial send: {}".format(cmndString))
            self.ser.write(cmndString)
            if (waitresponse):
                line = self.ser.readline()
                while not str(line)[2:3+len(str(id))] == "$" + str(id):
                    line = self.ser.readline()
                if (self.debug): print ("Response {}".format(line))
                if (self.moving):
                    self.moving = False
                    time.sleep(self.delay_after_move)
                return line
        else:
            if (self.debug):
                print ("error, trying to send command while not connected")
                self.moving = False



    def mode(self, modeid):
        # 0= Normal
        # 1= Laser
        # 2= 3D Printer
        # 3= Universal holder
        cmd = protocol.SET_MODE.format(modeid)
        self.sendcmd(cmd,True)

    def set_acceleration(self,p,t):
        self.moving = True
        p = str(round(p, 0))
        t = str(round(t, 0))
        cmd = protocol.SET_ACCELERATION.format(p,t)
        self.sendcmd(cmd, True)

    @staticmethod
    def PointsInCircum(r,n):
        return [(math.cos(2*pi/n*x)*r,math.sin(2*pi/n*x)*r) for x in xrange(0,n+1)]
