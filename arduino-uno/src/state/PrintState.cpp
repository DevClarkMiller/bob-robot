#include "Global.hpp"
#include "State.hpp"
#include "Context.hpp"

using namespace global;

void PrintState::copyIntoMsgBuffer(char buffer[command::CMD_BUFF_SIZE]) { strcpy(msgBuffer, buffer); }

void PrintState::start() { 
	State::start();
	Serial.println("PrintState Start");

	print_to_screen(msgBuffer);
	delay(1500);
	ctx->setState(prevState);
}

void PrintState::handle() {}