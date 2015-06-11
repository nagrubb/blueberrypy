#!/usr/bin/python

import sys
import time
import threading
from blueberrypyhelper import GattClient, GattCharacteristic

class ExampleClient(GattClient):
  def __init__(self, event):
    GattClient.__init__(self)
    self.event = event

  def onServicesDiscovered(self, success, attErrorCode):
    print 'onServicesDiscovered({0}, {1})'.format(success, attErrorCode);
    if success:
      self.printDatabase()
    event.set()

sys.stdout.write('Connecting Gatt Client...')
event = threading.Event()
client = ExampleClient(event)
if client.connect('CA:17:34:15:52:0B'):
  print 'Connected'
  event.wait()
  client.disconnect()
else:
  print 'Failed to connect'

client = None
