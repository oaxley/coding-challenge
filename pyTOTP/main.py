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
# @brief	TOTP management in the command line

#----- imports
from __future__ import annotations
from typing import Optional

import os
import sys
import click
import base64
import logging

from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker


from database import Base, User

#----- globals
SECRET_KEY_LENGTH = 20


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

def retrieveUser(account: str, issuer: str) -> Optional[User]:
    """Retrieve the user details from the database"""
    # retrieve the database session
    session = dbConnection()

    # verify if the account / issuer already exist in the database
    return (session
                .query(User)
                .filter(User.account == account)
                .filter(User.issuer == issuer)
                .first()
    )

def getSecretKey() -> bytes:
    """Create a 160bits (20 x 8) secret key"""
    return os.urandom(SECRET_KEY_LENGTH)


@click.group()
def main():
    """Main entry point"""


@main.command
@click.argument('account')
@click.argument('issuer')
def create(account: str, issuer: str):
    """Create a new entry in the database"""
    # verify if the account exist already
    result = retrieveUser(account, issuer)
    if result:
        logging.error(f"Sorry, an entry for {account}/{issuer} already exists in the database.")
        sys.exit(1)

    # create a new secret key for this user
    secret_key = getSecretKey()

    # create the user
    user = User(account=account, issuer=issuer, secret_key=base64.b64encode(secret_key))

    # add it to the db
    session = dbConnection()
    session.add(user)
    session.commit()


@main.command()
@click.argument('account')
@click.argument('issuer')
def qrcode(account: str, issuer: str):
    """Generate the QRCode for Google Authenticator"""

@main.command()
@click.argument('account')
@click.argument('issuer')
def rfc6238(account: str, issuer: str):
    """Generate the RFC6238 string for KeyPassXC"""

@main.command()
@click.argument('account')
@click.argument('issuer')
def show(account: str, issuer: str):
    """Show the current value for the TOTP"""

@main.command()
@click.argument('account')
@click.argument('issuer')
@click.argument('value')
def check(account: str, issuer: str, value: str):
    """Validate a value against the current TOTP parameters"""


#----- begin
if __name__ == "__main__":
    logging.basicConfig(level=logging.INFO)
    main()