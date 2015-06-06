import time
from blueberrypy import *

class BtAdapterEvents:
  def __init__(self):
     print 'BtAdapterEvents'
  
  def deviceDiscovered(self):
     print 'DeviceDiscovered'

events = BtAdapterEvents()
btAdapter = BtAdapter(0, events)
fSuccess = btAdapter.enableScanning()
print fSuccess
import time.sleep(1000) 
