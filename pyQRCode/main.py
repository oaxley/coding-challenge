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
import qrcode

from definitions import PluginManager, PluginTask


#----- functions
def createQRCode(data: bytes, filename: str) -> None:
    """Generate a QRCode with the data provided"""
    # new QRCode instance
    qr = qrcode.QRCode(
        version=None,
        error_correction=qrcode.ERROR_CORRECT_L,
        box_size=10,
        border=4
    )

    # add some data to it
    qr.add_data(data)

    # create the image
    qr.make(fit=True)
    img = qr.make_image(fill_color='black', back_color='white')

    # save the image
    img.save(filename)
    print(f"QRCode saved to {filename}.")


#----- begin
# plugin manager
manager = PluginManager()

# argument parser
parser = argparse.ArgumentParser(description="QRCode generator")
parser.add_argument("--image", required=True, help="Output image filename")
parser.add_argument("--type", required=True, choices=manager.availablePlugins() ,help="QRCode type")

args = parser.parse_args()

# execute the required plugin
plugin: PluginTask = manager.factory(args.type)
data = plugin.process()

# create the QRCode from the data
createQRCode(data, args.image)
