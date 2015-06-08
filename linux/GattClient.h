#include <string>
#include "MainLoop.h"

namespace bluez {
namespace native {
class GattClient {
public:
  GattClient(std::string btAddress);
  virtual ~GattClient();

  bool connect();
  bool disconnect();
private:
  MainLoop& m_mainLoop;
  std::string m_btAddress;
  bool m_connected;
  int m_socket;
};
} //native
} //bluez
