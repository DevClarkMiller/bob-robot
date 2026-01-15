#include "globals.hpp"
#include "chat.hpp"
#include "wifi.hpp"
#include "controller.hpp"
#include "command.hpp"

namespace chat {	
	const char* API_URL = "http://192.168.1.203:6512";
	unsigned long lastPollTime;

	ChatConfig config;

	bool isApiInit = false;

	// TODO: FIX THIS
	bool initAPI() {
		// snprintf(url, sizeof(url), "%s/Chat?unitGuid=%s", API_URL, UNIT_GUID);
		// snprintf(bearerToken, sizeof(bearerToken), "Bearer %s", API_TOKEN);
		config.isUrlSecure = wifi::isUrlSecured(config.url);
		isApiInit = isConfigValid();
		return isApiInit;
	}

	// TODO: STORE STATE ON ARDUINO, INCLUDING IF ITS SENT THE SIG THAT ITS READY TO RECEIVE
	bool canPoll() {
		return wifi::creds.connected 
			&& controller::context.canReceive;
			// && millis() - chat::lastPollTime > chat::POLL_INTERVAL;
	}

	void onPollSuccess(HTTPClient& http, int httpCode) {
		String payload = http.getString();
		Serial.println(payload);
	}

	void onPollError(HTTPClient& http, int httpCode) {
		Serial.printf("HTTP GET failed, error: %s\n", http.errorToString(httpCode).c_str());
	}

	void poll(bool log) {
		using namespace wifi;

		if (!isApiInit && !initAPI()) return;

		RequestOptions options;
		options.token = config.bearerToken;
		options.isAuthorized = true;

		request(RequestType::GET, config.url, &options, onPollSuccess, onPollError);
		
		lastPollTime = millis();
	}

	// TODO: SEND CHAT
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

	void saveChatConfig(char buffer[command::CMD_BUFF_SIZE]) {
		using namespace chat;

		if (!isConfigValid()) { 
			command::sendCommand("CHAT_CONFIG_INVALID", "1");
			return;
		}
	
		writeConfigToStorage();

		command::sendCommand("CHAT_SAVED", "1");
	}
}