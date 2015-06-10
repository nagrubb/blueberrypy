#pragma once

extern "C" {
  #include "bluetooth.h"
  #include "uuid.h"
  #include "gatt-db.h"
  #include "gatt-client.h"
}

#include "GattCharacteristic.h"
#include <stdint.h>
#include <list>

namespace bluez {
namespace native {
class GattService {
private:
  typedef std::list<GattCharacteristic*> CharacteristicCollection;

public:
  static GattService* create(bt_gatt_client* client, gatt_db_attribute* attr);
  ~GattService();

  uint16_t getStartHandle();
  uint16_t getEndHandle();
  bool getPrimary();
  std::string getUuid();

  typedef CharacteristicCollection::const_iterator CharacteristicIterator;
  CharacteristicIterator CharacteristicCollectionBegin() const { return m_characteristics.begin(); }
  CharacteristicIterator CharacteristicCollectionEnd() const { return m_characteristics.end(); }

private:
  GattService(bt_gatt_client* client, gatt_db_attribute* attr, uint16_t startHandle, uint16_t endHandle, bool primary, bt_uuid_t uuid);
  static void _createCharacteristic(gatt_db_attribute* attr, void* obj);
  void createCharacteristic(gatt_db_attribute* attr);

  bt_gatt_client* m_client;
  gatt_db_attribute* m_attribute;
  uint16_t m_startHandle;
  uint16_t m_endHandle;
  bool m_primary;
  bt_uuid_t m_uuid;
  CharacteristicCollection m_characteristics;
};
} //native
} //bluez
