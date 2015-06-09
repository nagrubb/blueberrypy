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

  return new GattCharacteristic(attr, handle, valueHandle, properties, uuid);
}

GattCharacteristic::~GattCharacteristic() {
  for(auto i = m_descriptors.begin(); i != m_descriptors.end(); ++i) {
    delete *i;
  }

  m_descriptors.clear();
}

GattCharacteristic::GattCharacteristic(gatt_db_attribute* attr, uint16_t handle, uint16_t valueHandle, uint8_t properties, bt_uuid_t uuid) :
  m_attribute(attr),
  m_handle(handle),
  m_valueHandle(valueHandle),
  m_properties(properties),
  m_uuid(uuid) {

  gatt_db_service_foreach_desc(attr, &GattCharacteristic::_createDescriptor, this);
}

uint16_t GattCharacteristic::getHandle() {
  return m_handle;
}

uint16_t GattCharacteristic::getValueHandle() {
  return m_valueHandle;
}

uint8_t GattCharacteristic::getProperties() {
  return m_properties;
}

std::string GattCharacteristic::getUuid() {
  return uuidToString(&m_uuid);
}

void GattCharacteristic::_createDescriptor(gatt_db_attribute* attr, void* obj) {
  GattCharacteristic* characteristic = static_cast<GattCharacteristic*>(obj);
  characteristic->createDescriptor(attr);
}

void GattCharacteristic::createDescriptor(gatt_db_attribute* attr) {
  GattDescriptor* descriptor = GattDescriptor::create(attr);

  if (descriptor) {
    m_descriptors.push_back(descriptor);
  }
}
