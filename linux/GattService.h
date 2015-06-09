#pragma once

extern "C" {
  #include "bluetooth.h"
  #include "uuid.h"
  #include "gatt-db.h"
}

#include "GattCharacteristic.h"
#include <stdint.h>
#include <list>

namespace bluez {
namespace native {
class GattService {
public:
  static GattService* create(gatt_db_attribute* attr);
  ~GattService();

private:
  GattService(gatt_db_attribute* attr, uint16_t startHandle, uint16_t endHandle, bool primary, bt_uuid_t uuid);
  static void _createCharacteristic(gatt_db_attribute* attr, void* obj);
  void createCharacteristic(gatt_db_attribute* attr);

  gatt_db_attribute* m_attribute;
  uint16_t m_startHandle;
  uint16_t m_endHandle;
  bool m_primary;
  bt_uuid_t m_uuid;
  std::list<GattCharacteristic*> m_characteristics;
};
} //native
} //bluez
