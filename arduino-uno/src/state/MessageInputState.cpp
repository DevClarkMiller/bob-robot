#include "Global.hpp"
#include "State.hpp"
#include "Context.hpp"

// TODO: Send message over wifi chip
void MessageInputState::sendMessage() {
	print_to_screen("Sent message!");
	delay(1000);
}

void MessageInputState::start() {
	State::start();

	if (strInp.length() > 0) { // Means message has been inputted
	 	Serial.println(strInp); // Send message over serial
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