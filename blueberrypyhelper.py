import blueberrypy

class BtAdapter:
  def __init__(self, id):
    self.btAdapter = blueberrypy.BtAdapter(id, self)

  def enableScanning(self):
    return self.btAdapter.enableScanning()

  def disableScanning(self):
    return self.btAdapter.disableScanning()

  def onAdvertisementScanned(self, adv):
    pass

  def printAdvertisement(self, adv):
    print 'Bluetooth Address: {0}'.format(adv.btAddress)
    print '  Address Type: {0}'.format(adv.addressType)
    print '  RSSI: {0}'.format(adv.rssi)

    if (adv.shortenedLocalName is not None):
      print '  Shortened Local Name: {0}'.format(adv.shortenedLocalName)

    if (adv.completeLocalName is not None):
      print '  Complete Local Name: {0}'.format(adv.completeLocalName)

class GattCharacteristic:
  def __init__(self, char):
    self.char = char
    char.bind(self)

  def read(self):
    return self.char.read()

  def write(self, data, writeWithResponse = False, signedWrite = False):
    return self.char.write(data, writeWithResponse, signedWrite)

  def registerNotify(self):
    return self.char.registerNotify()

  def unregisterNotify(self):
    return self.unregisterNotify()

  def onReadResponse(self, success, attErrorCode, value):
    pass

  def onWriteResponse(self, success, attErrorCode):
    pass

  def onRegistration(self, attErrorCode):
    pass

  def  onNotification(self, value):
    pass

class GattClient(object):
  def __init__(self):
    self.client = blueberrypy.GattClient(self)

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

  def connect(self, address):
    return self.client.connect(address)

  def disconnect(self):
    return self.client.disconnect()
