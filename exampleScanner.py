#!/usr/bin/python

import time
from blueberrypyhelper import BtAdapter 

class ExampleScanner(BtAdapter):
  def __init__(self):
     BtAdapter.__init__(self, 0)

  def onAdvertisementScanned(self, adv):
     self.printAdvertisement(adv)

scanner = ExampleScanner()
print 'enableScanning returned {0}'.format(scanner.enableScanning())
time.sleep(5)
print 'disableScanning returned {0}'.format(scanner.disableScanning())
