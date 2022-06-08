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
# @brief	Generate a QRCode for a phone number

#----- imports
from definitions import (
    PluginManager, PluginTask
)


#----- classes
@PluginManager.register
class Phone(PluginTask):
    """Plugin to create a QRCode from a Phone number"""

    def __init__(self) -> None:
        """Constructor"""

    def process(self) -> bytes:
        """Ask the user for a phone number"""
        data = input("Please enter your phone number: ")
        data = "tel:" + data
        return data.encode('utf-8')

