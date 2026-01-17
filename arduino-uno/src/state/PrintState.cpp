#include "Global.hpp"
#include "State.hpp"
#include "Context.hpp"

using namespace global;

void PrintState::copyIntoMsgBuffer(char buffer[command::CMD_BUFF_SIZE]) { strcpy(msgBuffer, buffer); }

void PrintState::start() { }

void PrintState::handle() {
    // read all the available characters
    print_to_screen(msgBuffer);
	ctx->setState(prevState);
}