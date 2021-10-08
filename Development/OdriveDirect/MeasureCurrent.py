#!C:\Users\User\anaconda3\python.exe
"""
Example usage of the ODrive python library to monitor and control ODrive devices
"""

from __future__ import print_function

import odrive
from odrive.enums import *
import time
import math

## Inputs
Axis = 3
Test = 1
moveto = 10

## Write File
titlestr = "Axis" + str(Axis) + "_Test" + str(Test) +".txt"
f = open(titlestr, "w")
f.write("pos,current \n")

print(titlestr)

## Connect and Calibrate
# Find a connected ODrive (this will block until you connect one)
print("finding an odrive...")
my_drive = odrive.find_any()

# f.write("Bus voltage is " + str(my_drive.vbus_voltage) + "V")

# Calibrate motor and wait for it to finish
print("starting calibration...")
my_drive.axis0.requested_state = AXIS_STATE_FULL_CALIBRATION_SEQUENCE
while my_drive.axis0.current_state != AXIS_STATE_IDLE:
    time.sleep(0.1)

my_drive.axis0.requested_state = AXIS_STATE_CLOSED_LOOP_CONTROL

print("Current pos: "+str(my_odrive.axis0.encoder.pos_estimate))

E = my_drive.axis0.encoder.pos_estimate

print("Bus voltage is " + str(E) + "V")

# ## Move and measure
# time.sleep(1)
# print("Now gunna move")
# time.sleep(1)

# for i in [3, 2, 1]:
    # print(str(i))
    # time.sleep(1)
# print("Moving!")

# my_drive.axis0.controller.input_pos = moveto

# D = my_odrive.axis0.encoder.pos_estimate
# d = abs(D)-abs(moveto)

# while d > 1:
    # D = my_odrive.axis0.encoder.pos_estimate
    # d = abs(D)-abs(moveto)
    
    # f.write(str(my_odrive.axis0.encoder.pos_estimate))
    # f.write(",")
    # f.write(str(odrv0.axis0.motor.current_control.Iq_measured))
    # f.write("\n")

f.close()
time.sleep(5)



