#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# vim: filetype=python
#
# This source file is subject to the Apache License 2.0
# that is bundled with this package in the file LICENSE.txt.
# It is also available through the Internet at this address:
# https://opensource.org/licenses/Apache-2.0
#
# @author	Sebastien LEGRAND
# @license	Apache License 2.0
#
# @brief	Display MOD information

#----- imports
import sys
import pathlib
import argparse

from definitions import (
    FormatManager, IFormat,
    ModInformation
)

# register all the plugins
import formats


#----- begin

# read command line arguments
parser = argparse.ArgumentParser()
parser.add_argument("--file", metavar="filename", help="show information on MOD file")
parser.add_argument("--list", action="store_true", help="list of available plugins")
args = parser.parse_args()

# show help if there is no arguments
if args is None:
    parser.print_help()
    sys.exit(0)

# print the available loader
if args.list:
    for plugin, fmt, desc in FormatManager().availablePlugins():
        print(f"{plugin}: {desc} [.{fmt}]")
    sys.exit(0)

if args.file is None:
    print("Error: no filename specified.")
    sys.exit(1)

# ensure the file exists first
filename = pathlib.PosixPath(args.file)
if not filename.exists():
    print(f"Error: could not find file [{args.file}].")
    sys.exit(1)

# find the proper plugin
name = FormatManager().lookup(filename.suffix[1:])
if name == "":
    print(f"Error: unable to find proper plugin to load this file.")
    sys.exit(1)

# create an instance of the plugin and parse the file
try:
    plugin: IFormat = FormatManager().factory(name)
    output: ModInformation = plugin.process(filename)
    output.display()
except ValueError as e:
    print(e)
    sys.exit(1)
