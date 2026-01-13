#include "Io.h"

namespace io {
	void copySerialInfoBuffer(char* buffer, size_t buffsize) {
		int i = 0;
		while (Serial.available() > 0 && i < (int)buffsize - 1) {
			char c = Serial.read();
			if (c == '\n' || c == '\r') break;
			buffer[i++] = c;
		}
		buffer[i] = '\0'; // Null-terminate
	}
}