#include "Io.h"

namespace io {
	void copySerialInfoBuffer(char* buffer, size_t buffsize) {
		size_t len = Serial.readBytesUntil('\n', buffer, buffsize - 1);
		buffer[len] = '\0';
		if (len > 0 && buffer[len - 1] == '\r') buffer[len - 1] = '\0';
	}
}