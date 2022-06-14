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
# @brief	Receive DATAREF values from X-Plane

#----- imports
import sys
import socket


#----- globals

# Port where X-Plane is sending data
UDP_PORT = 35880


#----- begin

# create the UDP socket and bind it to the port
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(('', UDP_PORT))

old_data = b""
while True:
    data, address = sock.recvfrom(1024)

    if (data != old_data):
        print(f"Received {len(data)} bytes from {address}: {data.decode()}", file=sys.stderr)
        old_data = data

