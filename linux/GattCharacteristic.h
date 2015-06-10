#pragma once

extern "C" {
  #include "bluetooth.h"
  #include "uuid.h"
  #include "gatt-db.h"
  #include "gatt-client.h"
}

#include "GattDescriptor.h"
#include <list>
#include <string>

namespace bluez {
namespace native {
class IGattCharacteristicCallback {
public:
  virtual void onReadResponse(bool success, uint8_t attErrorCode, std::string value) = 0;
  virtual void onWriteResponse(bool success, uint8_t attErrorCode) = 0;
};

class GattCharacteristic {
private:
  typedef std::list<GattDescriptor*> DescriptorCollection;

public:
  static GattCharacteristic* create(bt_gatt_client* client, gatt_db_attribute* attr);
  ~GattCharacteristic();

  uint16_t getHandle();
  uint16_t getValueHandle();
  uint8_t getProperties();
  std::string getUuid();

  void bind(IGattCharacteristicCallback* callback);
  void unbind();

  typedef DescriptorCollection::const_iterator DescriptorIterator;
  DescriptorIterator DescriptorCollectionBegin() const { return m_descriptors.begin(); }
  DescriptorIterator DescriptorCollectionEnd() const { return m_descriptors.end(); }

public:
  bool read();
private:
  static void _readCallback(bool success, uint8_t attErrorCode, const uint8_t* value, uint16_t length, void* obj);
  void readCallback(bool success, uint8_t attErrorCode, const uint8_t* value, uint16_t length);

public:
  bool write(std::string& data, bool writeWithResponse = false, bool signedWrite = false);
private:
  static void _writeCallback(bool success, uint8_t attErrorCode, void* obj);
  void writeCallback(bool success, uint8_t attErrorCode);

private:
  GattCharacteristic(bt_gatt_client* client, gatt_db_attribute* attr, uint16_t m_handle, uint16_t m_valueHandle, uint8_t m_properties, bt_uuid_t m_uuid);

  static void _createDescriptor(gatt_db_attribute* attr, void* obj);
  void createDescriptor(gatt_db_attribute* attr);


  bt_gatt_client* m_client;
  gatt_db_attribute* m_attribute;
  uint16_t m_handle;
  uint16_t m_valueHandle;
  uint8_t m_properties;
  bt_uuid_t m_uuid;
  DescriptorCollection m_descriptors;
  IGattCharacteristicCallback* m_callback;
};
} //native
} //bluez
