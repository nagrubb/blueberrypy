#include "BtAdapter.h"
#include <iostream>

#include "bluetooth.h"
#include "hci.h"
#include "hci_lib.h"

using namespace std;
using namespace bluez::native;

BtAdapter::BtAdapter(int id) {
  m_id = id;
  m_hci_device = hci_open_dev(id);

  if (m_hci_device < 0) {
    perror("hci_open_dev()");
    m_active = false;
  } else {
    m_active = true;
    m_reader_thread = new boost::thread(&BtAdapter::processHciData, this);
  }
}

BtAdapter::~BtAdapter() {
  m_active = false;

  if (m_reader_thread) {
    m_reader_thread->join();
  }

  if (hci_close_dev(m_hci_device) < 0) {
    perror("hci_close_dev()");
  }
}

bool BtAdapter::enableScanning() {
  if (!setScanParameters()) {
    return false;
  }

  return setScanEnable(true, false);
}

bool BtAdapter::disableScanning() {
  return setScanEnable(false, false);
}

bool BtAdapter::setScanParameters() {
  le_set_scan_parameters_cp scan_parameters;

  memset(&scan_parameters, 0, sizeof(scan_parameters));
  scan_parameters.type = 0x01; //active scanning
  scan_parameters.interval = htobs(0x0012);
  scan_parameters.window = htobs(0x0012);
  scan_parameters.own_bdaddr_type = 0; //Public
  scan_parameters.filter = 0; //No filter

  if (hci_send_cmd(m_hci_device, OGF_LE_CTL, OCF_LE_SET_SCAN_PARAMETERS, LE_SET_SCAN_PARAMETERS_CP_SIZE, (void*) &scan_parameters) < 0) {
    perror("hci_send_cmd(OCF_LE_SET_SCAN_ENABLE)");
    return false;
  }

  return true;
}

bool BtAdapter::setScanEnable(bool enable, bool filterDuplicates) {
  le_set_scan_enable_cp scan_enable_cp;

  scan_enable_cp.enable = enable ? 1 : 0;
  scan_enable_cp.filter_dup = filterDuplicates ? 1 : 0;

  if (hci_send_cmd(m_hci_device, OGF_LE_CTL, OCF_LE_SET_SCAN_ENABLE, LE_SET_SCAN_ENABLE_CP_SIZE, (void*) &scan_enable_cp) < 0) {
    perror("hci_send_cmd(OCF_LE_SET_SCAN_ENABLE)");
    return false;
  }

  return true;
}

void BtAdapter::processHciData() {
  int hci_event_len = 0;
  evt_le_meta_event *le_meta_event = NULL;
  le_advertising_info *adv_info = NULL;
  unsigned char hci_event_buf[HCI_MAX_EVENT_SIZE];
	hci_filter filter;
  fd_set rfds;
  timeval tv;
  int select_ret;

	// setup HCI filter
	hci_filter_clear(&filter);
	hci_filter_set_ptype(HCI_EVENT_PKT, &filter);
	hci_filter_set_event(EVT_LE_META_EVENT, &filter);
	setsockopt(m_hci_device, SOL_HCI, HCI_FILTER, &filter, sizeof(filter));

	while(m_active) {

		FD_ZERO(&rfds);
		FD_SET(m_hci_device, &rfds);

		tv.tv_sec = 1;
		tv.tv_usec = 0;

    select_ret = select(m_hci_device + 1, &rfds, NULL, NULL, &tv);

		if (select_ret == -1) {
			continue;
		}

    if (select_ret) {
  		// read HCI event
  		hci_event_len = read(m_hci_device, hci_event_buf, sizeof(hci_event_buf));

  		le_meta_event = (evt_le_meta_event *)(hci_event_buf + (1 + HCI_EVENT_HDR_SIZE));
  		hci_event_len -= (1 + HCI_EVENT_HDR_SIZE);

  		if (le_meta_event->subevent != EVT_LE_ADVERTISING_REPORT) {
  			continue;
  		}

  		adv_info = (le_advertising_info *)(le_meta_event->data + 1);
  		BleAdvertisement* advertisment = BleAdvertisement::parse(adv_info);
      if (!onAdvertisementScanned(advertisment)) {
        delete advertisment;
      }
    }
  }
}
