#include "Command.hpp"
#include "Arduino.h"

namespace command {
	unsigned long lastPollTime;

	bool canPoll() { return millis() - command::lastPollTime > command::POLL_INTERVAL; }

	void poll() {	
		if (!canPoll()) return;

		lastPollTime = millis();
		sendCommand("POLL");
	}
}