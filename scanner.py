#!/usr/bin/python

import time
from blueberrypy import *

class BtAdapterEvents:
  def onAdvertisementScanned(self, adv):
     print 'Found Device'
     print '\tBluetooth Address: {0}'.format(adv.btAddress)
     print '\tAddress Type: {0}'.format(adv.addressType)
     print '\tRSSI: {0}'.format(adv.rssi)

     if (adv.shortenedLocalName is not None):
       print '\tShortened Local Name: {0}'.format(adv.shortenedLocalName)

     if (adv.completeLocalName is not None):
       print '\tComplete Local Name: {0}'.format(adv.completeLocalName)


events = BtAdapterEvents()
btAdapter = BtAdapter(0, events)
print 'enableScanning returned {0}'.format(btAdapter.enableScanning())
time.sleep(5)
print 'disableScanning returned {0}'.format(btAdapter.disableScanning())
