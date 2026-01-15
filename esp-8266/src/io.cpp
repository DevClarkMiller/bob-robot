#include "globals.hpp"
#include "io.hpp"
#include "command.hpp"
#include <Util.h>

namespace io {
	size_t storedDataAddrArr[STORED_DATA_SIZE];

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
				
		bool isCmd = firstChar == CMD_START_SYM; 

		if (isCmd) {
			Serial.read(); // Remove first char
			
			char buffer[CMD_BUFF_SIZE];
			copySerialInfoBuffer(buffer, CMD_BUFF_SIZE);
			handleCommand(buffer);
		} else {
			Serial.println("Data received isn't a CMD");
			Serial.println(Serial.readString());
		}

		// TODO: Handle other input types
	}

	size_t getStoredDataAddr(const char* name) {
		for (int i = 0; i < STORED_DATA_SIZE; i++) {
			StoredDataInfo& storedDataInfo = storedDataInfoArr[i];
			if (strcmp(storedDataInfo.name, name) == 0) return storedDataAddrArr[i];
		}

		return -1;
	}

	size_t initStoredDataAddresses() {		
		storedDataAddrArr[0] = 0;
		for (int i = 1; i < STORED_DATA_SIZE && i < STORED_DATA_SIZE; i++) {
			storedDataAddrArr[i] = storedDataAddrArr[i - 1] + storedDataInfoArr[i - 1].size;
		}

		// Return the total size needed for EEPROM
		return storedDataAddrArr[STORED_DATA_SIZE - 1] + storedDataInfoArr[STORED_DATA_SIZE - 1].size;
	}

	void initStorage() {
		size_t storedDataSize = initStoredDataAddresses();
		EEPROM.begin(storedDataSize);
	}
}