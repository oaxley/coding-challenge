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
import click
import logging

from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker


from .database import Base, User


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


@click.group()
def main():
    """Main entry point"""


@main.command
@click.argument('account')
@click.argument('issuer')
def create(account: str, issuer: str):
    """Create a new entry in the database"""

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
    main()