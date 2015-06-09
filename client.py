#!/usr/bin/python

import time
from blueberrypy import *

print 'Creating Gatt Client...'
client = GattClient('CA:17:34:15:52:0B')
client.connect()
time.sleep(6)
client.disconnect()
client = None
