#include "GattUtilities.h"

std::ostream& operator<<(std::ostream& os, const bt_uuid_t uuid) {
 char uuidStr[MAX_LEN_UUID_STR];
 bt_uuid_to_string(&uuid, uuidStr, sizeof(uuidStr));
 return os << uuidStr;
}
