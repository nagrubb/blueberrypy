#include "blueberrypy.h"

BOOST_PYTHON_MODULE(blueberrypy)
{
  class_<BtAdapter>("BtAdapter", init<int, PyObject*>())
    .def("enableScanning", &BtAdapter::enableScanning)
    .def("disableScanning", &BtAdapter::disableScanning)
    .def("trigger", &BtAdapter::trigger);

  class_<BleAdvertisement>("BleAdvertisement")
    .def("hasFlags", &BleAdvertisement::hasFlags)
    .def("rawFlags", &BleAdvertisement::rawFlags)
    .def("limitedDiscoverable", &BleAdvertisement::limitedDiscoverable);

  class_<BleDevice>("BleDevice");

  class_<GattClient>("GattClient");

  class_<GattCharacteristic>("GattCharacteristic");
}
