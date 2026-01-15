#pragma once
#include <Command.h>

namespace command {
	void handleCommand(char buffer[CMD_BUFF_SIZE]);
	void help(char buffer[CMD_BUFF_SIZE]);
}