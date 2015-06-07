#pragma once
#include <boost/thread.hpp>
#include "BleAdvertisement.h"

namespace bluez {
namespace native {
class BtAdapter {
public:
  BtAdapter(int id);
  virtual ~BtAdapter();

  bool enableScanning();
  bool disableScanning();

protected:
  virtual bool onAdvertisementScanned(BleAdvertisement* advertisment) { return false; }

private:
  bool setScanEnable(bool enable, bool filterDuplicates);
  void processHciData();

  int m_hci_device;
  int m_id;
  bool m_active;
  boost::thread* m_reader_thread;
};
} //native
} //bluez
