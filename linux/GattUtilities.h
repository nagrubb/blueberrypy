#pragma once

#include <ostream>
#include <iostream>

extern "C" {
  #include "bluetooth.h"
  #include "uuid.h"
}

std::ostream& operator<<(std::ostream& os, const bt_uuid_t uuid);
