#include "GattCharacteristic.h"
#include "GattUtilities.h"
#include <iostream>

using namespace std;
using namespace bluez::native;

GattCharacteristic* GattCharacteristic::create(bt_gatt_client* client, gatt_db_attribute* attr) {
  uint16_t handle;
  uint16_t valueHandle;
  uint8_t properties;
  bt_uuid_t uuid;

  if (!gatt_db_attribute_get_char_data(attr, &handle, &valueHandle, &properties, &uuid)) {
    return NULL;
  }

  return new GattCharacteristic(client, attr, handle, valueHandle, properties, uuid);
}

GattCharacteristic::~GattCharacteristic() {
  for(auto i = m_descriptors.begin(); i != m_descriptors.end(); ++i) {
    delete *i;
  }

  m_descriptors.clear();
}

GattCharacteristic::GattCharacteristic(bt_gatt_client* client, gatt_db_attribute* attr, uint16_t handle, uint16_t valueHandle, uint8_t properties, bt_uuid_t uuid) :
  m_client(client),
  m_attribute(attr),
  m_handle(handle),
  m_valueHandle(valueHandle),
  m_properties(properties),
  m_uuid(uuid),
  m_notifyId(0) {

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

void GattCharacteristic::bind(IGattCharacteristicCallback* callback) {
  m_callback = callback;
}

void GattCharacteristic::unbind() {
  m_callback = NULL;
}

bool GattCharacteristic::read() {
  unsigned int id = bt_gatt_client_read_value(m_client, m_valueHandle, &GattCharacteristic::_readCallback, this, NULL);
  return (id != 0);
}

void GattCharacteristic::_readCallback(bool success, uint8_t attErrorCode, const uint8_t* value, uint16_t length, void* obj) {
  GattCharacteristic* characteristic = static_cast<GattCharacteristic*>(obj);
  characteristic->readCallback(success, attErrorCode, value, length);
}

void GattCharacteristic::readCallback(bool success, uint8_t attErrorCode, const uint8_t* value, uint16_t length) {
  if (m_callback) {
    if (value && length > 0) {
      m_callback->onReadResponse(success, attErrorCode, string(reinterpret_cast<const char*>(value), length));
    } else {
      m_callback->onReadResponse(success, attErrorCode, string());
    }
  }
}

bool GattCharacteristic::write(std::string& data, bool writeWithResponse, bool signedWrite) {
  const uint8_t* value = reinterpret_cast<const uint8_t*>(data.c_str());
  unsigned int id = 0;

  if (writeWithResponse) {
    id = bt_gatt_client_write_value(m_client, m_valueHandle, value, data.length(), &GattCharacteristic::_writeCallback, this, NULL);
  } else {
    id = bt_gatt_client_write_without_response(m_client, m_valueHandle, signedWrite, value, data.length());
  }

  return (id != 0);
}

void GattCharacteristic::_writeCallback(bool success, uint8_t attErrorCode, void* obj) {
  GattCharacteristic* characteristic = static_cast<GattCharacteristic*>(obj);
  characteristic->writeCallback(success, attErrorCode);
}

void GattCharacteristic::writeCallback(bool success, uint8_t attErrorCode) {
  if (m_callback) {
    m_callback->onWriteResponse(success, attErrorCode);
  }
}

bool GattCharacteristic::registerNotify() {
  m_notifyId = bt_gatt_client_register_notify(m_client, m_valueHandle, &GattCharacteristic::_registerCallback, &GattCharacteristic::_notifyCallback, this, NULL);
  return (m_notifyId != 0);
}

bool GattCharacteristic::unregisterNotify() {
  cout << __PRETTY_FUNCTION__ << endl;
  return bt_gatt_client_unregister_notify(m_client, m_notifyId);
}

void GattCharacteristic::_registerCallback(uint16_t attErrorCode, void* obj) {
  GattCharacteristic* characteristic = reinterpret_cast<GattCharacteristic*>(obj);
  characteristic->registerCallback(attErrorCode);
}

void GattCharacteristic::registerCallback(uint16_t attErrorCode) {
  if (m_callback) {
    m_callback->onRegistration(attErrorCode);
  }
}

void GattCharacteristic::_notifyCallback(uint16_t valueHandle, const uint8_t* value, uint16_t length, void* obj) {
  GattCharacteristic* characteristic = static_cast<GattCharacteristic*>(obj);
  characteristic->notifyCallback(valueHandle, value, length);
}

void GattCharacteristic::notifyCallback(uint16_t valueHandle, const uint8_t* value, uint16_t length) {
  if (m_callback) {
    if (value && length > 0) {
      m_callback->onNotification(string(reinterpret_cast<const char*>(value), length));
    } else {
      m_callback->onNotification(string());
    }
  }
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
