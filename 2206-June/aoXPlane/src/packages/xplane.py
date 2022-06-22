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
# @brief	Module to communicate with X-Plane

#----- imports
from __future__ import annotations
from typing import Any, Union, Optional

import socket
import struct


#----- globals
UDP_SOCK_TIMEOUT = 2.0
UDP_SOCK_MAX_TTL = 1

UDP_DEFAULT_PORT = 49000


#----- classes
class XPlaneCOM:
    """Class to communicate with X-Plane COM module"""

    def __init__(self, udp_host: str = '', udp_port: int = UDP_DEFAULT_PORT) -> None:
        """Constructor"""
        self.udp_host = udp_host
        self.udp_port = udp_port


    def connect(self) -> None:
        """Create the UDP connection to X-Plane"""
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.sock.settimeout(UDP_SOCK_TIMEOUT)
        self.sock.setsockopt(socket.IPPROTO_IP, socket.IP_MULTICAST_TTL, struct.pack('b', UDP_SOCK_MAX_TTL))


    def setDREF(self, name: str, value: Union[int, float]) -> None:
        """Set a DREF value in X-Plane"""
        # prepare the data
        command = b'DREF\x00'
        dataref = name.encode() + b'\x00'
        dataref = dataref.ljust(500)

        # pack the data according to value type
        if isinstance(value, int):
            message = struct.pack("<5si500s", command, value, dataref)
        elif isinstance(value, float):
            message = struct.pack("<5sf500s", command, value, dataref)
        else:
            message = b""

        # send the data to X-Plane
        self.sock.sendto(message, (self.udp_host, self.udp_port))


    def getDREF(self, name: str) -> Optional[float]:
        """Retrieve a DREF value from X-Plane"""
        # prepare the data
        command = b'RREF\x00'
        dataref = name.encode() + b'\x00'
        dataref = dataref.ljust(400)
        index = 0
        frequency = 1

        # pack the data
        message = struct.pack("<5sii400s", command, frequency, index, dataref)

        # step 1: subscribe to the dataref from X-Plane
        self.sock.sendto(message, (self.udp_host, self.udp_port))

        # step 2: wait for the answer from X-Plane
        data, _ = self.sock.recvfrom(1024)

        # step 3: stop the subscription
        frequency = 0
        message = struct.pack("<5sii400s", command, frequency, index, dataref)
        self.sock.sendto(message, (self.udp_host, self.udp_port))

        # decode the data
        _, _, value = struct.unpack("<5sif", data)
        
        return value
        