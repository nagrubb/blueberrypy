#include "GattService.h"
#include "GattUtilities.h"
#include <iostream>

using namespace std;
using namespace bluez::native;

GattService* GattService::create(bt_gatt_client* client, gatt_db_attribute* attr) {
  uint16_t startHandle;
  uint16_t endHandle;
  bool primary;
  bt_uuid_t uuid;

  if (!gatt_db_attribute_get_service_data(attr, &startHandle, &endHandle, &primary, &uuid)) {
    return NULL;
  }

  return new GattService(client, attr, startHandle, endHandle, primary, uuid);
}

GattService::~GattService() {
  for(auto i = m_characteristics.begin(); i != m_characteristics.end(); ++i) {
    delete *i;
  }

  m_characteristics.clear();
}

GattService::GattService(bt_gatt_client* client, gatt_db_attribute* attr, uint16_t startHandle, uint16_t endHandle,
 bool primary, bt_uuid_t uuid) :
  m_client(client),
  m_attribute(attr),
  m_startHandle(startHandle),
  m_endHandle(endHandle),
  m_primary(primary),
  m_uuid(uuid) {

  gatt_db_service_foreach_char(attr, &GattService::_createCharacteristic, this);
}

uint16_t GattService::getStartHandle() {
  return m_startHandle;
}

uint16_t GattService::getEndHandle() {
  return m_endHandle;
}

bool GattService::getPrimary() {
  return m_primary;
}

std::string GattService::getUuid() {
  return uuidToString(&m_uuid);
}

void GattService::_createCharacteristic(gatt_db_attribute* attr, void* obj) {
  GattService* service = static_cast<GattService*>(obj);
  service->createCharacteristic(attr);
}

void GattService::createCharacteristic(gatt_db_attribute* attr) {
  GattCharacteristic* characteristic = GattCharacteristic::create(m_client, attr);

  if (characteristic) {
    m_characteristics.push_back(characteristic);
  }
}
