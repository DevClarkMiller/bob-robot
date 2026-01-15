#include "globals.hpp"

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include "wifi.hpp"
#include "chat.hpp"
#include "command.hpp"
#include "io.hpp"

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

void setup() {
	Serial.begin(global::BAUD_RATE);
	delay(50);

	Serial.println("Starting ESP-8266...");

	initStorage();

	if (wifi::initCredsFromStorage()) wifi::connect(LOGGING);
	chat::initConfigFromStorage();
	
	wifi::secureClient.setInsecure(); // WARNING: Insecure, for testing only
	Serial.println("Finished Setup!");
}

void loop() {
	handleInput();
	delay(1);
}