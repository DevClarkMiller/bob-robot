#include "Global.hpp"
#include "State.hpp"
#include "Context.hpp"

void prompt(const char* msg) {
	print_to_screen(msg);
	delay(1000);
}

// TODO: SEND CREDENTIALS TO WIFI CHIP, CHECK IF SUCCESS, THEN STORE IN FLASH IF SO
bool WifiInputState::transferCredentials() {
	return true; // PLACEHOLDER
}

void WifiInputState::start() {
	String* outputString = &ctx->ssid;
	// First get SSID
	if (ctx->ssid.length() == 0) { 
		prompt("Enter SSID");
		outputString = &ctx->ssid;
	} else if (ctx->passwd.length() == 0) {
		prompt("Enter Password");
		outputString = &ctx->passwd;
	}else if (transferCredentials()) {
		print_to_screen("Successfully connected to wifi");
		// No go back to main menu
		State::onBackHold();
	}else { // Means transfering credentials failed
		print_to_screen("Failed to connect to wifi");
	}
	
	ctx->textInputState->prevState = this;
	ctx->textInputState->outputString = outputString;
	ctx->setState(ctx->textInputState);
}

void WifiInputState::handle() { }