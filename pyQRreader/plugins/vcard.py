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
# @brief	Plugin to decode data from a QRCode as a VCard

#----- imports
from __future__ import annotations
from typing import Dict

from definitions import (
    PluginManager, PluginTask
)


#----- classes
@PluginManager.register
class VCard(PluginTask):
    """Plugin to decode data from a QRCode as a VCard"""

    def __init__(self) -> None:
        """Constructor"""

        self.output: Dict[str, str] = {
            'N':  'Name\t\t: %(value)s',
            'FN': 'Formatted Name  : %(value)s',
            'NICKNAME': 'Nickname\t: %(value)s',
            'BDAY': 'Birthday\t: %(value)s',
            'ANNIVERSARY': 'Anniversary\t: %(value)s',
            'ADR': 'Address%(extra)s\t: %(value)s',
            'EMAIL': 'Email%(extra)s\t: %(value)s',
            'TEL': 'Phone%(extra)s\t: %(value)s',
            'ORG': 'Organization\t: %(value)s',
            'TITLE': 'Job Title\t: %(value)s',
        }

    def process(self, data: bytes) -> None:
        """Process the data"""
        string = data.decode()

        for field in string.split('\n'):
            key, value = field.split(':')

            if key in [ 'BEGIN', 'VERSION', 'PRODID', 'END' ]:
                continue

            if ';' in key:
                key, *extra = key.split(';')
                extra = [ field.split('=')[1] for field in extra ]

                # remove un-wanted items
                for v in [ 'INTERNET', 'VOICE', 'CELL', 'pref' ]:
                    try:
                        extra.remove(v)
                    except ValueError:
                        pass

            else:
                extra = []

            if key in self.output:
                print(self.output[key] % {'value': value.replace(';',' ').strip(), 'extra': extra})
            else:
                print(f'{key} => {value} {extra}')