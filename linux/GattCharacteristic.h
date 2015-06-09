#pragma once

extern "C" {
  #include "bluetooth.h"
  #include "uuid.h"
  #include "gatt-db.h"
}

namespace bluez {
namespace native {
class GattCharacteristic {
public:
  static GattCharacteristic* create(gatt_db_attribute* attr);
  ~GattCharacteristic();

private:
  GattCharacteristic(gatt_db_attribute* attr, uint16_t m_handle, uint16_t m_valueHandle, uint8_t m_properties, bt_uuid_t m_uuid);

  gatt_db_attribute* m_attribute;
  uint16_t m_handle;
  uint16_t m_valueHandle;
  uint8_t m_properties;
  bt_uuid_t m_uuid;
};
} //native
} //bluez
