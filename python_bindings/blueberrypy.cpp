#include "blueberrypy.h"
#include <boost/python.hpp>
using namespace boost::python;

BOOST_PYTHON_MODULE(blueberrypy)
{
  class_<BtAdapter>("BtAdapter", init<int>())
    .def("enableScanning", &BtAdapter::enableScanning)
    .def("disableScanning", &BtAdapter::disableScanning);

  class_<BleDevice>("BleDevice");

  class_<GattClient>("GattClient");

  class_<GattCharacteristic>("GattCharacteristic");
}
