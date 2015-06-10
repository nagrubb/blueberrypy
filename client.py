#!/usr/bin/python

import time
import threading
import blueberrypy

class GattClient:
  def __init__(self, address, event):
    self.event = event
    self.client = blueberrypy.GattClient(address, self)

  def onServicesDiscovered(self, success, attErrorCode):
    print 'Services Discovery Complete'

    if (success) :
      for service in self.client.services:
        print 'Service UUID: {0}'.format(service.uuid)
        print '  Primary: {0}'.format(service.primary)
        print '  Start Handle: {0}'. format(service.startHandle)
        print '  End Handle: {0}'. format(service.endHandle)

        for characteristic in service.characteristics:
          print '    Charateristic UUID: {0}'.format(characteristic.uuid)
          print '      Handle: {0}'.format(characteristic.handle)
          print '      Value Handle: {0}'.format(characteristic.valueHandle)
          print '      Properties: {0}'.format(characteristic.properties)

          for descriptor in characteristic.descriptors:
            print '        Descriptor UUID: {0}'.format(descriptor.uuid)
            print '          Handle: {0}'.format(descriptor.handle)

    else:
      print 'Failed: {0}'.format(attErrorCode)
    self.event.set()

  def connect(self):
    return self.client.connect()

  def disconnect(self):
    return self.client.disconnect()

  def services(self):
    return self.client.services

print 'Connecting Gatt Client...'
event = threading.Event()
client = GattClient('CA:17:34:15:52:0B', event)
if client.connect():
  print 'Connected'
  event.wait()
  time.sleep(5)
  client.disconnect()
else:
  print 'Failed to connect'

client = None
