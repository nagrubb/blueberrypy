#!/usr/bin/python

import time
import blueberrypy

class GattClient:
  def __init__(self, address):
    self.client = blueberrypy.GattClient(address, self)

  def onServicesDiscovered(self):
    print 'onServicesDiscovered'
    print self.client.services
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

  def connect(self):
    return self.client.connect()

  def disconnect(self):
    return self.client.disconnect()

  def services(self):
    return self.client.services

print 'Creating Gatt Client...'
client = GattClient('CA:17:34:15:52:0B')
client.connect()
time.sleep(6)
client.disconnect()
client = None
