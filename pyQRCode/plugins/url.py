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
# @brief	Plugin to create a QRCode from a URL

#----- imports
from definitions import (
    PluginManager, PluginTask
)


#----- classes
@PluginManager.register
class URL(PluginTask):
    """Plugin to create a QRCode from a URL"""

    def __init__(self) -> None:
        """Constructor"""

    def process(self) -> bytes:
        """Ask the User for a URL"""
        data = input("Please enter your URL: ")
        return data.encode("utf-8")

