#include "Util.h"
#include "Arduino.h"

namespace util {
	void triml(char* str) {
		char* p = str;
		while (isspace((unsigned char)* p)) p++;
		if (p != str) memmove(str, p, strlen(p) + 1);
	}

	void trimr(char* str) {
		// Trim trailing whitespace
		int len = strlen(str);
		while (len > 0 && isspace((unsigned char)str[len - 1])) {
			str[--len] = '\0';
		}
	}

	void trim(char* str) {
		trimr(str);
		triml(str);
	}

	void clearSerial() {
		while (Serial.available()) Serial.read();
	}
}