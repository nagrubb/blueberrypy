#pragma once
#include "BtAdapter.h"
#include "GattClient.h"
#include <boost/python.hpp>
#include <string>
#include <sstream>
#include <iostream>

using namespace boost::python;

enum class AttErrorCode : uint8_t {
  NoError = 0x00,
  InvalidHandle = 0x01,
  ReadNotPermitted = 0x02,
  WriteNotPermitted = 0x03,
  InvalidPdu = 0x04,
  Authentication = 0x05,
  RequestNotSupported = 0x06,
  InvalidOffset = 0x07,
  Authorization = 0x08,
  PrepareQueueFull = 0x09,
  AttributeNotFound = 0x0A,
  AttributeNotLong = 0x0B,
  InsufficientEncryptionKeySize = 0x0C,
  InvalidAttributeValueLength = 0x0D,
  Unlikely = 0x0E,
  InsufficientEncryption = 0x0F,
  UnsupportedGroupType = 0x10,
  InsufficientResources = 0x11
};

struct BleAdvertisement {
  BleAdvertisement() {
    throw;
  }

  BleAdvertisement(bluez::native::BleAdvertisement* advertisement) {
      m_advertisement = advertisement;
  }

  ~BleAdvertisement() {
      delete m_advertisement;
  }

  boost::python::object rssi() {
    return boost::python::object(m_advertisement->rssi());
  }

  boost::python::object addressType() {
    return boost::python::object(m_advertisement->addressType());
  }

  boost::python::object btAddress() {
    return boost::python::object(m_advertisement->btAddress());
  }

  bool hasFlags() {
    return m_advertisement->hasFlags();
  }

  boost::python::object rawFlags() {
    uint8_t flags;
    if (!m_advertisement->rawFlags(flags)) {
        return boost::python::object();
    }

    return boost::python::object(flags);
  }

  boost::python::object limitedDiscoverable() {
    if (!m_advertisement->hasFlags()) {
      return boost::python::object();
    }

    return boost::python::object(m_advertisement->limitedDiscoverable());
  }

  boost::python::object generalDiscoverable() {
    if (!m_advertisement->hasFlags()) {
      return boost::python::object();
    }

    return boost::python::object(m_advertisement->generalDiscoverable());
  }

  boost::python::object leOnly() {
    if (!m_advertisement->hasFlags()) {
      return boost::python::object();
    }

    return boost::python::object(m_advertisement->leOnly());
  }

  boost::python::object simulatenousLeBrEdrController() {
    if (!m_advertisement->hasFlags()) {
      return boost::python::object();
    }

    return boost::python::object(m_advertisement->simulatenousLeBrEdrController());
  }

  boost::python::object simulatenousLeBrEdrHost() {
    if (!m_advertisement->hasFlags()) {
      return boost::python::object();
    }

    return boost::python::object(m_advertisement->simulatenousLeBrEdrHost());
  }

  boost::python::object incompleteList16BitServiceClass() {
    return convert(&bluez::native::BleAdvertisement::incompleteList16BitServiceClass);
  }

  boost::python::object incompleteList32BitServiceClass() {
    return convert(&bluez::native::BleAdvertisement::incompleteList32BitServiceClass);
  }

  boost::python::object incompleteList128BitServiceClass() {
    return convert(&bluez::native::BleAdvertisement::incompleteList128BitServiceClass);
  }

  boost::python::object completeList16BitServiceClass() {
    return convert(&bluez::native::BleAdvertisement::completeList16BitServiceClass);
  }

  boost::python::object completeList32BitServiceClass() {
    return convert(&bluez::native::BleAdvertisement::completeList32BitServiceClass);
  }

  boost::python::object completeList128BitServiceClass() {
    return convert(&bluez::native::BleAdvertisement::completeList128BitServiceClass);
  }

  boost::python::object shortenedLocalName() {
    return convert(&bluez::native::BleAdvertisement::shortenedLocalName);
  }

  boost::python::object completeLocalName() {
    return convert(&bluez::native::BleAdvertisement::completeLocalName);
  }

  boost::python::object txPowerLevel() {
    return convert(&bluez::native::BleAdvertisement::txPowerLevel);
  }

  boost::python::object deviceId() {
    return convert(&bluez::native::BleAdvertisement::deviceId);
  }

  boost::python::object slaveConnectionIntervalRange() {
    return convert(&bluez::native::BleAdvertisement::slaveConnectionIntervalRange);
  }

  boost::python::object list16BitServiceSolicitation() {
    return convert(&bluez::native::BleAdvertisement::list16BitServiceSolicitation);
  }

  boost::python::object list32BitServiceSolicitation() {
    return convert(&bluez::native::BleAdvertisement::list32BitServiceSolicitation);
  }

  boost::python::object list128BitServiceSolicitation() {
    return convert(&bluez::native::BleAdvertisement::list128BitServiceSolicitation);
  }

  boost::python::object serviceData16Bit() {
    return convert(&bluez::native::BleAdvertisement::serviceData16Bit);
  }

  boost::python::object serviceData32Bit() {
    return convert(&bluez::native::BleAdvertisement::serviceData32Bit);
  }

  boost::python::object serviceData128Bit() {
    return convert(&bluez::native::BleAdvertisement::serviceData128Bit);
  }

  boost::python::object appearance() {
    return convert(&bluez::native::BleAdvertisement::appearance);
  }

  boost::python::object publicTargetAddress() {
    return convert(&bluez::native::BleAdvertisement::publicTargetAddress);
  }

  boost::python::object advertisingInterval() {
    return convert(&bluez::native::BleAdvertisement::advertisingInterval);
  }

  boost::python::object manufacturerData() {
    return convert(&bluez::native::BleAdvertisement::manufacturerData);
  }

  boost::python::object convert(bool (bluez::native::BleAdvertisement::*method)(std::string&)) {
    std::string tmp;

    if (!(*m_advertisement.*method)(tmp)) {
      return boost::python::object();
    }

    return boost::python::object(tmp);
  }


  bluez::native::BleAdvertisement* m_advertisement;
};

struct BtAdapter : public bluez::native::BtAdapter {
    BtAdapter(int id, PyObject* pyCallback) : bluez::native::BtAdapter(id), m_pyCallback(pyCallback) {
        PyEval_InitThreads();
    }

    virtual bool onAdvertisementScanned(bluez::native::BleAdvertisement* advertisment) {
      BleAdvertisement* wrapper = new BleAdvertisement(advertisment);

  		PyGILState_STATE gstate;
  		gstate = PyGILState_Ensure();
  		call_method<void>(m_pyCallback, "onAdvertisementScanned", wrapper);
      PyGILState_Release(gstate);
      return true;
    }

    PyObject* const m_pyCallback;
};

struct GattDescriptor {
  GattDescriptor() {
    throw;
  }

  GattDescriptor(bluez::native::GattDescriptor *descriptor) {
    m_descriptor = descriptor;
  }

  ~GattDescriptor() {
    //don't delete m_service as it doesn't belong to us. This
    //is a wrapper object that doesn't own the underlying object.
  }

  boost::python::object getUuid() {
    return boost::python::object(m_descriptor->getUuid());
  }

  boost::python::object getHandle() {
    return boost::python::object(m_descriptor->getHandle());
  }

  bluez::native::GattDescriptor* m_descriptor;
};

struct GattCharacteristic : bluez::native::IGattCharacteristicCallback {
  GattCharacteristic() {
    throw;
  }

  GattCharacteristic(bluez::native::GattCharacteristic* characteristic) :
    m_characteristic(characteristic),
    m_pyCallback(NULL) {
  }

  ~GattCharacteristic() {
    //don't delete m_service as it doesn't belong to us. This
    //is a wrapper object that doesn't own the underlying object.
  }

  boost::python::object getHandle() {
    return boost::python::object(m_characteristic->getHandle());
  }

  boost::python::object getValueHandle() {
    return boost::python::object(m_characteristic->getValueHandle());
  }

  boost::python::object getProperties() {
    return boost::python::object(m_characteristic->getProperties());
  }

  boost::python::object getUuid() {
    return boost::python::object(m_characteristic->getUuid());
  }

  boost::python::list getDescriptors() {
    boost::python::list list;

    for (auto i = m_characteristic->DescriptorCollectionBegin(); i != m_characteristic->DescriptorCollectionEnd(); ++i) {
      GattDescriptor* wrapper = new GattDescriptor(*i);
      list.append(wrapper);
    }

    return list;
  }

  void bind(PyObject* pyCallback) {
    m_characteristic->bind(this);
    m_pyCallback = pyCallback;
  }

  void unbind() {
    m_characteristic->unbind();
    m_pyCallback = NULL;
  }

  bool read() {
    return m_characteristic->read();
  }

  bool write(std::string data, bool writeWithResponse = false, bool signedWrite = false) {
    return m_characteristic->write(data, writeWithResponse, signedWrite);
  }

  bool registerNotify() {
    return m_characteristic->registerNotify();
  }

  bool unregisterNotify() {
    return m_characteristic->registerNotify();
  }

  /* IGattCharacteristicCallback Interface Ipmlementation */
  virtual void onReadResponse(bool success, uint8_t attErrorCode, std::string value) {
    PyGILState_STATE gstate;
    gstate = PyGILState_Ensure();
    call_method<void>(m_pyCallback, "onReadResponse", success, (AttErrorCode) attErrorCode, value);
    PyGILState_Release(gstate);
  }

  virtual void onWriteResponse(bool success, uint8_t attErrorCode) {
    PyGILState_STATE gstate;
    gstate = PyGILState_Ensure();
    call_method<void>(m_pyCallback, "onWriteResponse", success, (AttErrorCode)attErrorCode);
    PyGILState_Release(gstate);
  }

  virtual void onRegistration(uint16_t attErrorCode) {
    PyGILState_STATE gstate;
    gstate = PyGILState_Ensure();
    call_method<void>(m_pyCallback, "onRegistration", (AttErrorCode)attErrorCode);
    PyGILState_Release(gstate);
  }

  virtual void onNotification(std::string value) {
    PyGILState_STATE gstate;
    gstate = PyGILState_Ensure();
    call_method<void>(m_pyCallback, "onNotification", value);
    PyGILState_Release(gstate);
  }
  /* IGattCharacteristicCallback Interface End */

  bluez::native::GattCharacteristic* m_characteristic;
  PyObject* m_pyCallback;
};

struct GattService {
  GattService() {
    throw;
  }

  GattService(bluez::native::GattService* service) {
    m_service = service;
  }

  ~GattService() {
    //don't delete m_service as it doesn't belong to us. This
    //is a wrapper object that doesn't own the underlying object.
  }

  boost::python::object getStartHandle() {
    return boost::python::object(m_service->getStartHandle());
  }

  boost::python::object getEndHandle() {
    return boost::python::object(m_service->getEndHandle());
  }

  boost::python::object getPrimary() {
    return boost::python::object(m_service->getPrimary());
  }

  boost::python::object getUuid() {
    return boost::python::object(m_service->getUuid());
  }

  boost::python::list getCharacteristics() {
    boost::python::list list;

    for (auto i = m_service->CharacteristicCollectionBegin(); i != m_service->CharacteristicCollectionEnd(); ++i) {
      GattCharacteristic* wrapper = new GattCharacteristic(*i);
      list.append(wrapper);
    }

    return list;
  }

  bluez::native::GattService* m_service;
};

struct GattClient : bluez::native::GattClient {
  GattClient(PyObject* pyCallback) : bluez::native::GattClient(), m_pyCallback(pyCallback) {
    PyEval_InitThreads();
  }

  GattClient(PyObject* pyCallback, uint16_t mtu) : bluez::native::GattClient(mtu), m_pyCallback(pyCallback) {
    PyEval_InitThreads();
  }

  ~GattClient() {}

  virtual void onServicesDiscovered(bool success, uint8_t attErrorCode) {
    PyGILState_STATE gstate;
    gstate = PyGILState_Ensure();
    call_method<void>(m_pyCallback, "onServicesDiscovered", success, (AttErrorCode) attErrorCode);
    PyGILState_Release(gstate);
  }

  boost::python::list getServices() {
    boost::python::list list;

    for (auto i = ServiceCollectionBegin(); i != ServiceCollectionEnd(); ++i) {
      GattService* wrapper = new GattService(*i);
      list.append(wrapper);
    }

    return list;
  }

  PyObject* const m_pyCallback;
};
