#include "globals.hpp"
#include "io.hpp"
#include "command.hpp"

namespace io {
	size_t storedDataAddrArr[STORED_DATA_ADDR_ARR_SIZE];

	bool StoredData::isStored() { return magic == RAW_DATA_HEALTH_ID; }
	void StoredData::init() { magic = RAW_DATA_HEALTH_ID; }

	void copySerialInfoBuffer(char* buffer, size_t buffsize) {
		int i = 0;
		while (Serial.available() > 0 && i < (int)buffsize - 1) {
			char c = Serial.read();
			if (c == '\n' || c == '\r') break;
			buffer[i++] = c;
		}
		buffer[i] = '\0'; // Null-terminate
	}

	void handleInput() {
		using namespace command;

		if (Serial.available() == 0) return;
		
		char firstChar = Serial.peek(); // Peek at the first character without removing it
		Serial.println(firstChar);
		// if (firstChar != CMD_START_SYM) return; // Simply return for now

		// Serial.println("Command start detected");

		// Serial.read(); // Remove first char
		// char buffer[CMD_BUFF_SIZE];
		// copySerialInfoBuffer(buffer, CMD_BUFF_SIZE);
		
		// handleCommand(buffer);
		return;

		// Handle other input types
	}
}