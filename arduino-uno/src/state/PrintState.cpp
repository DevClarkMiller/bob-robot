#include "Global.hpp"
#include "State.hpp"
#include "Context.hpp"

void PrintState::handle() {
  	// wait a bit for the entire message to arrive
    delay(100);
    // read all the available characters
    *msg = Serial.readString(); 
    print_to_screen(*msg);
	// Once message is completely outputted, make system receivable again
    make_ready();
	ctx->setState(ctx->receiveState);
  }