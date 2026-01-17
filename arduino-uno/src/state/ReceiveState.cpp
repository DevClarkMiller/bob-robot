#include "Global.hpp"
#include "State.hpp"
#include "Context.hpp"
#include "Command.hpp"
#include <Io.h>

using namespace global;

void ReceiveState::onBackHold() {
	ctx->setState(ctx->menuState);
	ctx->start();
}

void ReceiveState::start() {
	print_to_screen("Listening for messages...");
}


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
	} else {
		Serial.println("Data received isn't a CMD");
		Serial.println(Serial.readString());
	}
}