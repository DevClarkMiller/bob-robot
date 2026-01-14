#include "chat.hpp"
#include "wifi.hpp"
#include "controller.hpp"

namespace chat {	
	const char* API_URL = "http://192.168.1.203:6512";

	ChatConfig config;

	bool isApiInit = false;

	// TODO: FIX THIS
	void initAPI() {
		// snprintf(url, sizeof(url), "%s/Chat?unitGuid=%s", API_URL, UNIT_GUID);
		// snprintf(bearerToken, sizeof(bearerToken), "Bearer %s", API_TOKEN);
		config.isUrlSecure = wifi::isUrlSecured(config.url);
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

		if (wifi::beginHttp(http, config.url, config.isUrlSecure)) {
			http.addHeader("Authorization", config.bearerToken);
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

	bool isConfigValid() {
		return config.url[0] != '\0' && 
			config.bearerToken[0] != '\0' && 
			config.unitGuid[0] != '\0';
	}

	bool initConfigFromStorage() {
		ChatConfigData configData;
		size_t storageAddr = io::getStoredDataAddr("ChatConfig");
		EEPROM.get(storageAddr, configData);

		if (!configData.isStored()) return false;

		strcpy(config.bearerToken, configData.bearerToken);
		strcpy(config.url, configData.url);
		strcpy(config.unitGuid, configData.unitGuid);

		return true;
	}

	void writeConfigToStorage() {
		ChatConfigData configData;
		configData.init();

		strcpy(configData.bearerToken, config.bearerToken);
		strcpy(configData.url, config.url);
		strcpy(configData.unitGuid, config.unitGuid);

		size_t storageAddr = io::getStoredDataAddr("ChatConfig");
		EEPROM.put(storageAddr, configData);
		EEPROM.commit();
	}
}