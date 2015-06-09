extern "C" {
  #include "bluetooth.h"
  #include "att.h"
  #include "uuid.h"
  #include "gatt-db.h"
  #include "gatt-client.h"
}

#include <string>
#include "MainLoop.h"
#include "GattService.h"
#include <list>

namespace bluez {
namespace native {
class GattClient {
private:
  typedef std::list<GattService*> GattServiceCollection;

public:
  GattClient(std::string btAddress);
  virtual ~GattClient();

  virtual void onServicesDiscovered() {}

  bool connect();
  bool disconnect();

  typedef GattServiceCollection::const_iterator GattServiceIterator;
  GattServiceIterator ServiceCollectionBegin() const { return m_services.begin(); }
  GattServiceIterator ServiceCollectionEnd() const { return m_services.end(); }

private:
  bool initializeAtt();
  void onDisconnected(int err);

  static void _onDisconnected(int err, void* obj);
  static void _onDebugMessage(const char* str, void* obj);
  void onDebugMessage(const char* str);
  static void _onServiceAdded(gatt_db_attribute* attr, void* obj);
  void onServiceAdded(gatt_db_attribute* attr);
  static void _onServiceRemoved(gatt_db_attribute* attr, void* obj);
  void onServiceRemoved(gatt_db_attribute* attr);
  static void _onReady(bool success, uint8_t attErrorCode, void* obj);
  void onReady(bool success, uint8_t attErrorCode);
  static void _onServiceChanged(uint16_t startHandle, uint16_t endHandle, void* obj);
  void onServiceChanged(uint16_t startHandle, uint16_t endHandle);

  static void _createService(gatt_db_attribute* attr, void* obj);
  void createService(gatt_db_attribute* attr);

  MainLoop& m_mainLoop;
  std::string m_btAddress;
  bool m_connected;
  int m_socket;
  bt_att* m_att;
  gatt_db* m_db;
  bt_gatt_client* m_client;
  GattServiceCollection m_services;
};
} //native
} //bluez
