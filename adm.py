#!/usr/bin/python

import time
import threading
from GattClient import GattClient, GattCharacteristic, AttErrorCode

class AdmGattCommand(GattCharacteristic):
  def __init__(self, char):
    GattCharacteristic.__init__(self, char)

  def onReadResponse(self, success, attErrorCode, value):
    print 'AdmCommand.onReadResponse({0}, {1}, {2})'.format(success, attErrorCode, value)

  def onWriteResponse(self):
    print 'AdmCommand.onWriteResponse'

class AdmGattClient(GattClient):
  def __init__(self, address, event):
    GattClient.__init__(self, address)
    self.event = event
    self.commandChar = None
    self.responseChar = None
    self.notifyChar = None

  def onServicesDiscovered(self, success, attErrorCode):
    print 'onServicesDiscovered({0})'.format(success);
    if success:
      self.commandChar = AdmGattCommand(self.getCharacteristicByUuid('9ec5d2b8-8f51-4dea-9cd3-f3dea220b5e1'))
      self.responseChar = self.getCharacteristicByUuid('9ec5d2b8-8f51-4dea-9cd3-f3dea220b5e2')
      self.notifyChar = self.getCharacteristicByUuid('9ec5d2b8-8f51-4dea-9cd3-f3dea220b5e3')

      self.commandChar.read()
      print self.commandChar
      print self.responseChar
      print self.notifyChar

    event.set()

print (AttErrorCode.Red)
print type(AttErrorCode.Red)
print (AttErrorCode.Red.name)

print 'Connecting Gatt Client...'
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
