#include "GattDescriptor.h"
#include "GattUtilities.h"
#include <iostream>

using namespace std;
using namespace bluez::native;

GattDescriptor* GattDescriptor::create(gatt_db_attribute* attr) {
  uint16_t handle = gatt_db_attribute_get_handle(attr);
  bt_uuid_t uuid = *(gatt_db_attribute_get_type(attr));

  cout << "GattDescriptor::create(" << uuid << ")" << endl;
  return new GattDescriptor(attr, handle, uuid);
}

GattDescriptor::~GattDescriptor() {
  cout << "bluez::native::GattDescriptor::~GattDescriptor" << endl;
}

GattDescriptor::GattDescriptor(gatt_db_attribute* attr, uint16_t handle, bt_uuid_t uuid) :
  m_attribute(attr),
  m_handle(handle),
  m_uuid(uuid) {}

string GattDescriptor::getUuid() {
  return uuidToString(&m_uuid);
}

uint16_t GattDescriptor::getHandle() {
  return m_handle;
}
