#!/usr/bin/python

import time
import blueberrypy

class GattClient:
  def __init__(self, address):
    self.client = blueberrypy.GattClient(address, self)

  def onServicesDiscovered(self):
    print 'onServicesDiscovered'
    print self.client.services
    #for service in self.client.services:
    #  print service.uuid;

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
