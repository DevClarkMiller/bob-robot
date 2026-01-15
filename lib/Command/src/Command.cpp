#include "Command.h"
#include "Arduino.h"

namespace command {
	void sendCommand(const char* cmdName, const char *payload) {
		size_t nameSize = strlen(cmdName); 
		if (nameSize > CMD_NAME_BUFF_SIZE || nameSize + strlen(payload) > CMD_BUFF_SIZE) return; // TODO: MAKE ERROR HAPPEN PROPERLY

		char buffer[CMD_BUFF_SIZE];
		if (payload && payload[0] != '\0') 
			snprintf(buffer, CMD_BUFF_SIZE, "%c%s%c%s", CMD_START_SYM, cmdName, CMD_DELIM, payload);
		else 
			snprintf(buffer, CMD_BUFF_SIZE, "$c%s", CMD_START_SYM, cmdName);

		Serial.println(buffer); // Finally send the finished buffer over serial
	}

	void sendCommand(const char* cmdName) { sendCommand(cmdName, "1"); }

	const Command* getCommand(const char* cmdName, const Command* commands, int n) {
		for (int i = 0; i < n; i++) {
			const Command* cmd = &commands[i];
			if (strcmp(cmd->name, cmdName) == 0) return cmd;
		}

		return nullptr;
	}

	void parseCommand(char* buffer, char* cmdNameBuff) {
		// Find the first delim or end of string
		size_t i = 0;
		while (buffer[i] != CMD_DELIM && buffer[i] != '\0' && i < CMD_NAME_BUFF_SIZE - 1) {
			cmdNameBuff[i] = buffer[i];
			i++;
		}
		cmdNameBuff[i] = '\0';

		// Skip ' ' and ':' (if any)
		size_t j = i;
		if (buffer[j] == ' ' || buffer[j] == CMD_DELIM) j++;

		// Shift the rest of the buffer left
		size_t k = 0;
		while (j < CMD_BUFF_SIZE && buffer[j] != '\0') {
			buffer[k++] = buffer[j++];
		}
		buffer[k] = '\0';
	}
}