#pragma once
#include "BtAdapter.h"

#include <string>
#include <sstream>

struct BtAdapter {
    BtAdapter(int id) : m_adapter(id) {}
    bool enableScanning() { return m_adapter.enableScanning(); }
    bool disableScanning() { return m_adapter.disableScanning(); }
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
