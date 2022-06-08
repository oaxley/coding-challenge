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
# @brief	Plugin to decode data from a QRCode as a Wifi setup

#----- imports
from definitions import (
    PluginManager, PluginTask
)


#----- classes
@PluginManager.register
class Wifi(PluginTask):
    """Plugin to decode data from a QRCode as a Wifi setup"""

    def __init__(self) -> None:
        """Constructor"""

    def process(self, data: bytes) -> None:
        """Process the data"""
        string = data.decode()

        # remove the wifi header
        string = string[5:]

        for field in string.split(';'):
            if field == '':
                continue

            key, value = field.split(':')

            if key == 'T':
                print(f"Auth Type : {value}")
            elif key == 'S':
                print(f"SSID      : {value}")
            elif key == 'P':
                print(f"Password  : {value}")
            elif key == 'H':
                print(f"Hidden    : {value}")
