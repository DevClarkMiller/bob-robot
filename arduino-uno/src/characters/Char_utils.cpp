#include "Global.hpp"
#include "Characters.hpp"

using namespace global;

// Create each char for the LCD 
void init_characters() {
	lcd.createChar((int)CharacterLocation::Bell, bell);
	lcd.createChar((int)CharacterLocation::Connect, connect);
	lcd.createChar((int)CharacterLocation::Question_Mark, question_mark);
	lcd.createChar((int)CharacterLocation::Speaker, speaker);
	lcd.createChar((int)CharacterLocation::Right_Arrow, right_arrow);
}