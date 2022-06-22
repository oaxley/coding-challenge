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
# @brief	Module to communicate with the Arduino

#----- imports
from __future__ import annotations
from typing import Any, Optional

import serial


#----- globals
DEFAULT_SERIAL_PORT = '/dev/ttyACM0'
DEFAULT_SERIAL_BAUD = 9600

#----- functions
#----- classes
class SerialCOM:
    """Class to communicate with the Arduino over Serial port"""

    def __init__(self, port: str = DEFAULT_SERIAL_PORT, baud: int = DEFAULT_SERIAL_BAUD) -> None:
        """Constructor"""
        self.port = port
        self.baud = baud

    def connect(self) -> None:
        """Connect to the serial port"""
        self.serial = serial.Serial(self.port, self.baud)

    def sendData(self, data: str) -> None:
        """Send data to serial port"""
        self.serial.write(data.encode())

    def recvData(self) -> Optional[str]:
        """Recv data from the serial port"""
        data = self.serial.readline()
        if not data:
            return None
        
        data = data.decode().rstrip('\n\r')
        return data
