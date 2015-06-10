import blueberrypy

class GattCharacteristic:
  def __init__(self, char):
    self.char = char
    print char
    char.bind(self)

  def read(self):
    return self.char.read()

  def onReadResponse(self, success, attErrorCode, value):
    pass;

  def onWriteResponse(self):
    print 'GattCharacteristic.onWriteResponse'

class GattClient(object):
  def __init__(self, address):
    self.client = blueberrypy.GattClient(address, self)

  def onServicesDiscovered(self, success, attErrorCode):
    pass

  def printDatabase(self):
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

  def getCharacteristicByUuid(self, uuid):
    for service in self.client.services:
      for characteristic in service.characteristics:
        if characteristic.uuid == uuid:
          return characteristic

    return None

  def connect(self):
    return self.client.connect()

  def disconnect(self):
    return self.client.disconnect()
