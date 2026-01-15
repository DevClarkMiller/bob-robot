#pragma once
#include "State.hpp"

class Context {
public:
	struct ButtonState {
		bool isPressed = false;
		bool isClicked = false;
		bool holdTriggered = false;
		unsigned long pressStartTime = 0;
		unsigned long lastClickTime = 0;
	};

	// The main state
	State* state;
	// Handles receiving input from serial and its handle method is called every loop along with the other states context
	State* receiverState; 

	ReceiveState* receiveState;
	PrintState* printState;
	WifiInputState* wifiInputState;
	TextInputState* textInputState;
	MessageInputState* messageInputState;
	MenuState* menuState;

	String ssid;
	String passwd;

	Context(
		ReceiveState* receiveState,
		PrintState* printState,
		WifiInputState* wifiInputState,
		TextInputState* textInputState,
		MessageInputState* messageInputState,
		MenuState* menuState
	);

	void setState(State* s) { 
		state = s;
		state->start(); 
	}

	void start() { state->start(); }
	void handle();
	void initMenu();

private:
	ButtonState backButtonState;
	ButtonState leftButtonState;
	ButtonState actionButtonState;
	ButtonState rightButtonState;
};