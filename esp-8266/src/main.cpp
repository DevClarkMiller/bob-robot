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
// 1. UnitGuid
// 2. BearerToken
// 3. WifiCredentials

struct UnitGuidData: StoredData { int getInfoArrIdx() const override { return 1; } };
struct BearerTokenData: StoredData { int getInfoArrIdx() const override { return 2; } };

int wifi::WifiCredentialsData::getInfoArrIdx() const { return 3; }

size_t initStoredDataAddresses() {
	size_t dataSizes[] = {
		sizeof(UnitGuidData),
		sizeof(BearerTokenData),
		sizeof(wifi::WifiCredentialsData)
	};
	
	int n = sizeof(dataSizes) / sizeof(dataSizes[0]);
	
	storedDataAddrArr[0] = 0;
	for (int i = 1; i < n && i < STORED_DATA_ADDR_ARR_SIZE; i++) {
		storedDataAddrArr[i] = storedDataAddrArr[i - 1] + dataSizes[i - 1];
	}

	// Return the total size needed for EEPROM
	return storedDataAddrArr[n - 1] + dataSizes[n - 1];
}

void initStorage() {
	size_t storedDataSize = initStoredDataAddresses();
	EEPROM.begin(storedDataSize);
}

void setup() {
	using namespace wifi;

	Serial.begin(global::BAUD_RATE);
	delay(250);

	initStorage();

	if (initCredsFromStorage()) connect(creds.ssid, creds.passwd, LOGGING);

	chat::initAPI();

	secureClient.setInsecure(); // WARNING: Insecure, for testing only
	lastPollTime = millis();
}

void loop() {
	handleInput();
	if (chat::canPoll() && millis() - lastPollTime > chat::POLL_INTERVAL) {
		chat::poll(LOGGING);
		lastPollTime = millis();
	}
}