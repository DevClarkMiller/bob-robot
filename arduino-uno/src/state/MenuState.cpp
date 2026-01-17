#include "Global.hpp"
#include "State.hpp"
#include "Context.hpp"
#include "Characters.hpp"
#include <Util.h>

using namespace global;

struct MenuOption {
	State* state;
	CharacterLocation charLocation;
};

MenuOption menuOptions[] = { 
	{ nullptr, CharacterLocation::Right_Arrow},
	{ nullptr, CharacterLocation::Connect }
};

void Context::initMenu(){
	menuOptions[0].state = this->messageInputState;
	menuOptions[1].state = this->wifiInputState;
}

int numMenuOpts() { return sizeof(menuOptions) / sizeof(MenuOption); }

void MenuState::start() {
	State::start();
	lcd.blink();
	// Print all menu options
	int nOpts = numMenuOpts();
	for (int i = 0; i < nOpts; i++) {
		print_character(menuOptions[i].charLocation);
	}
}

void MenuState::onLeftClick() {
	cursorX = util::wrap(cursorX - 1, numMenuOpts());
}

void MenuState::onLeftHold() { cursorX = 0; }

void MenuState::onActionClick() {
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.noBlink();
	ctx->setState(menuOptions[cursorX].state); 
}

// Submit text
void MenuState::onActionHold() { }

void MenuState::onRightClick() {
	cursorX = (cursorX + 1) % numMenuOpts();
}

void MenuState::onRightHold() { cursorX = numMenuOpts() - 1; }

void MenuState::handle() {
	lcd.setCursor(cursorX % 16, cursorX > 15);
}