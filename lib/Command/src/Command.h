#pragma once
#include <stddef.h>

namespace command {
	constexpr size_t CMD_BUFF_SIZE = 256;
	constexpr size_t CMD_NAME_BUFF_SIZE = 32;
	constexpr char CMD_DELIM = ':';
	constexpr char CMD_START_SYM = '$';
	
	typedef void (*CommandHandler)(char buffer[CMD_BUFF_SIZE]);

	struct Command {
		const char* name;
		CommandHandler handler;
	};

	void sendCommand(const char* cmdName, const char* payload);
	void sendCommand(const char* cmdName);
	const Command* getCommand(const char* cmdName, const Command* commands, int n);

	void parseCommand(char* buffer, char* cmdNameBuff);
}