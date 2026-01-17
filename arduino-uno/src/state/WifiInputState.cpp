#include "Global.hpp"
#include "State.hpp"
#include "Context.hpp"
#include "Command.hpp"

using namespace global;

void prompt(const char* msg) {
	print_to_screen(msg);
	delay(1000);
}

// TODO: SEND CREDENTIALS TO WIFI CHIP, CHECK IF SUCCESS, THEN STORE IN FLASH IF SO
void WifiInputState::transferCredentials() {
	command::sendCommand("SET_SSID", ctx->ssid.c_str());
	command::sendCommand("SET_PASSWD", ctx->passwd.c_str());
	command::sendCommand("SAVE_WIFI_CREDS");
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
	}
	
	transferCredentials();
	
	ctx->textInputState->prevState = this;
	ctx->textInputState->outputString = outputString;
	ctx->setState(ctx->textInputState);
}

void WifiInputState::handle() { }