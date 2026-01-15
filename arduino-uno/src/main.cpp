#include "Global.hpp"
#include "Context.hpp"
#include "Characters.hpp"
#include "Command.hpp"

String msg;

ReceiveState receiveState;
PrintState printState(&msg);
WifiInputState wifiInputState;
TextInputState textInputState;
MessageInputState messageInputState;
MenuState menuState;
Context context(&receiveState, &printState, &wifiInputState, &textInputState, &messageInputState, &menuState);

using namespace global;

void initPins() {
	pinMode(INP_BACK_PIN, INPUT_PULLUP);
	pinMode(INP_LEFT_PIN, INPUT_PULLUP); 
	pinMode(INP_MAIN_PIN, INPUT_PULLUP); 
	pinMode(INP_RIGHT_PIN, INPUT_PULLUP); 
}

void setup(){
	Serial.begin(global::BAUD_RATE); // TODO: CHANGE TO CORRECT NUM
	initPins();	

	context.receiverState = context.receiverState;

	context.setState(context.menuState);

	delay(200);
	lcd.begin(16, 2);

	delay(250); // give hardware serial time to stabilize
	init_characters();
	context.initMenu();

	lcd.setCursor(0, 0);
	lcd.print("Waiting...");
	makeReadyToReceive();
	delay(150);

	context.state->start();
	command::lastPollTime = millis();
}

// Calls the current states handle method
void loop() { 
	command::poll();
	context.handle(); 
}