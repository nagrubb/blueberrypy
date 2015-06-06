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
    m_reader_thread->join();
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

void BtAdapter::processHciData() {
	hci_filter filter;

	// setup HCI filter
	hci_filter_clear(&filter);
	hci_filter_set_ptype(HCI_EVENT_PKT, &filter);
	hci_filter_set_event(EVT_LE_META_EVENT, &filter);
	setsockopt(m_hci_device, SOL_HCI, HCI_FILTER, &filter, sizeof(filter));

	while(m_active) {
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

		tv.tv_sec = 1;
		tv.tv_usec = 0;

		select_ret = select(m_hci_device + 1, &rfds, NULL, NULL, &tv);

		if (select_ret == -1) {
			continue;
		}

		// read HCI event
		hci_event_len = read(m_hci_device, hci_event_buf, sizeof(hci_event_buf));
		le_meta_event = (evt_le_meta_event *)(hci_event_buf + (1 + HCI_EVENT_HDR_SIZE));
		hci_event_len -= (1 + HCI_EVENT_HDR_SIZE);

		if (le_meta_event->subevent != 0x02) {
			continue;
		}

		adv_info = (le_advertising_info *)(le_meta_event->data + 1);
    cout << "Found Advertisment...Need to parse it" << endl;
		//memset(&activator_adv, 0, sizeof(activator_adv));
		//activator_adv.bdaddr = &adv_info->bdaddr;
		//activator_adv.rssi = *(adv_info->data + adv_info->length);
		//adv_data = adv_info->data;
  }
}
