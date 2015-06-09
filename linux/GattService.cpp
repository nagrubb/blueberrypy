#include "GattService.h"

using namespace bluez::native;

GattService* GattService::create(gatt_db_attribute* attr) {
  uint16_t startHandle;
  uint16_t endHandle;
	bool primary;
	bt_uuid_t uuid;

	if (!gatt_db_attribute_get_service_data(attr, &startHandle, &endHandle, &primary, &uuid)) {
		return NULL;
  }

  return new GattService(attr, startHandle, endHandle, primary, uuid);
}

GattService::~GattService() {}

GattService::GattService(gatt_db_attribute* attr, uint16_t startHandle, uint16_t endHandle,
 bool primary, bt_uuid_t uuid) :
  m_attribute(attr), m_startHandle (startHandle), m_endHandle(endHandle), m_primary(primary),
  m_uuid(uuid) {}
