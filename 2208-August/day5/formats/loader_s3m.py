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
# @brief	S3M file loader

#----- imports
from __future__ import annotations
from typing import Tuple

import struct

from definitions import (
    IFormat, FormatManager,
    ModInformation, ModSample
)


#----- classes
@FormatManager.register('s3m')
class LoaderS3M(IFormat):
    """Plugin to load S3M tracker music format"""

    def __init__(self) -> None:
        """Constructor"""

    def process(self, filename: str) -> ModInformation:
        """Process the file"""
        obj = ModInformation()
        obj.file_fmt = "S3M"

        with open(filename, 'rb') as fh:

            # lookup for the marker
            fh.seek(44, 0)
            value = fh.read(4).decode()
            if value != 'SCRM':
                raise ValueError("Error: invalid S3M file!")

            # read song title
            fh.seek(0, 0)
            obj.song_name = fh.read(28).decode()

            fh.read(4) # DOS marker + filetype + reserved

            obj.song_length = struct.unpack("H", fh.read(2))[0]

            nb_instrument = struct.unpack("H", fh.read(2))[0]
            nb_pattern = struct.unpack("H", fh.read(2))[0]

            fh.read(2)  # flags
            fh.read(2)  # cwt/v
            fh.read(2)  # sample format
            fh.read(4)  # SCRM
            fh.read(1)  # global volume
            fh.read(1)  # initial speed
            fh.read(1)  # initial tempo
            fh.read(1)  # master volume
            fh.read(1)  # ultraclick
            fh.read(1)  # default panning
            fh.read(8)  # expansion
            fh.read(2)  # special

            obj.channels = 0
            for i in range(32):
                value = struct.unpack("B", fh.read(1))[0]
                if value < 16:
                    obj.channels += 1

            count = 0
            obj.max_pattern = 0
            for i in range(obj.song_length):
                order = struct.unpack("B", fh.read(1))[0]
                if (order < 254):
                    obj.tracks.append(order)
                    count = count + 1
                    if order > obj.max_pattern:
                        obj.max_pattern = order

            # correct song length
            obj.song_length = count

            # parapointers
            for i in range(nb_instrument):
                pointer = struct.unpack("H", fh.read(2))[0]

                # save the current file pointer
                save = fh.tell()

                # # move to the instrument details
                fh.seek(pointer * 16 - 1, 0)

                sample = ModSample()

                fh.read(1)  # type
                fh.read(13) # DOS name
                fh.read(3)  # memseg

                sample.length = struct.unpack("H", fh.read(2))[0]
                fh.read(2)  # unused

                sample.loop_start = struct.unpack("H", fh.read(2))[0]
                fh.read(2)  # high part - unused

                value = struct.unpack("H", fh.read(2))[0]
                sample.loop_length = value - sample.loop_start

                fh.read(2)  # high part - unused

                sample.volume = struct.unpack("B", fh.read(1))[0]

                fh.read(1)  # reserved
                fh.read(1)  # packing scheme
                fh.read(1)  # flags (bit 1 = loop)
                fh.read(2)  # middle C rate
                fh.read(2)  # unused
                fh.read(12) # unused

                sample.name = fh.read(28).decode().replace('\x00', ' ')
                fh.read(4)  # unused

                # # add sample to mod information
                obj.instruments.append(sample)

                # move back the pointer where we left of
                fh.seek(save, 0)


        return obj