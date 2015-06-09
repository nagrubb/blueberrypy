#pragma once

#include <ostream>
#include <iostream>
#include <string>

extern "C" {
  #include "bluetooth.h"
  #include "uuid.h"
}

std::ostream& operator<<(std::ostream& os, const bt_uuid_t uuid);
std::string uuidToString(const bt_uuid_t* uuid);
