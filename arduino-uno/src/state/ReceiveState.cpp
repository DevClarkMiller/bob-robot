#include "Global.hpp"
#include "State.hpp"
#include "Context.hpp"
#include "Command.hpp"
#include <Io.h>
#include <Util.h>

using namespace global;

void ReceiveState::onBackHold() {
	ctx->setState(ctx->menuState);
}

void ReceiveState::start() { State::start(); }

void ReceiveState::handle() {
	using namespace command;
	if (Serial.available() == 0) return; 

	char firstChar = Serial.peek(); // Peek at the first character without removing it
			
	bool isCmd = firstChar == CMD_START_SYM; 

	if (isCmd) {
		Serial.read(); // Remove first char
		
		char buffer[CMD_BUFF_SIZE];
		io::copySerialInfoBuffer(buffer, CMD_BUFF_SIZE);
		command::handleCommand(buffer);
	} else util::clearSerial();
}