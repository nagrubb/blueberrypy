#pragma once

namespace bluez {
namespace native {
  class BtAdapter {
  public:
    BtAdapter(int id);
    ~BtAdapter();

    bool enableScanning();
    bool disableScanning();

  private:
    int m_hci_device;
    int m_id;
  };
} //native
} //bluez
