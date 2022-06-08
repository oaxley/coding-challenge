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
# @brief	Define the Plugins Abstract Class

#----- imports
from __future__ import annotations
from typing import Any

from abc import abstractmethod, ABC


#----- classes
class PluginTask(ABC):
    """Plugin Abstract class"""

    def __init__(self) -> None:
        """Constructor"""

    @abstractmethod
    def process(self) -> bytes:
        """Process the input from the user"""
