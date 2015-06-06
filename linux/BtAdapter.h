#pragma once
#include <boost/thread.hpp>

namespace bluez {
namespace native {
  class BtAdapter {
  public:
    BtAdapter(int id);
    ~BtAdapter();

    bool enableScanning();
    bool disableScanning();

  private:
    void processHciData();

    int m_hci_device;
    int m_id;
    bool m_active;
    boost::thread* m_reader_thread;
  };
} //native
} //bluez
