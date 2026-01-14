#pragma once
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <Wifi.h>

#include "io.hpp"

namespace wifi {
	struct WifiCredentialsData: io::StoredData {
		char ssid[34];
		char passwd[65];
	};

	extern WifiCredentials creds;

	extern WiFiClientSecure secureClient;
	extern WiFiClient client;

	bool connect(const char* ssid, const char* passwd, bool log);
	bool beginHttp(HTTPClient& http, const char* url, bool isSecured);
	bool isUrlSecured(const char* url);
	bool initCredsFromStorage();
	void writeCredsToStorage();
}