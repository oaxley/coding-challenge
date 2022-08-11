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
# @brief	Plugins Manager

#----- imports
from __future__ import annotations
from typing import Dict, List, ClassVar, Optional

from .plugins import IFormat


#----- classes
class FormatManager:
    """FormatManager
       This class acts as a Singleton to ensure only one instance exists to manage plugins
    """

    __instance: ClassVar[Optional[FormatManager]] = None

    def __new__(cls: type[FormatManager]) -> FormatManager:
        """Create a new instance of the class

        Returns:
            A new instance or the current one if it has been already created
        """
        # create a new instance
        if cls.__instance is None:
            cls.__instance = object.__new__(cls)
            cls.__instance._setup()

        return cls.__instance

    def _setup(self) -> None:
        """Setup the instance after its creation"""
        # list of available plugins
        self._plugins: Dict[str, IFormat] = {}

    def availablePlugins(self) -> List[str]:
        """List of available plugins

        Returns:
            a list of all the plugins name
        """
        return list(self._plugins.keys())

    def factory(self, name: str) -> Optional[IFormat]:
        """Create a new instance of a registered plugin

        Returns:
            the associated plugins, None otherwise
        """
        if name in self._plugins:
            return self._plugins[name]()
        else:
            return None

    @staticmethod
    def register(cls):
        """Decorator to register a new plugin"""
        # retrieve the format manager instance
        instance = FormatManager()

        # register the plugin
        if cls.__name__ not in instance._plugins:
            instance._plugins[cls.__name__] = cls

        # don't modify the decorated class
        return cls
