#include "BleAdvertisement.h"
#include <iostream>
#include <string>

#include "bluetooth.h"
#include "hci.h"
#include "hci_lib.h"

using namespace std;
using namespace bluez::native;

enum class adv_data_type_t : uint8_t {
	Flags = 0x01,
	IncompleteList16BitServiceClass = 0x02,
	CompleteList16BitServiceClass = 0x03,
	IncompleteList32BitServiceClass = 0x04,
	CompleteList32BitServiceClass = 0x05,
	IncompleteList128BitServiceClass = 0x06,
	CompleteList128BitServiceClass = 0x07,
	ShortenedLocalName = 0x08,
	CompleteLocalName = 0x09,
	TxPowerLevel = 0x0A,
	DeviceId = 0x10,
	SlaveConnectionIntervalRange = 0x12,
	List16BitServiceSolicitation = 0x14,
	List32BitServiceSolicitation = 0x1F,
	List128BitServiceSolicitation = 0x15,
	ServiceData16Bit = 0x16,
	ServiceData32Bit = 0x20,
	ServiceData128Bit = 0x21,
	Appearance = 0x19,
	PublicTargetAddress = 0x17,
	RandomTargetAddress = 0x18,
	AdvertisingInterval = 0x1A,
	ManufacturerData = 0xFF
};

enum class adv_data_flag_t {
	LimitedDiscoverable       			= 0x01,
	GeneralDiscoverable       			= 0x02,
	LeOnly                       		= 0x04,
	SimulatenousLeBrEdrController   = 0x08,
	SimulatenousLeBrEdrHost   			= 0x10
};

BleAdvertisement* BleAdvertisement::parse(void* adv_info) {
	BleAdvertisement* adv = new BleAdvertisement();
	le_advertising_info* info = (le_advertising_info*) adv_info;
	uint8_t* data = info->data;
	size_t dataLength = info->length;

	adv->m_rssi = *(info->data + info->length);
	adv->m_addressType = (info->bdaddr_type == LE_PUBLIC_ADDRESS) ? "public" : "random";

	char tmp[18];
	ba2str(&info->bdaddr, tmp);
	adv->m_btAddress = tmp;

	//Advertisment parts are in the following format:
	//Byte 1: Length (Number of bytes for the following Type and Data fields)
	//Byte 2: Type (The type of advertisment part)
	//Byte 3: Value
	while (dataLength >= 3) {
		size_t length = data[0];
		adv_data_type_t type = (adv_data_type_t) data[1];

		--dataLength;
		--length; //length includes itself as one byte

		if (length > dataLength) {
			length = dataLength - 1;
		}

		std::string value((const char*) &data[2], length);
		data += (length + 2);
		dataLength -= (length + 1);

		adv->m_parts[(uint8_t) type] = value;
	}

	return adv;
}

bool BleAdvertisement::hasFlags() {
	std::string value;

	return getValue(adv_data_type_t::Flags, value);
}

bool BleAdvertisement::rawFlags(uint8_t& flags) {
	std::string value;

	if (getValue(adv_data_type_t::Flags, value)) {
		flags = value[0];
		return true;
	}

	return false;
}

bool BleAdvertisement::limitedDiscoverable() {
	return isFlagSet(adv_data_flag_t::LimitedDiscoverable);
}

bool BleAdvertisement::generalDiscoverable() {
	return isFlagSet(adv_data_flag_t::GeneralDiscoverable);
}

bool BleAdvertisement::leOnly() {
	return isFlagSet(adv_data_flag_t::LeOnly);
}

bool BleAdvertisement::simulatenousLeBrEdrController() {
	return isFlagSet(adv_data_flag_t::SimulatenousLeBrEdrController);
}

bool BleAdvertisement::simulatenousLeBrEdrHost() {
	return isFlagSet(adv_data_flag_t::SimulatenousLeBrEdrController);
}

bool BleAdvertisement::incompleteList16BitServiceClass(std::string& serviceClass) {
	return getValue(adv_data_type_t::IncompleteList16BitServiceClass, serviceClass);
}

bool BleAdvertisement::incompleteList32BitServiceClass(std::string& serviceClass) {
	return getValue(adv_data_type_t::IncompleteList32BitServiceClass, serviceClass);
}

bool BleAdvertisement::incompleteList128BitServiceClass(std::string& serviceClass) {
	return getValue(adv_data_type_t::IncompleteList128BitServiceClass, serviceClass);
}

bool BleAdvertisement::completeList16BitServiceClass(std::string& serviceClass) {
	return getValue(adv_data_type_t::CompleteList16BitServiceClass, serviceClass);
}

bool BleAdvertisement::completeList32BitServiceClass(std::string& serviceClass) {
	return getValue(adv_data_type_t::CompleteList32BitServiceClass, serviceClass);
}

bool BleAdvertisement::completeList128BitServiceClass(std::string& serviceClass) {
	return getValue(adv_data_type_t::CompleteList128BitServiceClass, serviceClass);
}

bool BleAdvertisement::shortenedLocalName(std::string& name) {
	return getValue(adv_data_type_t::ShortenedLocalName, name);
}

bool BleAdvertisement::completeLocalName(std::string& name) {
	return getValue(adv_data_type_t::CompleteLocalName, name);
}

bool BleAdvertisement::txPowerLevel(std::string& powerLevel) {
	return getValue(adv_data_type_t::TxPowerLevel, powerLevel);
}

bool BleAdvertisement::deviceId(std::string& deviceId) {
	return getValue(adv_data_type_t::DeviceId, deviceId);
}

bool BleAdvertisement::slaveConnectionIntervalRange(std::string& intervalRange) {
	return getValue(adv_data_type_t::SlaveConnectionIntervalRange, intervalRange);
}

bool BleAdvertisement::list16BitServiceSolicitation(std::string& service) {
	return getValue(adv_data_type_t::List16BitServiceSolicitation, service);
}

bool BleAdvertisement::list32BitServiceSolicitation(std::string& service) {
	return getValue(adv_data_type_t::List32BitServiceSolicitation, service);
}

bool BleAdvertisement::list128BitServiceSolicitation(std::string& service) {
	return getValue(adv_data_type_t::List128BitServiceSolicitation, service);
}

bool BleAdvertisement::serviceData16Bit(std::string& serviceData) {
	return getValue(adv_data_type_t::ServiceData16Bit, serviceData);
}

bool BleAdvertisement::serviceData32Bit(std::string& serviceData) {
	return getValue(adv_data_type_t::ServiceData32Bit, serviceData);
}

bool BleAdvertisement::serviceData128Bit(std::string& serviceData) {
	return getValue(adv_data_type_t::ServiceData128Bit, serviceData);
}

bool BleAdvertisement::appearance(std::string& appearance) {
	return getValue(adv_data_type_t::Appearance, appearance);
}

bool BleAdvertisement::publicTargetAddress(std::string& targetAddress) {
	return getValue(adv_data_type_t::PublicTargetAddress, targetAddress);
}

bool BleAdvertisement::advertisingInterval(std::string& advertisingInterval) {
	return getValue(adv_data_type_t::AdvertisingInterval, advertisingInterval);
}

bool BleAdvertisement::manufacturerData(std::string& manufacturerData) {
	return getValue(adv_data_type_t::ManufacturerData, manufacturerData);
}
