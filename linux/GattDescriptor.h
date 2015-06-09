#pragma once

extern "C" {
  #include "bluetooth.h"
  #include "uuid.h"
  #include "gatt-db.h"
}

#include <string>

namespace bluez {
namespace native {
class GattDescriptor {
public:
  static GattDescriptor* create(gatt_db_attribute* attr);
  ~GattDescriptor();

  std::string getUuid();
  uint16_t getHandle();
  
private:
  GattDescriptor(gatt_db_attribute* attr, uint16_t m_handle, bt_uuid_t m_uuid);

  gatt_db_attribute* m_attribute;
  uint16_t m_handle;
  bt_uuid_t m_uuid;
};
} //native
} //bluez
