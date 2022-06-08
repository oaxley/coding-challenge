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
# @brief	Main entry point for pyQRreader

#----- imports
from __future__ import annotations
from typing import Any, Optional

import sys
import pathlib
import argparse

from pyzbar.pyzbar import decode
from PIL import Image

from definitions import PluginManager, PluginTask

# register all the plugins
import plugins


#----- functions
def detectType(data: bytes) -> str:
    """Detect the proper type of the QRCode"""

    if b'VCARD' in data:
        return 'VCard'

    elif b'tel:' in data:
        return 'Phone'

    elif b'mailto' in data:
        return 'Email'

    elif b'WIFI' in data:
        return 'Wifi'

    elif b'http' in data:
        return 'Url'

def decodeQRCode(filename: str) -> Optional[bytes]:
    """Decode a QRCode from an image"""
    data = decode(Image.open(filename))

    # no QRCode found
    if len(data) == 0:
        return None
    else:
        return data[0].data


#----- begin
# plugin manager
manager = PluginManager()

# argument parser
parser = argparse.ArgumentParser(description="QRCode reader")
parser.add_argument("--image", required=True, help="Input image filename")

args = parser.parse_args()

# check if the file exists
if not pathlib.Path(args.image).exists():
    print(f"Error: could not find image [{args.image}]")
    sys.exit(1)

# try to decode QRcode from this image
data = decodeQRCode(args.image)
if data is None:
    print("Error: no QRCode has been found in the image.")
    sys.exit(1)

# retrieve the type of the QRCode
qr_type = detectType(data)
print(f"*** QRCode Type is {qr_type}")

# instantiate the proper plugin
plugin: PluginTask = manager.factory(qr_type)
plugin.process(data)