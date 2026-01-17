#pragma once
#include "Global.hpp"

// Each Character Def goes here

extern byte bell[8];
extern byte connect[8];
extern byte question_mark[8];
extern byte speaker[8];
extern byte right_arrow[8];

enum CharacterLocation {
	Bell = 0,
	Connect,
	Question_Mark,
	Speaker,
	Right_Arrow
};

void init_characters();
inline void print_character(CharacterLocation charLocation) { global::lcd.write(byte(charLocation)); }