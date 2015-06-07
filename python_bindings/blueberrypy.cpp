#include "blueberrypy.h"

BOOST_PYTHON_MODULE(blueberrypy)
{
  class_<BtAdapter>("BtAdapter", init<int, PyObject*>())
    .def("enableScanning", &BtAdapter::enableScanning)
    .def("disableScanning", &BtAdapter::disableScanning)
    .def("trigger", &BtAdapter::trigger);

  class_<BleAdvertisement>("BleAdvertisement")
    .add_property("rssi", &BleAdvertisement::rssi)
    .add_property("addressType", &BleAdvertisement::addressType)
    .add_property("btAddress", &BleAdvertisement::btAddress)
    .add_property("hasFlags", &BleAdvertisement::hasFlags)
    .add_property("rawFlags", &BleAdvertisement::rawFlags)
    .add_property("limitedDiscoverable", &BleAdvertisement::limitedDiscoverable)
    .add_property("generalDiscoverable", &BleAdvertisement::limitedDiscoverable)
    .add_property("leOnly", &BleAdvertisement::limitedDiscoverable)
    .add_property("simulatenousLeBrEdrController", &BleAdvertisement::limitedDiscoverable)
    .add_property("simulatenousLeBrEdrHost", &BleAdvertisement::limitedDiscoverable)
    .add_property("incompleteList16BitServiceClass", &BleAdvertisement::incompleteList16BitServiceClass)
    .add_property("incompleteList32BitServiceClass", &BleAdvertisement::incompleteList32BitServiceClass)
    .add_property("incompleteList128BitServiceClass", &BleAdvertisement::incompleteList128BitServiceClass)
    .add_property("completeList16BitServiceClass", &BleAdvertisement::completeList16BitServiceClass)
    .add_property("completeList32BitServiceClass", &BleAdvertisement::completeList32BitServiceClass)
    .add_property("completeList128BitServiceClass", &BleAdvertisement::completeList128BitServiceClass)
    .add_property("shortenedLocalName", &BleAdvertisement::shortenedLocalName)
    .add_property("completeLocalName", &BleAdvertisement::completeLocalName)
    .add_property("txPowerLevel", &BleAdvertisement::txPowerLevel)
    .add_property("deviceId", &BleAdvertisement::deviceId)
    .add_property("slaveConnectionIntervalRange", &BleAdvertisement::slaveConnectionIntervalRange)
    .add_property("list32BitServiceSolicitation", &BleAdvertisement::list32BitServiceSolicitation)
    .add_property("list128BitServiceSolicitation", &BleAdvertisement::list128BitServiceSolicitation)
    .add_property("serviceData16Bit", &BleAdvertisement::serviceData16Bit)
    .add_property("serviceData32Bit", &BleAdvertisement::serviceData32Bit)
    .add_property("serviceData128Bit", &BleAdvertisement::serviceData128Bit)
    .add_property("appearance", &BleAdvertisement::appearance)
    .add_property("publicTargetAddress", &BleAdvertisement::publicTargetAddress)
    .add_property("advertisingInterval", &BleAdvertisement::advertisingInterval)
    .add_property("manufacturerData", &BleAdvertisement::manufacturerData);

  class_<BleDevice>("BleDevice");

  class_<GattClient>("GattClient");

  class_<GattCharacteristic>("GattCharacteristic");
}
