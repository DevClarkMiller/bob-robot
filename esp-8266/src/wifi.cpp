#include "wifi.hpp"
#include "globals.hpp"
#include "EEPROM.h"

namespace wifi {
	WifiCredentials creds;

	WiFiClientSecure secureClient;
	WiFiClient client;

	bool connect(const char* ssid, const char* passwd, bool log) {
		Serial.printf("Attempting to connect to %s", ssid);

		WiFi.begin(ssid, passwd);

		unsigned long startAttemptTime = millis();

		while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < WIFI_TIMEOUT) {
			delay(500);
			Serial.print(".");
		}

		if (WiFi.status() == WL_CONNECTED) {
			if (log) {
				Serial.println("WiFi connected");
				Serial.print("IP address: ");
				Serial.println(WiFi.localIP());
			}
			creds.connected = true;
			return true;
		} else {
			if (log) Serial.println("Failed to connect: check credentials");
			return false;
		}
	}

	bool beginHttp(HTTPClient& http, const char* url, bool isSecured) {
		if (isSecured) return http.begin(secureClient, url);
		return http.begin(client, url);
	}

	bool isUrlSecured(const char* url) {
		const char* IDENTIFIER = "https";
		size_t len = strlen(IDENTIFIER);
		return strlen(url) > len && strncmp(url, IDENTIFIER, len) == 0;
	}

	bool initCredsFromStorage() {
		WifiCredentialsData rawCreds;
		EEPROM.get(0, rawCreds); // TODO: FETCH POS FROM STORAGE

		if (!rawCreds.isStored()) return false;

		strcpy(creds.ssid, rawCreds.ssid);
		strcpy(creds.passwd, rawCreds.passwd);

		return true;
	}

	void writeCredsToStorage() {
		WifiCredentialsData rawCreds;
		rawCreds.init();
		strcpy(rawCreds.ssid, creds.ssid);
		strcpy(rawCreds.passwd, creds.passwd);

		EEPROM.put(0, rawCreds);
		EEPROM.commit();
	}
}