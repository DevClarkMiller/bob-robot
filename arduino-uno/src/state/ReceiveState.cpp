#include "Global.hpp"
#include "State.hpp"
#include "Context.hpp"

void ReceiveState::onBackHold() {
	ctx->setState(ctx->menuState);
	ctx->start();
}

void ReceiveState::start() {
	print_to_screen("Listening for messages...");
}

void ReceiveState::handle() {
	if (Serial.available() > 0) {
    	ctx->setState(ctx->printState);
	}
}