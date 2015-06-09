#pragma once

extern "C" {
  #include "bluetooth.h"
  #include "uuid.h"
  #include "gatt-db.h"
}

#include "GattDescriptor.h"
#include <list>
#include <string>

namespace bluez {
namespace native {
class GattCharacteristic {
public:
  static GattCharacteristic* create(gatt_db_attribute* attr);
  ~GattCharacteristic();

  uint16_t getHandle();
  uint16_t getValueHandle();
  uint8_t getProperties();
  std::string getUuid();

private:
  GattCharacteristic(gatt_db_attribute* attr, uint16_t m_handle, uint16_t m_valueHandle, uint8_t m_properties, bt_uuid_t m_uuid);

  static void _createDescriptor(gatt_db_attribute* attr, void* obj);
  void createDescriptor(gatt_db_attribute* attr);

  gatt_db_attribute* m_attribute;
  uint16_t m_handle;
  uint16_t m_valueHandle;
  uint8_t m_properties;
  bt_uuid_t m_uuid;
  std::list<GattDescriptor*> m_descriptors;
};
} //native
} //bluez
