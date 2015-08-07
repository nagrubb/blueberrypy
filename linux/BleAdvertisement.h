#pragma once
#include <stdint.h>
#include <string>
#include <map>

namespace bluez {
namespace native {
enum class BleAdvertisementType : uint8_t {
  ConnectableUndirected = 0x00,
  ConnectableDirected = 0x01,
  ScannableUndirected = 0x02,
  NonConnectableUndirected = 0x03,
  ScanResponse = 0x04
};

class BleAdvertisement {
public:
  static BleAdvertisement* parse(void* adv_info);
  virtual ~BleAdvertisement() {}

  BleAdvertisementType type() { return m_type; }
  uint8_t rssi() { return m_rssi; }
  std::string addressType() { return m_addressType; }
  std::string btAddress() { return m_btAddress; }

  bool hasFlags();
  bool rawFlags(uint8_t& flags);
  bool limitedDiscoverable();
  bool generalDiscoverable();
  bool leOnly();
  bool simulatenousLeBrEdrController();
  bool simulatenousLeBrEdrHost();

  bool incompleteList16BitServiceClass(std::string& serviceClass);
  bool incompleteList32BitServiceClass(std::string& serviceClass);
  bool incompleteList128BitServiceClass(std::string& serviceClass);
  bool completeList16BitServiceClass(std::string& serviceClass);
  bool completeList32BitServiceClass(std::string& serviceClass);
  bool completeList128BitServiceClass(std::string& serviceClass);

  bool shortenedLocalName(std::string& name);
  bool completeLocalName(std::string& name);

  bool txPowerLevel(std::string& powerLevel);
  bool deviceId(std::string& deviceId);
  bool slaveConnectionIntervalRange(std::string& intervalRange);
  bool list16BitServiceSolicitation(std::string& service);
  bool list32BitServiceSolicitation(std::string& service);
  bool list128BitServiceSolicitation(std::string& service);
  bool serviceData16Bit(std::string& serviceData);
  bool serviceData32Bit(std::string& serviceData);
  bool serviceData128Bit(std::string& serviceData);
  bool appearance(std::string& appearance);
  bool publicTargetAddress(std::string& targetAddress);
  bool advertisingInterval(std::string& advertisingInterval);
  bool manufacturerData(std::string& advertisingInterval);

private:
  BleAdvertisement() {}

  template<class T>
  bool getValue(T type, std::string& value) {
    auto pair = m_parts.find((uint8_t) type);

    if (pair == m_parts.end()) {
      return false;
    }

    value = pair->second;
    return true;
  }

  template<class T>
  bool isFlagSet(T flag) {
    uint8_t tmp;

    if (!rawFlags(tmp)) {
      throw;
    }

    if (tmp & (uint8_t) flag) {
      return true;
    }

    return false;
  }

  BleAdvertisementType m_type;
  uint8_t m_rssi;
  std::string m_addressType;
  std::string m_btAddress;
  std::map<uint8_t, std::string> m_parts;
};
} //native
} //bluez
