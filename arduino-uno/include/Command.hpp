#pragma once
#include <Command.h>

namespace command {
	constexpr size_t POLL_INTERVAL = 5000;

	extern unsigned long lastPollTime;

	bool canPoll();
	// Send a poll command to the ESP-8266
	void poll();

	void handleCommand(char buffer[CMD_BUFF_SIZE]);
	void sendChat(char buffer[CMD_BUFF_SIZE]);
} 