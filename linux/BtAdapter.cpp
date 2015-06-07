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
    m_active = false;
  } else {
    m_active = true;
    m_reader_thread = new boost::thread(&BtAdapter::processHciData, this);
  }
}

BtAdapter::~BtAdapter() {
  m_active = false;

  cout << "bluez::native::BtAdapter::~BtAdapter()" << endl;

  if (hci_close_dev(m_hci_device) < 0) {
    perror("hci_close_dev()");
  }

  if (m_reader_thread) {
    m_reader_thread->interrupt();
    m_reader_thread->join();
  }
}

bool BtAdapter::enableScanning() {
  return setScanEnable(true, false);
}

bool BtAdapter::disableScanning() {
  return setScanEnable(false, false);
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
	hci_filter filter;

	// setup HCI filter
	hci_filter_clear(&filter);
	hci_filter_set_ptype(HCI_EVENT_PKT, &filter);
	hci_filter_set_event(EVT_LE_META_EVENT, &filter);
	setsockopt(m_hci_device, SOL_HCI, HCI_FILTER, &filter, sizeof(filter));

	while(m_active) {
    cout << "m_active is " << m_active << endl;
		fd_set rfds;
		timeval tv;
		int select_ret;
		unsigned char hci_event_buf[HCI_MAX_EVENT_SIZE];
		int hci_event_len = 0;
		evt_le_meta_event *le_meta_event = NULL;
		le_advertising_info *adv_info = NULL;
		uint8_t* adv_data = NULL;
		bool le_adv_part_parsed = false;
		//le_adv_part_t le_adv_part;
		//activator_adv_t activator_adv;

		FD_ZERO(&rfds);
		FD_SET(m_hci_device, &rfds);

		tv.tv_sec = 0;
		tv.tv_usec = 10000;

		select_ret = select(m_hci_device + 1, &rfds, NULL, NULL, &tv);

		if (select_ret == -1) {
      cout << "select_ret" << select_ret << endl;
			continue;
		}

		// read HCI event
		hci_event_len = read(m_hci_device, hci_event_buf, sizeof(hci_event_buf));
		le_meta_event = (evt_le_meta_event *)(hci_event_buf + (1 + HCI_EVENT_HDR_SIZE));
		hci_event_len -= (1 + HCI_EVENT_HDR_SIZE);

		if (le_meta_event->subevent != 0x02) {
      cout << "le_meta_event->subevent" << le_meta_event->subevent << endl;
			continue;
		}

		adv_info = (le_advertising_info *)(le_meta_event->data + 1);
    cout << "Found Advertisment: Parsing..." << endl;
		BleAdvertisement* advertisment = BleAdvertisement::parse(adv_info->data, adv_info->length);
    if (!onAdvertisementScanned(advertisment)) {
      cout << "Freeing advertisment" << endl;
      delete advertisment;
    }

    cout << "Finished parsing..." << endl;

		//activator_adv.bdaddr = &adv_info->bdaddr;
		//activator_adv.rssi = *(adv_info->data + adv_info->length);
  }
}
