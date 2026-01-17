#include "Command.hpp"
#include "Arduino.h"
#include "Global.hpp"
#include "Context.hpp"

namespace command {
	unsigned long lastPollTime;

	bool canPoll() { return millis() - command::lastPollTime > command::POLL_INTERVAL; }

	void poll() {	
		if (!canPoll()) return;

		lastPollTime = millis();
		sendCommand("POLL_CHAT");
	}

	void onChat(char buffer[CMD_BUFF_SIZE]) {
		context.printState->prevState = context.state;
		context.printState->copyIntoMsgBuffer(buffer);
    	context.setState(context.printState);
	}

	// Sends chat to the ESP-8266 which then sends it to the server
	void sendChat(char buffer[CMD_BUFF_SIZE]) { sendCommand("SEND_CHAT", buffer); }

	const Command COMMANDS[] = {
		{"CHAT", onChat}
	};

	void handleCommand(char buffer[CMD_BUFF_SIZE]) {
		char cmdNameBuff[CMD_NAME_BUFF_SIZE];
		parseCommand(buffer, cmdNameBuff);
		
		int n = sizeof(COMMANDS) / sizeof(Command);
		const Command* cmd = command::getCommand(cmdNameBuff, COMMANDS, n);
		if (cmd != nullptr) cmd->handler(buffer);
		else sendCommand("CMD_404", "1");
	}
}