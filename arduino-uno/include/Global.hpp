#pragma once
#include "Arduino.h"
#include <LiquidCrystal.h>

extern const char ALPHA_STR[];
extern const char KEYBOARD_STR[];
extern const char KEYBOARD_UPPERCASE_STR[];

#define MESSAGE_LIFETIME 250
#define INP_BACK_PIN 7
#define INP_LEFT_PIN 8
#define INP_MAIN_PIN 9
#define INP_RIGHT_PIN 10

#define READY 0x1

extern LiquidCrystal lcd;
extern String strInp;

// Lets the transmitter know that things are ready
inline void make_ready(){ Serial.print(READY); }

void print_to_screen(String& str, int charDelay = 50);
void print_to_screen(const char* str, int charDelay = 50);

inline void print_alpha(bool doTransition = true) { print_to_screen(ALPHA_STR, doTransition ? 25 : 0); }

inline int wrap(int val, int max) { return (val % max + max) % max; }