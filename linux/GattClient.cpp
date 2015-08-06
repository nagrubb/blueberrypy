#include "GattClient.h"
#include <sys/socket.h>

#include "bluetooth.h"
#include "l2cap.h"

#define ATT_CID 4
#define INVALID_SOCKET -1

using namespace std;
using namespace bluez::native;

GattClient::GattClient(uint16_t mtu) :
  m_mtu(mtu),
  m_mainLoop(MainLoop::getInstance()),
  m_btAddress(),
  m_connected(false) {

  m_mainLoop.ref();
}

GattClient::~GattClient() {
  m_mainLoop.unref();

  for(auto i = m_services.begin(); i != m_services.end(); ++i) {
    delete *i;
  }

  m_services.clear();
}

bool GattClient::connect(std::string btAddress) {
  bdaddr_t srcAddress = {{0, 0, 0, 0, 0, 0}};
  sockaddr_l2 srcSocketAddress;
  bdaddr_t dstAddress;
  sockaddr_l2 dstSocketAddress;
  uint8_t dst_type = BDADDR_LE_PUBLIC;
  int sec = BT_SECURITY_LOW;
	bt_security btsec;

  m_btAddress = btAddress;
  if (str2ba(m_btAddress.c_str(), &dstAddress) < 0) {
    return false;
  }

	char srcaddr_str[18], dstaddr_str[18];

	ba2str(&srcAddress, srcaddr_str);
	ba2str(&dstAddress, dstaddr_str);

  m_socket = socket(PF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_L2CAP);
	if (m_socket < 0) {
		perror("socket(PF_BLUETOOTH)");
		return false;
	}

	/* Set up source address */
	memset(&srcSocketAddress, 0, sizeof(srcSocketAddress));
  srcSocketAddress.l2_family = AF_BLUETOOTH;
  srcSocketAddress.l2_cid = htobs(ATT_CID);
  srcSocketAddress.l2_bdaddr_type = 0;
	bacpy(&srcSocketAddress.l2_bdaddr, &srcAddress);

	if (bind(m_socket, (struct sockaddr *)&srcSocketAddress, sizeof(srcSocketAddress)) < 0) {
		perror("bind()");
		return false;
	}

	/* Set the security level */
	memset(&btsec, 0, sizeof(btsec));
	btsec.level = sec;
	if (setsockopt(m_socket, SOL_BLUETOOTH, BT_SECURITY, &btsec,
							sizeof(btsec)) != 0) {
		perror("setsockopt(SOL_BLUETOOTH, BT_SECURITY)");
    close(m_socket);
		return false;
	}

	/* Set up destination address */
	memset(&dstSocketAddress, 0, sizeof(dstSocketAddress));
  dstSocketAddress.l2_family = AF_BLUETOOTH;
  dstSocketAddress.l2_cid = htobs(ATT_CID);
  dstSocketAddress.l2_bdaddr_type = dst_type;
	bacpy(&dstSocketAddress.l2_bdaddr, &dstAddress);

	if (::connect(m_socket, (struct sockaddr *) &dstSocketAddress, sizeof(dstSocketAddress)) < 0) {
		perror("connect()");
		close(m_socket);
		return false;
	}

  m_connected = true;

  if (!initializeAtt()) {
    disconnect();
    return false;
  }

	return true;
}

bool GattClient::disconnect() {
  if (!m_connected) {
    cout << "disconnect() called, but not connected" << endl;
    return true;
  }

  m_connected = false;
  int socketToBeClosed = __sync_val_compare_and_swap(&m_socket, m_socket, INVALID_SOCKET);
  if (socketToBeClosed != INVALID_SOCKET) {
    if (close(socketToBeClosed) != 0) {
      //socketToBeClosed could be leaked at this point.
      perror("close()");
      return false;
    }
  }

  return true;
}

bool GattClient::initializeAtt() {
	m_att = bt_att_new(m_socket, false);
	if (!m_att) {
		fprintf(stderr, "Failed to initialze ATT transport layer\n");
		bt_att_unref(m_att);
		return false;
	}

	if (!bt_att_set_close_on_unref(m_att, true)) {
		fprintf(stderr, "Failed to set up ATT transport layer\n");
		bt_att_unref(m_att);
		return false;
	}

	if (!bt_att_register_disconnect(m_att, &GattClient::_onDisconnected, this, NULL)) {
		fprintf(stderr, "Failed to set ATT disconnect handler\n");
		bt_att_unref(m_att);
		return false;
	}

	m_db = gatt_db_new();
	if (!m_db) {
		fprintf(stderr, "Failed to create GATT database\n");
		bt_att_unref(m_att);
		return false;
	}

  m_client = bt_gatt_client_new(m_db, m_att, m_mtu);
	if (!m_client) {
		fprintf(stderr, "Failed to create GATT client\n");
		gatt_db_unref(m_db);
		bt_att_unref(m_att);
		return false;
	}

  bt_att_set_debug(m_att, &GattClient::_onDebugMessage, this, NULL);
	bt_gatt_client_set_debug(m_client, &GattClient::_onDebugMessage, this, NULL);

	gatt_db_register(m_db, &GattClient::_onServiceAdded, &GattClient::_onServiceRemoved, this, NULL);

	bt_gatt_client_set_mtu_exchanged_handler(m_client, &GattClient::_onMtuExchanged, this, NULL);
	bt_gatt_client_set_ready_handler(m_client, &GattClient::_onReady, this, NULL);
	bt_gatt_client_set_service_changed(m_client, &GattClient::_onServiceChanged, this, NULL);

	// bt_gatt_client already holds a reference
	gatt_db_unref(m_db);

	return true;
}

void GattClient::_onDisconnected(int err, void* obj) {
  GattClient* client = static_cast<GattClient*>(obj);
  client->onDisconnected(err);
}

void GattClient::onDisconnected(int err) {}

void GattClient::_onDebugMessage(const char* str, void* obj) {
  GattClient* client = static_cast<GattClient*>(obj);
  client->onDebugMessage(str);
}

void GattClient::_onServiceAdded(gatt_db_attribute *attr, void* obj) {
  GattClient* client = static_cast<GattClient*>(obj);
  client->onServiceAdded(attr);
}

void GattClient::onServiceAdded(gatt_db_attribute *attr) {}

void GattClient::_onServiceRemoved(gatt_db_attribute *attr, void* obj) {
  GattClient* client = static_cast<GattClient*>(obj);
  client->onServiceRemoved(attr);
}

void GattClient::onServiceRemoved(gatt_db_attribute *attr) {}

void GattClient::_onMtuExchanged(bool success, uint8_t attErrorCode, void* obj) {
	GattClient* client = static_cast<GattClient*>(obj);
	client->onMtuExchanged(success, attErrorCode);
}

void GattClient::onMtuExchanged(bool success, uint8_t attErrorCode) {
	onMtuExchanged(success, attErrorCode, bt_att_get_mtu(m_att));
}

void GattClient::_onReady(bool success, uint8_t attErrorCode, void* obj) {
  GattClient* client = static_cast<GattClient*>(obj);
  client->onReady(success, attErrorCode);
}

void GattClient::onReady(bool success, uint8_t attErrorCode) {
  if (success) {
    //enumerate services
    gatt_db_foreach_service(m_db, NULL, &GattClient::_createService, this);
  }

  onServicesDiscovered(success, attErrorCode);
}

void GattClient::_onServiceChanged(uint16_t startHandle, uint16_t endHandle, void* obj) {
  GattClient* client = static_cast<GattClient*>(obj);
  client->onServiceChanged(startHandle, endHandle);
}

void GattClient::onServiceChanged(uint16_t startHandle, uint16_t endHandle) {}

void GattClient::_createService(gatt_db_attribute* attr, void* obj) {
  GattClient* client = static_cast<GattClient*>(obj);
  client->createService(attr);
}

void GattClient::createService(gatt_db_attribute* attr) {
  GattService* service = GattService::create(m_client, attr);

  if (service) {
    m_services.push_back(service);
  }
}
