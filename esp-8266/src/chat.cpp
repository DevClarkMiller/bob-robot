#include "chat.hpp"
#include "wifi.hpp"
#include "controller.hpp"

namespace chat {	
	const char* API_URL = "http://192.168.1.203:6512";

	char url[256];
	char bearerToken[512];
	bool isUrlSecure;

	bool isApiInit = false;

	// TODO: FIX THIS
	void initAPI() {
		// snprintf(url, sizeof(url), "%s/Chat?unitGuid=%s", API_URL, UNIT_GUID);
		// snprintf(bearerToken, sizeof(bearerToken), "Bearer %s", API_TOKEN);
		isUrlSecure = wifi::isUrlSecured(url);
	}

	// TODO: STORE STATE ON ARDUINO, INCLUDING IF ITS SENT THE SIG THAT ITS READY TO RECEIVE
	bool canPoll() {
		return wifi::creds.connected && controller::context.canReceive;
	}

	void poll(bool log) {
		HTTPClient http;

		if (!isApiInit) {
			initAPI();
			isApiInit = true;
		}

		if (wifi::beginHttp(http, url, isUrlSecure)) {
			http.addHeader("Authorization", bearerToken);
			int httpCode = http.GET();

			if (httpCode > 0) {
				if (httpCode < 400) {
					String payload = http.getString();
					Serial.println(payload);
				}
			} else {
				Serial.printf("HTTP GET failed, error: %s\n", http.errorToString(httpCode).c_str());
			}
			http.end();
		} else {
			Serial.println("Unable to connect to server");
		}
	}

	void send() {
		
	}
}