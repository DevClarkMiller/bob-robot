#include "Global.hpp"
#include "Context.hpp"

#define HOLD_THRESHOLD_MS 500         // 0.5s for long press
#define DOUBLE_CLICK_THRESHOLD_MS 200 // max 0.20s between clicks

#define DEF_ACTION(name, actionType) \
    void on##name##actionType(State* state) { state->on##name##actionType(); }

typedef void (*actionCallback)(State* state);

DEF_ACTION(Back, Click)
DEF_ACTION(Back, Hold)
DEF_ACTION(Back, DoubleClick)

DEF_ACTION(Left, Click)
DEF_ACTION(Left, Hold)
DEF_ACTION(Left, DoubleClick)

DEF_ACTION(Action, Click)
DEF_ACTION(Action, Hold)
DEF_ACTION(Action, DoubleClick)

DEF_ACTION(Right, Click)
DEF_ACTION(Right, Hold)
DEF_ACTION(Right, DoubleClick)

void checkButtonInput(State* state, Context::ButtonState& buttonState, int pin, actionCallback onClick, actionCallback onHold, actionCallback onDoubleClick) {
	unsigned long now = millis();
	bool pressed = (digitalRead(pin) == LOW);

	if (pressed) {
        if (!buttonState.isPressed) {
            buttonState.isPressed = true;
            buttonState.pressStartTime = now;
        }

        // Hold detection
        if (!buttonState.holdTriggered &&
            (now - buttonState.pressStartTime >= HOLD_THRESHOLD_MS)) {

            onHold(state);

            buttonState.holdTriggered = true;
            buttonState.lastClickTime = 0; // cancel pending click
        }
        return;
    }

	// Check if the button was just clicked, but is unsure if double or single click
	if (buttonState.isClicked && now - buttonState.lastClickTime > DOUBLE_CLICK_THRESHOLD_MS) {
		onClick(state);
		buttonState.isClicked = false;
		buttonState.lastClickTime = 0;
		return;
	}

    if (!buttonState.isPressed)
        return;

    buttonState.isPressed = false;

    // Ignore release after hold
    if (buttonState.holdTriggered) {
        buttonState.holdTriggered = false;
        return;
    }

    // Puts the button state into a state that's essentially "Button is clicked, but we're waiting for a potential double click"
    if (buttonState.lastClickTime == 0) {
        buttonState.lastClickTime = now;
		buttonState.isClicked = true;
        return;
    }

	// Double click is triggered if we click twice within the given window of clicking
	if (now - buttonState.lastClickTime <= DOUBLE_CLICK_THRESHOLD_MS) {
        onDoubleClick(state);
        buttonState.lastClickTime = 0;
		buttonState.isClicked = false;
    }
}

void Context::handle() {
	using namespace global;
	// Get inputs
	checkButtonInput(state, backButtonState, INP_BACK_PIN, onBackClick, onBackHold, onBackDoubleClick);
	checkButtonInput(state, leftButtonState, INP_LEFT_PIN, onLeftClick, onLeftHold, onLeftDoubleClick);
	checkButtonInput(state, actionButtonState, INP_MAIN_PIN, onActionClick, onActionHold, onActionDoubleClick);
	checkButtonInput(state, rightButtonState, INP_RIGHT_PIN, onRightClick, onRightHold, onRightDoubleClick);

	receiveState->handle();
	state->handle(); 
}

Context::Context(
	ReceiveState* receiveState,
	PrintState* printState,
	WifiInputState* wifiInputState,
	TextInputState* textInputState,
	MessageInputState* messageInputState,
	MenuState* menuState
) :
	receiveState(receiveState),
	printState(printState),
	wifiInputState(wifiInputState),
	textInputState(textInputState),
	messageInputState(messageInputState),
	menuState(menuState)
{
	receiveState->setContext(this);
	printState->setContext(this);
	wifiInputState->setContext(this);
	textInputState->setContext(this);
	messageInputState->setContext(this);
	menuState->setContext(this);
}