#pragma once
#include "Arduino.h"

namespace io {
	void copySerialInfoBuffer(char* buffer, size_t buffsize);

	struct StoredData {
		bool isStored();
		void init();
	private:
		uint32_t magic; // Number which verifies if the struct is valid
	};
}