#pragma once
#include <stddef.h>

namespace global {
	constexpr size_t UNIT_NUMBER_BUFF_SIZE = 256;
	constexpr int BAUD_RATE = 115200;
	extern const char* READY;

	void makeReadyToReceive();
}