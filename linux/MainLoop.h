#pragma once

#include <boost/thread.hpp>

namespace bluez {
namespace native {
class MainLoop {
public:
  static MainLoop& getInstance() {
    if (s_instance == NULL) {
      s_instance = new MainLoop;
    }

    return *s_instance;
  }

  void ref();
  void unref();

  ~MainLoop();
private:
  MainLoop();

  void runner();

  static uint8_t s_refCount;
  static MainLoop* s_instance;
  boost::thread m_thread;
};
} //native
} //bluez
