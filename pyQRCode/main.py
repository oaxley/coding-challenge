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


#----- functions


#----- begin

# argument parser
parser = argparse.ArgumentParser(description="QRCode generator")
parser.add_argument("--image", required=True, help="Output image filename")
parser.add_argument("--type", required=True, help="QRCode type")

args = parser.parse_args()

