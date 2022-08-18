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
# @brief	Class to aggregate all the MOD information

#----- imports
from __future__ import annotations
from typing import List

from dataclasses import dataclass


#----- classes
@dataclass
class ModSample:
    """Single MOD sample information"""
    name: str = ""
    length: int = 0
    fine_tune: int = 0
    volume: int = 0
    loop_start: int = 0
    loop_length: int = 0


class ModInformation:
    """Aggregate all the MOD information in one place"""

    def __init__(self) -> None:
        """Constructor"""
        self.file_fmt: str = ""
        self.song_name: str = ""
        self.channels: int = 0
        self.instruments: List[ModSample] = []
        self.song_length: int = 0
        self.tempo = 125
        self.speed = 6
        self.tracks: List[int] = []
        self.max_pattern: int = 0
        self.tracker: str = ""

    def estimate(self,) -> str:
        rows = self.song_length * 64

        # Speed is 6 and BPM=125 (50Hz)
        total_secs = rows * self.speed * 1 / (2 * self.tempo / 5)

        minutes = int(total_secs // 60)
        seconds = int(total_secs % 60)

        return f"00:{minutes:02d}:{seconds:02d}"

    def display(self) -> None:
        """Display the information"""
        print("====== Music MOD Information ======")
        print(f"Format      : {self.file_fmt}")
        print(f"Tracker     : {self.tracker}")
        print(f"Song Name   : {self.song_name}")
        print(f"Song Length : {self.song_length} tracks (estimated {self.estimate()})")
        print(f"Speed / BPM : {self.speed} / {self.tempo}")
        print(f"Channels    : {self.channels}")
        print(f"Max Pattern : {self.max_pattern}")
        print(f"Track List  : {self.tracks[:self.song_length]}")

        print(f"Instruments :")
        print(f"    | {' ':^30} | {'Length':^10} | {' ':^8} | {' ':^8} | {'Loop':^7} | {'Loop':^7} |")
        print(f" ID | {'Name':^30} | {'(bytes)':^10} | {'Finetune':^8} | {'Volume':^8} | {'Start':^7} | {'Length':^7} |")
        print(f"----+{'-' * 32}+{'-' * 12}+{'-' * 10}+{'-' * 10}+{'-' * 9}+{'-' * 9}+")
        for i, sample in enumerate(self.instruments):
            # don't output empty instruments
            if sample.length == 0:
                continue

            print(f" {i:02d} | {sample.name:<30} | {sample.length:>10} | {sample.fine_tune:>8} | {sample.volume:>8} | {sample.loop_start:>7} | {sample.loop_length:>7} |")

        print("")