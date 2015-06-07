#!/usr/bin/python

import time
from blueberrypy import *

class BtAdapterEvents:
  def __init__(self):
     print 'BtAdapterEvents'
  
  def onAdvertisementScanned(self, adv):
     print 'onAdvertisementScanner'
     print adv
     print adv.hasFlags()
     print adv.rawFlags()
     print 'done'


events = BtAdapterEvents()
btAdapter = BtAdapter(0, events)
print btAdapter.enableScanning()
time.sleep(5) 
print btAdapter.disableScanning()
