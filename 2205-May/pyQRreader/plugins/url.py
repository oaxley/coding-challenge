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
# @brief	Plugin to decode data from a QRCode as an URL

#----- imports
from definitions import (
    PluginManager, PluginTask
)


#----- classes
@PluginManager.register
class Url(PluginTask):
    """Plugin to decode data from a QRCode as an URL"""

    def __init__(self) -> None:
        """Constructor"""

    def process(self, data: bytes) -> None:
        """Process the data"""
        string = data.decode()
        print(f"URL : {string}")