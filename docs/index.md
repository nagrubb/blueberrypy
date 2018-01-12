### Blueberrypy
Blueberrypy is a Bluetooth Low Energy module for Python. It's still in active development, but the major plan is to support the Central role in Bluetooth Low Energy. There's really good support on Bluetooth Low Energy in other languages, but for Python I couldn't find any complete library. This library currently only supports Linux, but it has a native binding on bluez (it doesn't rely on any other executable for any reason). It's created mainly with boost python bindings.

Eventually the plan is to support Peripheral role and hopefully support OSX.

After cloning this repo, you can build the python bindings and try this out:
```
$ cd blueberrypy2
$ mkdir build
$ cd build
$ cmake ..
$ make
$ sudo ./exampleScanner.py
$ sudo ./exampleClient.py
```

### Features
The features that are currently implemented are:
* Enable/disabling scanning
* Parsing advertisements
* Connecting to and disconnecting from a GATT server.
* Discovering GATT services and characteristics.
* Single read and write of a characteristic (i.e. a write under ATT_MTU).
* Registering for indications and notifications and receiving them.

### Authors and Contributors
(@silent-snowman) started blueberrypy.

