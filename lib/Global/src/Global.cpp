#include "Global.h"
#include "Arduino.h"

namespace global {
	const char* READY = "$READY:1";

	void makeReadyToReceive() {
		Serial.print(READY);
	}
}