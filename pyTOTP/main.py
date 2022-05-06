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

import sys
import click
import logging

from db_helpers import (
    createUser, isExistUser, getRFC6238
)

from totp_helpers import (
    createQRCode, showTOTP, validateTOTP
)


#----- functions

@click.group()
def main():
    """Main entry point"""


@main.command
@click.argument('account')
@click.argument('issuer')
def create(account: str, issuer: str):
    """Create a new entry in the database"""

    if isExistUser(account, issuer):
        logging.error(f"Sorry, an entry for {account}/{issuer} already exists in the database.")
        sys.exit(1)
    else:
        createUser(account, issuer)


@main.command()
@click.argument('account')
@click.argument('issuer')
def qrcode(account: str, issuer: str):
    """Generate the QRCode for Google Authenticator"""
    createQRCode(account, issuer)


@main.command()
@click.argument('account')
@click.argument('issuer')
def rfc6238(account: str, issuer: str):
    """Generate the RFC6238 string for KeyPassXC"""
    click.echo(f"RFC 6238 value: {getRFC6238(account, issuer)}")


@main.command()
@click.argument('account')
@click.argument('issuer')
def show(account: str, issuer: str):
    """Show the current value for the TOTP"""
    click.echo("Press CTRL+C to stop")
    showTOTP(account, issuer)


@main.command()
@click.argument('account')
@click.argument('issuer')
@click.argument('value')
def check(account: str, issuer: str, value: str):
    """Validate a value against the current TOTP parameters"""
    if not validateTOTP(account, issuer, value):
        sys.exit(1)


#----- begin
if __name__ == "__main__":
    logging.basicConfig(level=logging.INFO)
    main()