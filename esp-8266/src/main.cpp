#include "globals.hpp"

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include "wifi.hpp"
#include "chat.hpp"
#include "command.hpp"
#include "io.hpp"

unsigned long lastPollTime;

using namespace io;

// Order of stored items goes 
// 1. ChatConfig
// 2. WifiCredentials

namespace io {
	StoredDataInfo storedDataInfoArr [] = {
		{ sizeof(chat::ChatConfigData), "ChatConfig" },
		{ sizeof(wifi::WifiCredentialsData), "WifiCredentials" }
	};
}

void initStorage() {
	size_t storedDataSize = initStoredDataAddresses();
	EEPROM.begin(storedDataSize);
}

void setup() {
	Serial.begin(global::BAUD_RATE);
	delay(250);

	Serial.println("Starting ESP-8266...");

	initStorage();

	if (wifi::initCredsFromStorage()) wifi::connect(wifi::creds.ssid, wifi::creds.passwd, LOGGING);
	chat::initAPI();

	// TODO: LOAD CHAT FROM STORAGE

	wifi::secureClient.setInsecure(); // WARNING: Insecure, for testing only
	lastPollTime = millis();
	Serial.println("Finished Setup");
}

void loop() {
	handleInput();
	delay(1);
	if (chat::canPoll() && millis() - lastPollTime > chat::POLL_INTERVAL) {
		chat::poll(LOGGING);
		lastPollTime = millis();
	}
}