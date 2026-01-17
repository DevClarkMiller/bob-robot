#pragma once
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <Wifi.h>
#include <Command.h>
#include <Chat.h>

#include "io.hpp"

namespace wifi {
	constexpr size_t REQUEST_SIZE = 64;

	struct WifiCredentialsData: io::StoredData {
		char ssid[34];
		char passwd[65];
	};

	struct RequestOptions {
		bool isUrlSecure = false;

		const uint8_t* body = nullptr;
		size_t bodySize;

		char token[chat::BEARER_TOKEN_SIZE];
		size_t tokenSize;

		bool isAuthorized = false;

		inline void noBody() {
			body = nullptr;
			bodySize = 0;
		}
	};

	typedef void (*RequestCallback)(HTTPClient& http, int httpCode);
	
	enum RequestType {
		GET,
		POST,
		PUT,
		DELETE // Will never really use other ones so IDC!!!!!!
	};

	const char* requestTypeName(RequestType requestType);

	extern WifiCredentials creds;
	extern WiFiClientSecure secureClient;
	extern WiFiClient client;

	bool connect(bool log);
	bool beginHttp(HTTPClient& http, const char* url, bool isSecured);
	bool isUrlSecured(const char* url);

	void request(RequestType requestType, const char* url, const RequestOptions* options, RequestCallback onSuccess = nullptr, RequestCallback onError = nullptr);
	inline void request(RequestType requestType, const char* url, const RequestOptions& options, RequestCallback onSuccess = nullptr, RequestCallback onError = nullptr) {
		request(requestType, url, &options, onSuccess, onError);
	}

	bool initCredsFromStorage();
	void writeCredsToStorage();
	void saveCredentials(char buffer[command::CMD_BUFF_SIZE]);

	void setSSID(char buffer[command::CMD_BUFF_SIZE]);
	void setPASSWD(char buffer[command::CMD_BUFF_SIZE]);
}