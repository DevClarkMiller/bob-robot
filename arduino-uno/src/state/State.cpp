#include "State.hpp"
#include "Context.hpp"
#include "Global.hpp"

void State::onBackHold() {
	ctx->setState(ctx->menuState);
	ctx->start();
}

void State::start() {
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.noBlink();
}