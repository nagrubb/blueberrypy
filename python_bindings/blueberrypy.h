#pragma once
#include "BtAdapter.h"
#include <boost/python.hpp>
#include <string>
#include <sstream>

using namespace boost::python;

struct BtAdapter {
    BtAdapter(int id, PyObject* pyCallback) : m_adapter(id), m_pyCallback(pyCallback) {}
    bool enableScanning() { return m_adapter.enableScanning(); }
    bool disableScanning() { return m_adapter.disableScanning(); }

    void trigger() {
      call_method<bool>(m_pyCallback, "data", "fuck_yeah");
    }

    PyObject* m_pyCallback;
    bluez::native::BtAdapter m_adapter;
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
