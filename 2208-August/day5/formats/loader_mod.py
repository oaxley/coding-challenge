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
# @brief	MOD file loader

#----- imports
from __future__ import annotations
from typing import Any, Tuple

import struct

from definitions import (
    IFormat, FormatManager,
    ModInformation, ModSample
)


#----- classes
@FormatManager.register('mod')
class LoaderMod(IFormat):
    """Plugin to load MOD tracker music format"""

    def __init__(self) -> None:
        """Constructor"""

    def _unpack(self, value: Tuple[bytes]) -> int:
        """Unpack a Big Endian tuple to its counterpart integer"""
        return (value[0] * 0x100 + value[1]) * 2

    def process(self, filename: str) -> ModInformation:
        """Process the file"""
        obj = ModInformation()
        obj.file_fmt = "MOD"

        with open(filename, "rb") as fh:

            # lookup for the marker & channels number
            fh.seek(1080, 0)
            value = fh.read(4).decode()
            if (value == "M.K.") or (value == "4CHN"):
                obj.channels = 4
            elif value == "6CHN":
                obj.channels = 6
            elif value == "8CHN":
                obj.channels = 8
            else:
                raise ValueError("Error: unable to determine the number of channels!")

            # module name
            fh.seek(0, 0)
            obj.song_name = fh.read(20).decode()

            # read samples information
            for i in range(31):
                sample = ModSample(
                    name = fh.read(22).decode().replace('\x00', ' '),
                    length = self._unpack(struct.unpack("BB", fh.read(2))),
                    fine_tune = struct.unpack("B", fh.read(1))[0],
                    volume = struct.unpack("B", fh.read(1))[0],
                    loop_start = self._unpack(struct.unpack("BB", fh.read(2))),
                    loop_length = self._unpack(struct.unpack("BB", fh.read(2)))
                )

                # adjust the finetune
                if sample.fine_tune > 7:
                    sample.fine_tune -= 16

                # add the sample to the mod information
                obj.instruments.append(sample)

            # song length + unused
            obj.song_length = struct.unpack("B", fh.read(1))[0]
            fh.read(1)

            # read tracks pattern
            for i in range(128):
                value = struct.unpack("B", fh.read(1))[0]
                if value > obj.max_pattern:
                    obj.max_pattern = value
                obj.tracks.append(value)

        # return the Mod Information object
        return obj

