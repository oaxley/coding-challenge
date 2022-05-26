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
# @brief	Plugin to decode data from a QRCode as an Email

#----- imports
from definitions import (
    PluginManager, PluginTask
)


#----- classes
@PluginManager.register
class Email(PluginTask):
    """Plugin to decode data from a QRCode as an Email"""

    def __init__(self) -> None:
        """Constructor"""

    def process(self, data: bytes) -> None:
        """Process the data"""
        string = data.decode()

        if '?' in string:
            email, subject = string.split('?')
        else:
            email = string
            subject = None

        # remove extra characters
        email = email.split(':')[1]
        print(f'Email   : {email}')

        if subject:
            subject = subject.split('=')[1]
            print(f'Subject : {subject}')


