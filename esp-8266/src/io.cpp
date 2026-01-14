#include "globals.hpp"
#include "io.hpp"
#include "command.hpp"
#include <Util.h>

namespace io {
	size_t storedDataAddrArr[STORED_DATA_ADDR_ARR_SIZE];

	bool StoredData::isStored() { return magic == RAW_DATA_HEALTH_ID; }
	void StoredData::init() { magic = RAW_DATA_HEALTH_ID; }

	void copySerialInfoBuffer(char* buffer, size_t buffsize) {
		size_t len = Serial.readBytesUntil('\n', buffer, buffsize - 1);
		buffer[len] = '\0';
		if (len > 0 && buffer[len - 1] == '\r') buffer[len - 1] = '\0';
	}

	void handleInput() {
		using namespace command;

		if (Serial.available() == 0) return;
		
		char firstChar = Serial.peek(); // Peek at the first character without removing it

		Serial.println("INPUT");
		
		bool isCmd = firstChar == CMD_START_SYM; 

		if (isCmd) {
			Serial.read(); // Remove first char
			
			char buffer[CMD_BUFF_SIZE];
			copySerialInfoBuffer(buffer, CMD_BUFF_SIZE);
			handleCommand(buffer);
		} else {
			Serial.println("Data received isn't a CMD");
			util::clearSerial();
		}

		// TODO: Handle other input types
	}
}