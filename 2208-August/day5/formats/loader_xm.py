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
# @brief	XM file loader

#----- imports
from __future__ import annotations

import struct

from definitions import (
    IFormat, FormatManager,
    ModInformation, ModSample
)


#----- classes
@FormatManager.register('xm')
class LoaderXM(IFormat):
    """Plugin to load XM tracker music format"""

    def __init__(self) -> None:
        """Constructor"""

    def process(self, filename: str) -> ModInformation:
        """Process the file"""
        obj = ModInformation()
        obj.file_fmt = "XM"

        with open(filename, 'rb') as fh:

            # lookup for the marker
            marker = fh.read(17)
            if not marker.startswith(b"Extended"):
                raise ValueError("Error: invalid XM file!")

            obj.song_name = fh.read(20).decode()

            fh.read(1)      # dos marker

            obj.tracker = fh.read(20).decode()
            fh.read(2)      # version

            header_size = struct.unpack("I", fh.read(4))[0]
            obj.song_length = struct.unpack("h", fh.read(2))[0]

            fh.read(2)      # restart position
            obj.channels = struct.unpack("h", fh.read(2))[0]
            obj.max_pattern = struct.unpack("h", fh.read(2))[0]
            nb_instruments = struct.unpack("h", fh.read(2))[0]

            fh.read(2)      # flags
            fh.read(2)      # default tempo
            fh.read(2)      # default BPM

            # pattern data
            for i in range(256):
                order = struct.unpack("B", fh.read(1))[0]
                obj.tracks.append(order)

            # bypass all the pattern data
            for i in range(obj.max_pattern):
                fh.read(4)      # pattern header size
                fh.read(1)      # packing type
                fh.read(2)      # number of rows

                # get the size of the pattern and skip it
                length = struct.unpack("h", fh.read(2))[0]

                pos = fh.tell()
                pos = pos + length
                fh.seek(pos, 0)

            # instrument data
            for i in range(nb_instruments):
                sample = ModSample()

                # get the current file position & instr hdr size
                instr_begin = fh.tell()
                instr_size = struct.unpack("I", fh.read(4))[0]

                # sample name
                try:
                    sample.name = fh.read(22).decode().replace('\x00', ' ')
                except:
                    position = fh.tell()
                    print(f"Error: position in file {position - 22:x}")
                    raise

                fh.read(1)      # instrument type

                # number of samples
                value = struct.unpack("h", fh.read(2))[0]

                # no sample in this instrument.
                # remove header size, and sample length and switch to next instrument
                if value == 0:
                    fh.read(2)
                    fh.read(2)
                    continue

                # header size
                hdr_size = struct.unpack("I", fh.read(4))[0]

                # move to the beginning of the sample header
                hdr_pos = instr_begin + instr_size
                fh.seek(hdr_pos, 0)

                sample.length = struct.unpack("I", fh.read(4))[0]
                sample.loop_start = struct.unpack("I", fh.read(4))[0]
                sample.loop_length = struct.unpack("I", fh.read(4))[0]
                sample.volume = struct.unpack("B", fh.read(1))[0]
                sample.fine_tune = struct.unpack("b", fh.read(1))[0]

                # bypass all the remaining data
                pos = instr_begin + instr_size + hdr_size + sample.length
                fh.seek(pos, 0)

                # add the sample to the list
                obj.instruments.append(sample)



        return obj