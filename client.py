from blueberrypy import *

print 'Creating Gatt Client...'
client = GattClient('CA:17:34:15:52:0B')
client.connect()
client.disconnect()
client = None
