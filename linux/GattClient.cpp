#include "GattClient.h"
#include <sys/socket.h>

#include "bluetooth.h"
#include "l2cap.h"

#define ATT_CID 4
#define INVALID_SOCKET -1

using namespace std;
using namespace bluez::native;

GattClient::GattClient(string btAddress) :
  m_mainLoop(MainLoop::getInstance()),
  m_btAddress(btAddress),
  m_connected(false) {

  m_mainLoop.ref();
}

GattClient::~GattClient() {
  cout << "bluez::native::GattClient::~GattClient" << endl;
  m_mainLoop.unref();
}

bool GattClient::connect() {
  bdaddr_t srcAddress = {{0, 0, 0, 0, 0, 0}};
  sockaddr_l2 srcSocketAddress;
  bdaddr_t dstAddress;
  sockaddr_l2 dstSocketAddress;
  uint8_t dst_type = BDADDR_LE_PUBLIC;
  int sec = BT_SECURITY_LOW;
	bt_security btsec;

  if (str2ba(m_btAddress.c_str(), &dstAddress) < 0) {
    return false;
  }

  {
		char srcaddr_str[18], dstaddr_str[18];

		ba2str(&srcAddress, srcaddr_str);
		ba2str(&dstAddress, dstaddr_str);

		cout << "Opening L2CAP LE connection on ATT channel" << endl;
    cout << "Source: " << srcaddr_str << " Destination: " << dstaddr_str << endl;
	}

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

	cout << "Connecting to device..." << endl;
	fflush(stdout);

	if (::connect(m_socket, (struct sockaddr *) &dstSocketAddress, sizeof(dstSocketAddress)) < 0) {
		perror("connect()");
		close(m_socket);
		return false;
	}

	cout << "Done" << endl;
  m_connected = true;

	return true;
}

bool GattClient::disconnect() {
  if (!m_connected) {
    cout << "disconnect() called, but not connected" << endl;
    return true;
  }

  int socketToBeClosed = __sync_val_compare_and_swap(&m_socket, m_socket, INVALID_SOCKET);
  if (socketToBeClosed != INVALID_SOCKET) {
    if (close(socketToBeClosed) != 0) {
      //socketToBeClosed is leaked at this point.
      perror("close()");
      return false;
    }
  }


  return true;
}
