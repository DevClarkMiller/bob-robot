#include "Global.hpp"

namespace global {
	const char* ALPHA_STR = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	const char* KEYBOARD_STR = "abcdefghijklmnopqrstuvwxyz1234567890";
	const char* KEYBOARD_UPPERCASE_STR = "ABCDEFGHIJKLMNOPQRSTUVWXYZ!@#$%^&*()";

	LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

	template <typename T> void print_to_screen(T& str, int n, int charDelay = 50) {
		lcd.clear(); // Clear screen so that last message is 
		// no longer present
		
		int cursorPos = 0;
		
		if (n <= 16) {
			lcd.print(str);
			return;
		}
			
		for (int i = 0; i < n; i++) {
			lcd.setCursor((i % 16), cursorPos);
			lcd.write(str[i]);
			
			if ((i % 16) == 15 && i + 1 < n) {
				delay(250);
				if (cursorPos == 1) {
					cursorPos = 0;
					lcd.clear();
				} else {
					cursorPos = 1;
				}
			}
			delay(charDelay); // small delay for readability
		}
		
		delay(MESSAGE_LIFETIME);
	}

	void print_to_screen(const String& str, int charDelay) { print_to_screen(str, str.length(), charDelay); }
	void print_to_screen(String& str, int charDelay) { print_to_screen(str, str.length(), charDelay); }
	void print_to_screen(const char* str, int charDelay) { print_to_screen(str, strlen(str), charDelay); };
}