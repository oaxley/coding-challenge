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
# @brief	TOTP helper functions

#----- imports
from __future__ import annotations

import time
import base64
import qrcode

from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives.twofactor import totp, InvalidToken
from cryptography.hazmat.primitives.hashes import SHA1

from .db_helpers import retrieveUser


#----- globals
PASSWORD_SIZE = 6           # we keep only 6 digits
PASSWORD_TIME = 30          # password is valid only for 30s


#----- functions

def _createTOTP(secret_key: str) -> totp.TOTP:
    """Create the TOTP object"""
    return totp.TOTP(
                base64.b64decode(secret_key),
                PASSWORD_SIZE,
                SHA1(),
                PASSWORD_TIME,
                backend=default_backend()
    )


def createQRCode(account: str, issuer: str) -> None:
    """Generate the corresponding qrcode"""
    # retrieve the user from the database
    user = retrieveUser(account, issuer)

    # create the totp object and generate the corresponding URI
    data = _createTOTP(user.secret_key)
    uri = data.get_provisioning_uri(account, issuer)

    # generate the QRCode
    qr = qrcode.QRCode(
        version=1,
        error_correction=qrcode.constants.ERROR_CORRECT_L,
        box_size=5,
        border=4,
    )
    qr.add_data(uri)
    qr.make()

    # create the image with Pillow and display it
    img = qr.make_image(fill_color="black", back_color="white")

    filename: str = base64.b32encode(f'{account}-{issuer}'.encode()).decode()
    filename = filename.replace('=', '')[:12]
    img.save(f'{filename}.png')
    print(f"Image saved to {filename}.png")


def showTOTP(account: str, issuer: str) -> None:
    """Show the current value for the TOTP"""
    # retrieve the user from the database
    user = retrieveUser(account, issuer)

    # create the TOTP object
    data = _createTOTP(user.secret_key)

    # print the current value every 30s

    old_time = 0
    new_time = int(time.time()) // PASSWORD_TIME

    while True:
        try:
            if new_time > old_time:
                value = data.generate(int(time.time())).decode()
                print(f'{value}')

                old_time = new_time
            else:
                time.sleep(1)

            new_time = int(time.time()) // PASSWORD_TIME

        except KeyboardInterrupt:
            break


def validateTOTP(account: str, issuer: str, value: str) -> bool:
    """Validate a TOTP token"""
    # retrieve the user from the database
    user = retrieveUser(account, issuer)

    # create the TOTP object
    data = _createTOTP(user.secret_key)

    # validate the user input
    try:
        data.verify(value.encode(), int(time.time()))
        print("The code is valid.")

    except InvalidToken:
        print("The code is invalid.")
        return False

    return True