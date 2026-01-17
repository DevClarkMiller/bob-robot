#include "Global.hpp"
#include "State.hpp"
#include "Context.hpp"
#include "Command.hpp"

using namespace global;

// TODO: Send message over wifi chip
void MessageInputState::sendMessage() {
	char buffer[command::CMD_BUFF_SIZE];
	strcpy(buffer, strInp.c_str());

	command::sendChat(buffer);
	print_to_screen("Chat Sent!");
	delay(1000);
}

void MessageInputState::start() {
	State::start();

	if (strInp.length() > 0) { // Means message has been inputted
		print_to_screen(strInp);
		delay(500);
		sendMessage();

	 	strInp = ""; // Reset str inp
		// Continue to send messages
	}

	// We gotta switch to the message input state
	ctx->textInputState->prevState = this;
	delay(50);
	ctx->setState(ctx->textInputState);
}

void MessageInputState::handle() {}