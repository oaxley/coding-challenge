# pyQRreader

![Python](https://img.shields.io/badge/Python-3.9.6-blue?style=flat-square)

---

## **Abstract**

This project reads a QRCode and extract the information in it.  
It's the opposite of the [pyQRCode](../pyQRCode/README.md) script.  


## **Dependencies**

The script is dependent on the zbar shared library:

``` bash
sudo apt install libzbar0
```

You will also need the following Python packages:

- pyzbar
- PIL
- Phonenumbers

You can check this [link](https://pypi.org/project/pyzbar/) for details.


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

A plugin must implement the `process` function that takes `bytes` as parameter and 
does not return any values.

``` python
    def process(data: bytes) -> None:
        ...
```

In this function you can decode the data and present it to the user.
