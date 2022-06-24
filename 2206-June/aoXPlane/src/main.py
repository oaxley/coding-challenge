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
# @brief	Main entry point for X-Plane/Arduino experiment

#----- imports
from __future__ import annotations

import sys

from packages import XPlaneCOM, SerialCOM


#----- globals
XPLANE_DATAREF = "sim/cockpit/radios/com1_freq_hz"
MIN_DREF_VALUE = 11800
MAX_DREF_VALUE = 13700


#----- classes
class XCOM:
    """class to glue the Arduino and X-Plane"""

    def __init__(self) -> None:
        """Constructor"""
        self.xplane = XPlaneCOM()
        self.serial = SerialCOM()

    def connect(self) -> None:
        """Connect to both X-Plane and Serial link"""
        self.xplane.connect()

        try:
            self.serial.connect()
        except:
            print("Error: Unable to connect to Serial port")
            sys.exit(1)

    def startup(self) -> None:
        """Synchronize the data between X-Plane / Arduino"""
        # read the dataref from X-Plane
        value = self.xplane.getDREF(XPLANE_DATAREF)
        value = int(value)
        
        # ensure we are between boundaries
        if (MIN_DREF_VALUE < value < MAX_DREF_VALUE):
            pass
        else:
            print(f"Error: DREF value outside boundaries [{MIN_DREF_VALUE}, {MAX_DREF_VALUE}]: {int(value)}")
            sys.exit(1)

        # send the value to serial
        string = self.serial.recvData()
        if string is None:
            print("Error: unable to receive data from Serial link")
            sys.exit(1)
        else:
            # initialize the Arduino value
            if string.startswith("INIT_VALUE"):
                self.serial.sendData(f"{value}")
            else:
                print(f"Error: unexpected command '{string}'")
                sys.exit(1)

    def run(self):
        """Main loop"""
        while True:
            try:
                # read the data from the Arduino
                data = self.serial.recvData()
                if data is None:
                    continue

                # ensure it's a data command
                if data.startswith("DATA:"):
                    _, value = data.split(':')
                    self.xplane.setDREF(XPLANE_DATAREF, float(value))

            except KeyboardInterrupt:
                break


#----- begin
# instantiate a new object
xcom = XCOM()

# connect to X-Plane + Arduino
print("Connecting to X-Plane & Arduino ...")
xcom.connect()

# data synchronization
print("Synchronizing X-Plane & Arduino ...")
xcom.startup()

# mainloop
print("Running (CTRL+C to stop) ...")
xcom.run()

# end
print("Done.")
