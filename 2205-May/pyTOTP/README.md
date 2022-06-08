# pyTOTP

![Python](https://img.shields.io/badge/Python-3.9.6-blue?style=flat-square)

---

## **Abstract**

This project aims at creating a TOTP manager only with a command line interface (CLI).

The libraries used are:

- [click](https://pypi.org/project/click/)
- [SQLite](https://docs.python.org/3/library/sqlite3.html)
- [SQLAlchemy](https://www.sqlalchemy.org/)
- [QRCode](https://pypi.org/project/qrcode/)
- [Cryptography](https://pypi.org/project/cryptography/)


## **Usage**

All the commands (except stated otherwise), take 2 arguments:

- *account*: this can be any string (name, email, ...) that represents you
- *issuer*: this represents the other party (google, your bank, ...)

Available commands are listed below.

1. *create*

This will create a new entry in the database and assign a random secret key.

``` bash
python main.py create jdoe acme.corp
```

If the pair (account, issuer) already exists in the database, an error is thrown.

``` bash
python main.py create jdoe acme.corp
ERROR:root:Sorry, an entry for jdoe/acme.corp already exists in the database.
```

The **SQLite** database will be created in the current directory.
You can change the location by setting the environment variable *TOTP_DATABASE_NAME*.

2. *rfc6238*

This will output in the terminal a string compatible with the [RFC 6238](https://www.rfc-editor.org/rfc/rfc6238).
This value can then be used in software like [KeePassXC](https://keepassxc.org/).

``` bash
python main.py rfc6238 jdoe acme.corp
RFC 6238 value: ZJWHTRRFD6DREDEVEHVO6BYJCRV7FKSU
```

3. *qrcode*

To configure your mobile application (for ex. Google Authenticator), you can generate a QRCode with this option.

``` bash
python main.py qrcode jdoe acme.corp
Image saved to NJSG6ZJNMFRW.png
```

The image is saved locally as a PNG file:

![](./qrcode.png)


4. *show*

Display the current TOTP value indefinitely or until CTRL+C is pressed.

``` bash
python main.py show jdoe acme.corp
Press CTRL+C to stop
527525
108482
```

5. *check*

Check if a code is valid or not.

``` bash
# this is a valid code
python main.py check jdoe acme.corp 594955
The code is valid.

# this is an invalid code
python main.py check jdoe acme.corp 123456
The code is invalid.
```

