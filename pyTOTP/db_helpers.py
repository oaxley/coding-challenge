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
# @brief	Database helper functions

#----- imports
from __future__ import annotations
from typing import Optional

import os
import sys
import base64
import logging

from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker

from database import Base, User


#----- globals
SECRET_KEY_LENGTH = 20      # length for SHA1 hashing function


#----- functions
def dbConnection():
    """Create the SQLite connection"""
    # database name can be setup via environment variable
    db_name = os.environ.get('TOTP_DATABASE_NAME', 'pyTOTP.sqlite')

    # SQLAlchemy engine
    engine = create_engine(f'sqlite:///{db_name}')

    # create the tables if not done already
    Base.metadata.create_all(engine)

    # return a new session to the caller
    Session = sessionmaker(bind=engine)
    return Session()


def _loadUser(account: str, issuer: str) -> Optional[User]:
    """load the user details from the database"""
    # retrieve the database session
    session = dbConnection()

    # verify if the account / issuer already exist in the database
    return (session
                .query(User)
                .filter(User.account == account)
                .filter(User.issuer == issuer)
                .first()
    )


def createUser(account: str, issuer: str) -> None:
    """Create a new user in the database"""
    # create a new object
    secret_key = base64.b64encode(os.urandom(SECRET_KEY_LENGTH))
    user = User(account=account, issuer=issuer, secret_key=secret_key)

    # add it to the db
    session = dbConnection()
    session.add(user)
    session.commit()


def retrieveUser(account: str, issuer: str) -> User:
    """Retrieve a user details from the database"""
    result = _loadUser(account, issuer)
    if result is None:
        logging.error(f"Sorry, there is no entry for {account}/{issuer} in the database.")
        sys.exit(1)

    return result

def isExistUser(account: str, issuer: str) -> bool:
    """Check if a user is present in the database"""
    result = _loadUser(account, issuer)
    if result:
        return True
    else:
        return False


def getRFC6238(account: str, issuer: str) -> str:
    """Generate the RFC 6238 string"""
    # retrieve the user from the database
    user = retrieveUser(account, issuer)

    # encode the secret_key as a Base32
    secret_key = base64.b64decode(user.secret_key)
    secret_key_b32 = base64.b32encode(secret_key)

    # display the value to the user
    return secret_key_b32.decode()

