#pragma once
#include <Global.h>
#include "Arduino.h"

namespace global {
	constexpr std::size_t BAUD_RATE = 115200;

	extern char unitGuid[UNIT_NUMBER_BUFF_SIZE];

	#define LOGGING true	
}