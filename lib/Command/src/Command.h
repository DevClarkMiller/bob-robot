#pragma once
#include <cstddef>

namespace command {
	constexpr std::size_t CMD_BUFF_SIZE = 256;
	constexpr std::size_t CMD_NAME_BUFF_SIZE = 32;
	constexpr char CMD_DELIM = ':';
	constexpr char CMD_START_SYM = '$';

	void sendCommand(const char* cmdName, const char* payload);
}