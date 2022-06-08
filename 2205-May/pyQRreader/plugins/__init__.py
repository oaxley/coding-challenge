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
# @brief	Plugins importer

#----- imports
from importlib import (
    import_module, resources
)


#----- begin
# import all the plugins in this directory
for name in resources.contents(__name__):

    # bypass special files
    if name.startswith("__"):
        continue

    # import other python files
    if name.endswith(".py"):
        import_module(f"{__name__}.{name[:-3]}")
