#!/usr/bin/python

import sys
import time
import threading
from blueberrypyhelper import GattClient, GattCharacteristic

class ExampleClient(GattClient):
  def __init__(self, address, event):
    GattClient.__init__(self, address)
    self.event = event
    self.charOne = None
    self.charTwo = None
    self.charThree = None

  def onServicesDiscovered(self, success, attErrorCode):
    print 'onServicesDiscovered({0}, {1})'.format(success, attErrorCode);
    if success:
      self.printDatabase()
    event.set()

sys.stdout.write('Connecting Gatt Client...')
event = threading.Event()
client = AdmGattClient('CA:17:34:15:52:0B', event)
if client.connect():
  print 'Connected'
  event.wait()
  time.sleep(3)
  client.disconnect()
else:
  print 'Failed to connect'

client = None
