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
# @brief	Plugin to create a QRCode from an email address

#----- imports
from definitions import (
    PluginManager, PluginTask
)


#----- classes
@PluginManager.register
class Email(PluginTask):
    """Plugin to create a QRCode from an email address"""

    def __init__(self) -> None:
        """Constructor"""

    def process(self) -> bytes:
        """Ask the user for an Email address"""
        email = input("Please enter your email address: ")
        subject = input("Enter the subject (leave empty for no subject): ")

        data = f"mailto:{email}"

        # add the subject if any
        if len(subject) > 0:
            data = data + f"?subject={subject}"

        return data.encode('utf-8')
