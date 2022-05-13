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
# @brief	Plugin to create a QRCode for Wifi Setup

#----- imports
import getpass

from definitions import (
    PluginManager, PluginTask
)


#----- classes
@PluginManager.register
class Wifi(PluginTask):
    """Plugin to create a QRCode for Wifi Setup"""

    def __init__(self) -> None:
        """Constructor"""

    def process(self) -> bytes:
        """Ask user for information"""
        ssid = input("SSID: ")
        hidden = input("Is network hidden? (yes/no): ")
        auth = input("Authentication type (WEP/WPA/None): ")

        if auth.lower() != 'none':
            password = getpass.getpass("Enter password: ")
        else:
            password = "nopass"

        if hidden.lower() == "yes":
            hidden == 'True'
        else:
            hidden == "False"

        return f"WIFI:T:{auth};S:{ssid};P:{password};H:{hidden};;".encode('utf-8')
