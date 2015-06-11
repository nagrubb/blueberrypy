#include "blueberrypy.h"

BOOST_PYTHON_MODULE(blueberrypy)
{
  enum_<AttErrorCode>("AttErrorCode")
    .value("NoError", AttErrorCode::NoError)
    .value("InvalidHandle", AttErrorCode::InvalidHandle)
    .value("ReadNotPermitted", AttErrorCode::ReadNotPermitted)
    .value("WriteNotPermitted", AttErrorCode::WriteNotPermitted)
    .value("InvalidPdu", AttErrorCode::InvalidPdu)
    .value("Authentication", AttErrorCode::Authentication)
    .value("RequestNotSupported", AttErrorCode::RequestNotSupported)
    .value("InvalidOffset", AttErrorCode::InvalidOffset)
    .value("Authorization", AttErrorCode::Authorization)
    .value("PrepareQueueFull", AttErrorCode::PrepareQueueFull)
    .value("AttributeNotFound", AttErrorCode::AttributeNotFound)
    .value("AttributeNotLong", AttErrorCode::AttributeNotLong)
    .value("InsufficientEncryptionKeySize", AttErrorCode::InsufficientEncryptionKeySize)
    .value("InvalidAttributeValueLength", AttErrorCode::InvalidAttributeValueLength)
    .value("Unlikely", AttErrorCode::Unlikely)
    .value("InsufficientEncryption", AttErrorCode::InsufficientEncryption)
    .value("UnsupportedGroupType", AttErrorCode::UnsupportedGroupType)
    .value("InsufficientResources", AttErrorCode::InsufficientResources);

  class_<BtAdapter>("BtAdapter", init<int, PyObject*>())
    .def("enableScanning", &BtAdapter::enableScanning)
    .def("disableScanning", &BtAdapter::disableScanning);

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

  class_<GattClient>("GattClient", init<PyObject*>())
    .def("connect", &GattClient::connect)
    .def("disconnect", &GattClient::disconnect)
    .add_property("services", &GattClient::getServices);

  class_<GattService>("GattService")
    .add_property("startHandle", &GattService::getStartHandle)
    .add_property("endHandle", &GattService::getEndHandle)
    .add_property("primary", &GattService::getPrimary)
    .add_property("uuid", &GattService::getUuid)
    .add_property("characteristics", &GattService::getCharacteristics);

  class_<GattCharacteristic>("GattCharacteristic")
    .add_property("handle", &GattCharacteristic::getHandle)
    .add_property("valueHandle", &GattCharacteristic::getValueHandle)
    .add_property("properties", &GattCharacteristic::getProperties)
    .add_property("uuid", &GattCharacteristic::getUuid)
    .add_property("descriptors", &GattCharacteristic::getDescriptors)
    .def("bind", &GattCharacteristic::bind)
    .def("unbind", &GattCharacteristic::unbind)
    .def("read", &GattCharacteristic::read)
    .def("write", &GattCharacteristic::write)
    .def("registerNotify", &GattCharacteristic::registerNotify)
    .def("unregisterNotify", &GattCharacteristic::unregisterNotify);

  class_<GattDescriptor>("GattDescriptor")
    .add_property("handle", &GattDescriptor::getHandle)
    .add_property("uuid", &GattDescriptor::getUuid);
}
