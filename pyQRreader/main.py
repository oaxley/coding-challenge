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
# @brief	Main entry point for pyQRCode

#----- imports
from __future__ import annotations
from typing import Any

import argparse

from pyzbar.pyzbar import decode
from PIL import Image

#----- globals


#----- functions
def detectType(data: bytes) -> str:
    """Detect the proper type of the QRCode"""

    if b'VCARD' in data:
        return 'VCARD'

    elif b'tel:' in data:
        return 'PHONE'

    elif b'mailto' in data:
        return 'EMAIL'

    elif b'WIFI' in data:
        return 'WIFI'

    elif b'http' in data:
        return 'URL'


#----- begin

# argument parser
parser = argparse.ArgumentParser(description="QRCode reader")
parser.add_argument("--image", required=True, help="Input image filename")

args = parser.parse_args()

