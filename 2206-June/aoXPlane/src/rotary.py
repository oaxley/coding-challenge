# -*- coding: utf-8 -*-
# vim: set ft=python
#
# This source file is subject to the Apache License 2.0
# that is bundled with this package in the file LICENSE.txt.
# It is also available through the Internet at this address:
# https://opensource.org/licenses/Apache-2.0
#
# @author	Sebastien LEGRAND
# @license	Apache License 2.0
#
# @brief	Read serial input from the rotary encoder

#----- imports
import sys
import serial


#----- globals
SERIAL_PORT = '/dev/ttyACM0'
SERIAL_BAUD = 9600

#----- begin

# create the serial object
s = serial.Serial(SERIAL_PORT, SERIAL_BAUD)

while True:
    try:
        line = s.readline()
        if line:
            string = line.decode().rstrip('\n\r')
            print(string)

    except KeyboardInterrupt:
        break

s.close()
