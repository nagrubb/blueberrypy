#pragma once
#include "BtAdapter.h"
#include "GattClient.h"
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
      m_advertisement = advertisement;
  }

  ~BleAdvertisement() {
      delete m_advertisement;
  }

  boost::python::object rssi() {
    return boost::python::object(m_advertisement->rssi());
  }

  boost::python::object addressType() {
    return boost::python::object(m_advertisement->addressType());
  }

  boost::python::object btAddress() {
    return boost::python::object(m_advertisement->btAddress());
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

  boost::python::object generalDiscoverable() {
    if (!m_advertisement->hasFlags()) {
      return boost::python::object();
    }

    return boost::python::object(m_advertisement->generalDiscoverable());
  }

  boost::python::object leOnly() {
    if (!m_advertisement->hasFlags()) {
      return boost::python::object();
    }

    return boost::python::object(m_advertisement->leOnly());
  }

  boost::python::object simulatenousLeBrEdrController() {
    if (!m_advertisement->hasFlags()) {
      return boost::python::object();
    }

    return boost::python::object(m_advertisement->simulatenousLeBrEdrController());
  }

  boost::python::object simulatenousLeBrEdrHost() {
    if (!m_advertisement->hasFlags()) {
      return boost::python::object();
    }

    return boost::python::object(m_advertisement->simulatenousLeBrEdrHost());
  }

  boost::python::object incompleteList16BitServiceClass() {
    return convert(&bluez::native::BleAdvertisement::incompleteList16BitServiceClass);
  }

  boost::python::object incompleteList32BitServiceClass() {
    return convert(&bluez::native::BleAdvertisement::incompleteList32BitServiceClass);
  }

  boost::python::object incompleteList128BitServiceClass() {
    return convert(&bluez::native::BleAdvertisement::incompleteList128BitServiceClass);
  }

  boost::python::object completeList16BitServiceClass() {
    return convert(&bluez::native::BleAdvertisement::completeList16BitServiceClass);
  }

  boost::python::object completeList32BitServiceClass() {
    return convert(&bluez::native::BleAdvertisement::completeList32BitServiceClass);
  }

  boost::python::object completeList128BitServiceClass() {
    return convert(&bluez::native::BleAdvertisement::completeList128BitServiceClass);
  }

  boost::python::object shortenedLocalName() {
    return convert(&bluez::native::BleAdvertisement::shortenedLocalName);
  }

  boost::python::object completeLocalName() {
    return convert(&bluez::native::BleAdvertisement::completeLocalName);
  }

  boost::python::object txPowerLevel() {
    return convert(&bluez::native::BleAdvertisement::txPowerLevel);
  }

  boost::python::object deviceId() {
    return convert(&bluez::native::BleAdvertisement::deviceId);
  }

  boost::python::object slaveConnectionIntervalRange() {
    return convert(&bluez::native::BleAdvertisement::slaveConnectionIntervalRange);
  }

  boost::python::object list16BitServiceSolicitation() {
    return convert(&bluez::native::BleAdvertisement::list16BitServiceSolicitation);
  }

  boost::python::object list32BitServiceSolicitation() {
    return convert(&bluez::native::BleAdvertisement::list32BitServiceSolicitation);
  }

  boost::python::object list128BitServiceSolicitation() {
    return convert(&bluez::native::BleAdvertisement::list128BitServiceSolicitation);
  }

  boost::python::object serviceData16Bit() {
    return convert(&bluez::native::BleAdvertisement::serviceData16Bit);
  }

  boost::python::object serviceData32Bit() {
    return convert(&bluez::native::BleAdvertisement::serviceData32Bit);
  }

  boost::python::object serviceData128Bit() {
    return convert(&bluez::native::BleAdvertisement::serviceData128Bit);
  }

  boost::python::object appearance() {
    return convert(&bluez::native::BleAdvertisement::appearance);
  }

  boost::python::object publicTargetAddress() {
    return convert(&bluez::native::BleAdvertisement::publicTargetAddress);
  }

  boost::python::object advertisingInterval() {
    return convert(&bluez::native::BleAdvertisement::advertisingInterval);
  }

  boost::python::object manufacturerData() {
    return convert(&bluez::native::BleAdvertisement::manufacturerData);
  }

  boost::python::object convert(bool (bluez::native::BleAdvertisement::*method)(std::string&)) {
    std::string tmp;

    if (!(*m_advertisement.*method)(tmp)) {
      return boost::python::object();
    }

    return boost::python::object(tmp);
  }


  bluez::native::BleAdvertisement* m_advertisement;
};

struct BtAdapter : public bluez::native::BtAdapter {
    BtAdapter(int id, PyObject* pyCallback) : bluez::native::BtAdapter(id), m_pyCallback(pyCallback) {
        PyEval_InitThreads();
    }

    virtual bool onAdvertisementScanned(bluez::native::BleAdvertisement* advertisment) {
      BleAdvertisement* wrapper = new BleAdvertisement(advertisment);

  		PyGILState_STATE gstate;
  		gstate = PyGILState_Ensure();
  		call_method<void>(m_pyCallback, "onAdvertisementScanned", wrapper);
      PyGILState_Release(gstate);
      return true;
    }

    PyObject* const m_pyCallback;
    //bluez::native::BtAdapter m_adapter;
};

struct GattClient : bluez::native::GattClient {
    GattClient(std::string btAddress) : bluez::native::GattClient(btAddress)  {}
    ~GattClient() {}
    //bluez::native::GattClient m_client;
};

struct GattCharacteristic {
    GattCharacteristic() {}
};
