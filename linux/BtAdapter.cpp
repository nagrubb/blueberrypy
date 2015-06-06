#include "BtAdapter.h"
#include <iostream>

#include "bluetooth.h"
#include "hci.h"
#include "hci_lib.h"

using namespace std;
using namespace bluez::native;

BtAdapter::BtAdapter(int id) {
  cout << "bluez::native::BtAdapter::BtAdapter()" << endl;

  m_id = id;
  m_hci_device = hci_open_dev(id);

  if (m_hci_device < 0) {
    perror("hci_open_dev()");
  }
}

BtAdapter::~BtAdapter() {
  cout << "bluez::native::BtAdapter::~BtAdapter()" << endl;

  if (hci_close_dev(m_hci_device) < 0) {
    perror("hci_close_dev()");
  }

}

bool BtAdapter::enableScanning() {
  if (hci_le_set_scan_enable(m_hci_device, 0x01, 0x00, 1000) < 0) {
    perror("hci_le_set_scan_enable()");
    return false;
  }

  return true;
}

bool BtAdapter::disableScanning() {
  if (hci_le_set_scan_enable(m_hci_device, 0x01, 0x00, 1000) < 0) {
    perror("hci_le_set_scan_enable()");
    return false;
  }

  return true;
}
