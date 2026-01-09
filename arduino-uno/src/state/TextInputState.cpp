#include "Global.hpp"
#include "State.hpp"
#include "Context.hpp"

// Max of 32 characters per screen which is the total from each row
#define MAX_SCREEN_CHAR_CNT 32

String strInp;

int TextInputState::getCurrCol() { return (cursorX / 16) % 2; }
int TextInputState::getCurrPage() { return floor(cursorX / MAX_SCREEN_CHAR_CNT); }

const char* TextInputState::getKeyboardStr() { return isUppercase ? KEYBOARD_UPPERCASE_STR : KEYBOARD_STR; }
int TextInputState::getKeyArrSize() { return strlen(getKeyboardStr()); }

void TextInputState::checkPage() {
	if (getCurrPage() != prevPage) printKeyboard(false);
	prevPage = getCurrPage();
}

int TextInputState::getStartXOnCurrPage() {
	return getCurrPage() * MAX_SCREEN_CHAR_CNT;
}

void TextInputState::printKeyboard(bool doTransition) {
	// Print with an offset if the cursorX is above 30
	int startIndex = getStartXOnCurrPage();

	const char* keyboardStr = getKeyboardStr();
	char buff[MAX_SCREEN_CHAR_CNT + 1]; // Add extra char for null terminator

	// Copy over values from the keyboard str into the buff
	int index = 0, n = min(startIndex + MAX_SCREEN_CHAR_CNT, getKeyArrSize());
	for (int i = startIndex; i < n; i++) {
		buff[index++] = keyboardStr[i];
	}

	buff[index] = '\0';

	// Finally print the keyboard given the offset onto the screen
	print_to_screen(buff, doTransition ? 25 : 0);
}

void TextInputState::start() { 
	State::start();
	printKeyboard();
	lcd.blink(); 
	if (outputString == nullptr) outputString = &strInp;
}

void TextInputState::onInputAction() {
	delay(50);
	print_to_screen(*outputString);
	delay(500);
	printKeyboard(false);
}

// Backspace
void TextInputState::onBackClick() {
	int lastIndex = outputString->length() - 1;

	if (lastIndex >= 0) outputString->remove(lastIndex);
	onInputAction();
}

//Erase all
void TextInputState::onBackDoubleClick() {
	*outputString = "";
	onInputAction();
}

// Return to main menu
void TextInputState::onBackHold() {
	prevState = nullptr;
	outputString = &strInp;
	State::onBackHold();
}

void TextInputState::onLeftClick() {
	cursorX = wrap(cursorX - 1, getKeyArrSize());
	checkPage();
}

void TextInputState::onLeftHold() { 
	cursorX = 0; 
	checkPage();
}

void TextInputState::onLeftDoubleClick() {
	cursorX = getStartXOnCurrPage();
}

void TextInputState::onActionClick() {
	*outputString += getKeyboardStr()[cursorX];
	onInputAction();
}

void TextInputState::onActionDoubleClick() {
	this->isUppercase = !this->isUppercase; // Toggle uppercase
	printKeyboard(false);
}

// Submit text
void TextInputState::onActionHold() { ctx->setState(prevState); }

void TextInputState::onRightClick() {
	cursorX = (cursorX + 1) % getKeyArrSize();
	checkPage();
}

void TextInputState::onRightHold() { 
	cursorX = getKeyArrSize() - 1; 
	checkPage();
}

void TextInputState::onRightDoubleClick() {
	cursorX = getStartXOnCurrPage() + MAX_SCREEN_CHAR_CNT - 1;
}

void TextInputState::handle() {
	lcd.setCursor(cursorX % 16, getCurrCol());
}