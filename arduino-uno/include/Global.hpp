#pragma once
#include <LiquidCrystal.h>
#include <Global.h>
#include "Arduino.h"

namespace global {
	extern const char* ALPHA_STR;
	extern const char* KEYBOARD_STR;
	extern const char* KEYBOARD_UPPERCASE_STR;

	constexpr int MESSAGE_LIFETIME = 250;
	constexpr int INP_BACK_PIN = 7;
	constexpr int INP_LEFT_PIN = 8;
	constexpr int INP_MAIN_PIN = 9;
	constexpr int INP_RIGHT_PIN = 10;

	extern LiquidCrystal lcd;
	extern String strInp;

	void print_to_screen(String& str, int charDelay = 50);
	void print_to_screen(const char* str, int charDelay = 50);

	inline void print_alpha(bool doTransition = true) { print_to_screen(ALPHA_STR, doTransition ? 25 : 0); }
}