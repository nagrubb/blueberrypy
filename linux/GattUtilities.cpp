#include "GattUtilities.h"

using namespace std;

ostream& operator<<(ostream& os, const bt_uuid_t uuid) {
 char uuidStr[MAX_LEN_UUID_STR];
 bt_uuid_to_string(&uuid, uuidStr, sizeof(uuidStr));
 return os << uuidStr;
}

string uuidToString(const bt_uuid_t* uuid) {
  char uuidStr[MAX_LEN_UUID_STR];
  bt_uuid_to_string(uuid, uuidStr, sizeof(uuidStr));
  return string(uuidStr);
}
