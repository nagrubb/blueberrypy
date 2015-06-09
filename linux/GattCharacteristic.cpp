#include "GattCharacteristic.h"
#include "GattUtilities.h"
#include <iostream>

using namespace std;
using namespace bluez::native;

GattCharacteristic* GattCharacteristic::create(gatt_db_attribute* attr) {
  uint16_t handle;
  uint16_t valueHandle;
  uint8_t properties;
  bt_uuid_t uuid;

  if (!gatt_db_attribute_get_char_data(attr, &handle, &valueHandle, &properties, &uuid)) {
    return NULL;
  }

  cout << "GattCharacteristic::create(" << uuid << ")" << endl;
  return new GattCharacteristic(attr, handle, valueHandle, properties, uuid);
}

GattCharacteristic::~GattCharacteristic() {
  cout << "bluez::native::GattCharacteristic::~GattCharacteristic" << endl;
}

GattCharacteristic::GattCharacteristic(gatt_db_attribute* attr, uint16_t handle, uint16_t valueHandle, uint8_t properties, bt_uuid_t uuid) :
  m_attribute(attr),
  m_handle(handle),
  m_valueHandle(valueHandle),
  m_properties(properties),
  m_uuid(uuid) {}
