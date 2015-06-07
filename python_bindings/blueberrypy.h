#pragma once
#include "BtAdapter.h"
#include <boost/python.hpp>
#include <string>
#include <sstream>
#include <iostream>

using namespace boost::python;

struct BleAdvertisement {
  BleAdvertisement() {
    throw;
  }

  BleAdvertisement(bluez::native::BleAdvertisement* advertisement) {
      std::cout << "BleAdvertisement created!" << std::endl;
      m_advertisement = advertisement;
  }

  ~BleAdvertisement() {
    std::cout << "BleAdvertisement destroyed!" << std::endl;
      delete m_advertisement;
  }

  bool hasFlags() {
    return m_advertisement->hasFlags();
  }

  boost::python::object rawFlags() {
    uint8_t flags;
    if (!m_advertisement->rawFlags(flags)) {
        return boost::python::object();
    }

    return boost::python::object(flags);
  }

  boost::python::object limitedDiscoverable() {
    if (!m_advertisement->hasFlags()) {
      return boost::python::object();
    }

    return boost::python::object(m_advertisement->limitedDiscoverable());
  }

  bool generalDiscoverable() {
    if (!m_advertisement->hasFlags()) {
      return boost::python::object();
    }

    return boost::python::object(m_advertisement->generalDiscoverable());
  }

  bool leOnly() {
    if (!m_advertisement->hasFlags()) {
      return boost::python::object();
    }

    return boost::python::object(m_advertisement->leOnly());
  }

  bool simulatenousLeBrEdrController() {
    if (!m_advertisement->hasFlags()) {
      return boost::python::object();
    }

    return boost::python::object(m_advertisement->simulatenousLeBrEdrController());
  }

  bool simulatenousLeBrEdrHost() {
    if (!m_advertisement->hasFlags()) {
      return boost::python::object();
    }

    return boost::python::object(m_advertisement->simulatenousLeBrEdrHost());
  }

  bluez::native::BleAdvertisement* m_advertisement;
};

struct BtAdapter : public bluez::native::BtAdapter {
    BtAdapter(int id, PyObject* pyCallback) : bluez::native::BtAdapter(id), m_pyCallback(pyCallback) {
        PyEval_InitThreads();
        std::cout << "Ctor!" << std::endl;
    }

    void trigger() {
      call_method<void>(m_pyCallback, "onAdvertisementScanned", "fuck_yeah");
    }

    virtual bool onAdvertisementScanned(bluez::native::BleAdvertisement* advertisment) {
      BleAdvertisement* wrapper = new BleAdvertisement(advertisment);

      // GIL state handler
  		PyGILState_STATE gstate;
  		gstate = PyGILState_Ensure();
  		// Python callback
  		call_method<void>(m_pyCallback, "onAdvertisementScanned", wrapper);

  		// GIL handler release
      PyGILState_Release(gstate);
      return true;
    }

    PyObject* const m_pyCallback;
    //bluez::native::BtAdapter m_adapter;
};

struct BleDevice {
    BleDevice() {}
};

struct GattClient {
    GattClient() {}
};

struct GattCharacteristic {
    GattCharacteristic() {}
};
