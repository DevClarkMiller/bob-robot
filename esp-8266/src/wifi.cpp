#include "wifi.hpp"
#include "globals.hpp"
#include "EEPROM.h"

namespace wifi {
	WifiCredentials creds;

	WiFiClientSecure secureClient;
	WiFiClient client;

	bool connect(bool log) {
		Serial.printf("Attempting to connect to %s", creds.ssid);

		WiFi.begin(creds.ssid, creds.passwd);

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
		size_t storageAddr = io::getStoredDataAddr("WifiCredentials");
		EEPROM.get(storageAddr, rawCreds);

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

		size_t storageAddr = io::getStoredDataAddr("WifiCredentials");
		EEPROM.put(storageAddr, rawCreds);
		EEPROM.commit();
	}

	void setCredentials(char buffer[command::CMD_BUFF_SIZE]) {
		using namespace wifi;
		const char* cmdName = "WIFI_STAT";

		// Pull ssid and passwd buffers from the buffer since we already know their exact size
		char ssidBuff[SSID_BUFF_SIZE];
		char passwdBuff[PASSWD_BUFF_SIZE];

		int j = 0;
		// Copy over the ssid
		for (int i = 0; i < SSID_BUFF_SIZE; i++) 
			ssidBuff[i] = buffer[j++];

		for (int i = 0; i < PASSWD_BUFF_SIZE; i++) 
			passwdBuff[i] = buffer[j++];

		// Copy over the passwd
		strcpy(creds.ssid, ssidBuff);
		strcpy(creds.passwd, passwdBuff);

		if (connect(LOGGING)) {
			writeCredsToStorage(); // Only actually save the creds if they're valid
			command::sendCommand(cmdName, "1");
		} else command::sendCommand(cmdName, "0");
	}
	
	const char* requestTypeName(RequestType requestType) {
		switch (requestType) {
			case RequestType::GET: return "GET";
			case RequestType::POST: return "POST";
			case RequestType::PUT: return "PUT";
			case RequestType::DELETE: return "DELETE";
			default: return nullptr;
		}
	}

	int sendRequest(HTTPClient& http, RequestType requestType, const uint8_t* payload, size_t payloadSize) {
		switch (requestType) {
			case RequestType::GET: return http.GET();
			case RequestType::POST: return http.POST(payload, payloadSize);
			case RequestType::PUT: return http.PUT(payload, payloadSize);
			case RequestType::DELETE: return http.DELETE();
			default: return -1;
		}
	}

	void request(RequestType requestType, const char* url, const RequestOptions* options, RequestCallback onSuccess, RequestCallback onError) {
		HTTPClient http;

		if (wifi::beginHttp(http, url, options->isUrlSecure)) {
			if (options->isAuthorized) http.addHeader("Authorization", options->token);
			int httpCode = sendRequest(http, requestType, options->body, options->bodySize);

			if (httpCode > 0) {
				if (onSuccess != nullptr) onSuccess(http, httpCode);
			} else if (httpCode <= 0 || httpCode >= 400) {
				if (onError != nullptr) onError(http, httpCode);
			}
			http.end();
		} else {
			if (LOGGING) Serial.println("Unable to connect to server");
		}
	}
}