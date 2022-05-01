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
# @brief	SQLalchemy tables definition

#----- imports
from sqlalchemy import Column, String
from sqlalchemy.ext.declarative import declarative_base


#----- globals
Base = declarative_base()


#----- classes
class User(Base):
    __tablename__ = "user"
    account = Column(String, primary_key=True)
    issuer = Column(String, primary_key=True)
    secret_key = Column(String)

