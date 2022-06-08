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
# @brief	Plugin to create a QRCode from a VCard
# @notes    https://www.rfc-editor.org/rfc/rfc6350

#----- imports
from __future__ import annotations
from typing import List, Tuple, Optional

import os
import tempfile
import subprocess

from dataclasses import dataclass, field

from definitions import (
    PluginManager, PluginTask
)


#----- globals
DEFAULT_EDITOR = '/usr/bin/vi'

# the message for the temporary file
file_content = """# This is a temporary file.
# Uncomment and replace only the line you need.

# Title (Mr, Ms, ...)
# Firstname
# Lastname
# Nick name

# Birth date (YYYYMMDD)
# Anniversay (YYYYMMDD)

# Home Address:
# Street
# City
# State or Province
# Postal Code
# Country Name
# Email #1, Email #2, ..., Email #n
# [Type:]Phone number #1, [Type:]Phone number #2, ...

# Work Address
# Organization, Unit, Sub-Unit
# Job Title
# Street
# City
# State or Province
# Postal Code
# Country Name
# Email #1, Email #2, ..., Email #n
# [Type:]Phone number #1, [Type:]Phone number #2, ...

"""


#----- classes
@dataclass
class Person:
    title: str = ""
    firstname: str = ""
    lastname: str = ""
    nickname: Optional[str] = None
    birthday: Optional[str] = None
    anniversary: Optional[str] = None

@dataclass
class Address:
    street: str = ""
    city: str = ""
    state: str = ""
    zipcode: str = ""
    country: str = ""

    def isEmpty(self) -> bool:
        return (
            (self.street == "") and (self.city == "") and (self.state == "") and
            (self.zipcode == "") and (self.country == "")
        )

    def __repr__(self) -> str:
        return f";;{self.street};{self.city};{self.state};{self.zipcode};{self.country}"

@dataclass
class HomeAddress:
    address: Address = Address()
    emails: List[str] = field(default_factory=list)
    phones: List[str] = field(default_factory=list)

@dataclass
class WorkAddress:
    address: Address = Address()
    emails: List[str] = field(default_factory=list)
    phones: List[str] = field(default_factory=list)
    title: Optional[str] = None
    name: Optional[str] = None


@PluginManager.register
class VCard(PluginTask):
    """Plugin to create a QRCode from a VCard"""

    def __init__(self) -> None:
        """Constructor"""
        self.counter = 0
        self.handle  = None

        # information for the VCard
        self.person = Person()
        self.home = HomeAddress()
        self.work = WorkAddress()

        # set the fields
        self.fields = {
            4: (self.person, 'title'),
            5: (self.person, 'firstname'),
            6: (self.person, 'lastname'),
            7: (self.person, 'nickname'),
            9: (self.person, 'birthday'),
            10: (self.person, 'anniversary'),

            13: (self.home.address, 'street'),
            14: (self.home.address, 'city'),
            15: (self.home.address, 'state'),
            16: (self.home.address, 'zipcode'),
            17: (self.home.address, 'country'),

            18: (self.home.emails, ''),
            19: (self.home.phones, ''),

            22: (self.work, 'name'),
            23: (self.work, 'title'),
            24: (self.work.address, 'street'),
            25: (self.work.address, 'city'),
            26: (self.work.address, 'state'),
            27: (self.work.address, 'zipcode'),
            28: (self.work.address, 'country'),

            29: (self.work.emails, ''),
            30: (self.work.phones, ''),
        }

    def _getNextLine(self) -> Tuple[int, str]:
        """Retrieve the next line from the file"""
        line = self.handle.readline().rstrip('\n\r')
        self.counter += 1

        return (self.counter, line)

    def _processInput(self) -> None:
        """Process the user input and create the data structure"""
        # retrieve the number of available input from the file content
        max_lines = len(file_content.split('\n'))

        # go through all the lines
        while True:
            counter, line = self._getNextLine()

            # break the loop once we have processed all the lines
            if counter > max_lines:
                break

            # empty line or commented lines
            if line == "" or line.startswith('#'):
                continue

            if counter in self.fields:
                field = self.fields[counter][0]
                if type(field) is list:
                    for value in line.split(','):
                        field.append(value.strip())
                else:
                    setattr(field, self.fields[counter][1], line)

    def _createPhones(self, phones: List[str], phone_type: str) -> List[str]:
        """Create the data from the phones list"""
        data: List[str] = []
        counter = 1
        for phone in phones:

            text = f"TEL"

            # multiple types have been provided
            if ':' in phone:
                types = phone.split(':')
                number = types[-1]

                for type in types[:-1]:
                    text += f";type={type.upper()}"
            else:
                number = phone

            # default type for phone is "VOICE"
            text += f";type={phone_type};type=VOICE"

            # prefered phone is always the first one
            if counter == 1:
                text += ";type=pref"
                counter += 1

            text += f":{number}"

            data.append(text)

        return data

    def _createEmails(self, emails: List[str], email_type: str) -> List[str]:
        """Create a list of emails"""
        data: List[str] = []
        for email in emails:
            data.append(f"EMAIL;type=INTERNET;type={email_type}:{email}")

        return data

    def _createVCard(self) -> bytes:
        """Create the VCard data from the user input"""

        # VCard header
        data = ["BEGIN:VCARD", "VERSION:4.0", "PRODID:-//oaxley//pyQRCode 1.0//EN"]

        # Person information
        data.append(f"N:{self.person.lastname.upper()};{self.person.firstname.capitalize()};;{self.person.title};")
        data.append(f"FN:{self.person.firstname.capitalize()} {self.person.lastname.upper()}")

        if self.person.nickname:
            data.append(f"NICKNAME:{self.person.nickname}")

        if self.person.birthday:
            data.append(f"BDAY:{self.person.birthday}")

        if self.person.anniversary:
            data.append(f"ANNIVERSARY:{self.person.anniversary}")

        # Home address, emails & phones
        if not self.home.address.isEmpty():
            data.append(f"ADR;type=HOME:{self.home.address}")

        data.extend(self._createEmails(self.home.emails, "HOME"))
        data.extend(self._createPhones(self.home.phones, "HOME"))

        # Work parameters
        if self.work.name:
            if ',' in self.work.name:
                value = ';'.join([ v.strip() for v in self.work.name.split(',')])
            else:
                value = self.work.name
            data.append(f"ORG:{value}")

        if self.work.title:
            data.append(f"TITLE:{self.work.title}")

        if not self.work.address.isEmpty():
            data.append(f"ADR;type=WORK:{self.work.address}")

        data.extend(self._createEmails(self.work.emails, "WORK"))
        data.extend(self._createPhones(self.work.phones, "WORK"))

        # VCard footer
        data.append("END:VCARD")

        # create a long string
        return "\n".join(data).encode('utf-8')


    def process(self) -> bytes:
        """Retrieve information from the user"""

        # retrieve the default editor
        editor = os.environ.get("EDITOR", DEFAULT_EDITOR)

        # create a temporary file
        fd, tmpfile = tempfile.mkstemp(text=True)
        with os.fdopen(fd, "wt") as fh:
            fh.write(file_content)

        # wait for the user to edit the file
        subprocess.call([editor, tmpfile])

        # process the input and remove the temporary file
        self.handle = open(tmpfile, "rt")
        self._processInput()
        os.unlink(tmpfile)

        # create the VCard data
        return self._createVCard()