# pyQRCode

![Python](https://img.shields.io/badge/Python-3.9.6-blue?style=flat-square)

---

## **Abstract**

This project aims at easily create a QRCode for various cases:

- Call a phone number
- Send an email with a predefined subject
- Access a webpage
- Generate WiFi parameters for auto-connection
- Generate a VCard Contact

The only dependency is the [QRCode](https://pypi.org/project/qrcode/) python library.


## **Usage**

The script uses a plugins architecture to support multiple sources for the QRCode generation.
The help command lists all the available plugins

``` bash
$ python main --help
usage: main.py [-h] --image IMAGE --type {VCard,Wifi,Phone,URL,Email}

QRCode generator

optional arguments:
  -h, --help            show this help message and exit
  --image IMAGE         Output image filename
  --type {VCard,Wifi,Phone,URL,Email}
                        QRCode type
```

Generating a QRCode for a particular type is just a matter of answering few questions.

``` bash
$ python main.py --image qrcode.png --type URL
Please enter your URL: https://www.github.com
QRCode saved to qrcode.png.
```

Once done, a new QRCode image is generated  
![](./qrcode.png)



## **Plugin Architecture**

Each QRCode type has its own plugin to define it.  
Adding more type is only a matter of adding more plugins.  

A plugin should inherit from `PluginTask` class and register to the `PluginManager` with the provided python decorator.

``` python
# you need to import these two classes
from definitions import PluginManager, PluginTask

# define your plugin and register it
@PluginManager.register
class MyNewPlugin(PluginTask):
    ...
```

A plugin must implement the `process` function that takes no parameters and returns a `bytes` data.

``` python
    def process() -> bytes:
        ...
```

In this function, you can request the user to fill in information wether with a `input` function (like in the `Email` plugin) or open an editor for multiple fields (like in the `VCard` plugin).

New plugins will be automatically available with the `--type` argument.