#pragma once
#include <stddef.h>

namespace global {
	constexpr size_t UNIT_NUMBER_BUFF_SIZE = 256;
	extern const char* READY;

	void makeReadyToReceive();
}