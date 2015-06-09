# blueberrypy

This is still under development. Check back in a week or two.

This project was created/started due to a lack of good Bluetooth Low Energy support out there for Python. There's really good support for NodeJS and GoLang, but for python there's just a whole bunch of libraries and none of them seem to be complete with Bluetooth Low Energy support. I plan on changing that with this library (blueberrypy). The features I plan to support in the first phase (week 1) are:

- Enable/disabling scanning (Done)
- Parsing advertisement data (Done)
- Python callback with parsed advertisement (Done)
- Connect/disconnect BleDevice (Done)
- Enumeration of services (Done)
- Enumeration of characteristics (Done)
- Enumeration of descriptors (In progress)
- Exposing services with python bindings (Not done)
- Exposing characteristics with python bindings
- Read characteristic with python bindings
- Write characteristic with python bindings
- Notifty characteristic with python bindings
- Indicate characteristic with python bindings

Issues
 - Read advertisment thread never exits
 - Mainloop thread never exits
