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
    name: str
    length: int
    fine_tune: int
    volume: int
    loop_start: int
    loop_length: int


class ModInformation:
    """Aggregate all the MOD information in one place"""

    def __init__(self) -> None:
        """Constructor"""
        self.file_fmt: str = ""
        self.song_name: str = ""
        self.channels: int = 0
        self.instruments: List[ModSample] = []
        self.song_length: int = 0
        self.order: List[int] = []
        self.pattern: int = 0

    def display(self) -> None:
        """Display the information"""

