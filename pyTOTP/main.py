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
import qrcode as QRCode

from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives.twofactor import totp, InvalidToken
from cryptography.hazmat.primitives.hashes import SHA1

from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker


from database import Base, User

#----- globals
SECRET_KEY_LENGTH = 20      # length for SHA1 hashing function
PASSWORD_SIZE = 6           # we keep only 6 digits
PASSWORD_TIME = 30          # password is valid only for 30s


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

def createTOTP(user: User) -> totp.TOTP:
    """Create the TOTP object"""
    return totp.TOTP(
                base64.b64decode(user.secret_key),
                PASSWORD_SIZE,
                SHA1(),
                PASSWORD_TIME,
                backend=default_backend()
    )


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
    # verify that the account exists
    result = retrieveUser(account, issuer)
    if result is None:
        logging.error(f"Sorry, there is no entry for {account}/{issuer} in the database.")
        sys.exit(1)
    else:
        user: User = result

    # create the totp object and generate the corresponding URI
    data = createTOTP(user)
    uri = data.get_provisioning_uri(account, issuer)

    # generate the QRCode
    qr = QRCode.QRCode(
        version=1,
        error_correction=QRCode.constants.ERROR_CORRECT_L,
        box_size=5,
        border=4,
    )
    qr.add_data(uri)
    qr.make()

    # create the image with Pillow and display it
    img = qr.make_image(fill_color="black", back_color="white")
    img.show()


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