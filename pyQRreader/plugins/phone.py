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
# @brief	Plugin to decode data from a QRCode as a Phone number

#----- imports
from definitions import (
    PluginManager, PluginTask
)

import phonenumbers as pn

#----- classes
@PluginManager.register
class Phone(PluginTask):
    """Plugin to decode data from a QRCode as a Phone number"""

    def __init__(self) -> None:
        """Constructor"""

    def process(self, data: bytes) -> None:
        """Process the data"""
        string = data.decode().split(':')[1]

        # format correctly the phone number
        phone = pn.parse(string)
        value = pn.format_number(phone, pn.PhoneNumberFormat.INTERNATIONAL)
        print(f"Phone Number: {value}")

