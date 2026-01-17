#include "globals.hpp"
#include "chat.hpp"
#include "wifi.hpp"
#include "controller.hpp"
#include "command.hpp"

namespace chat {	
	ChatConfig config;

	// TODO: STORE STATE ON ARDUINO, INCLUDING IF ITS SENT THE SIG THAT ITS READY TO RECEIVE
	bool canPoll() { return wifi::creds.connected; }

	void onPollSuccess(HTTPClient& http, int httpCode) {
		String payload = http.getString();
		command::sendCommand("CHAT", payload.c_str());
	}

	void genericOnError(HTTPClient& http, int httpCode) {
		if (!LOGGING) return;
		if (httpCode > 0) 
			Serial.printf("HTTP Request failed with status: %d\n", httpCode);
		 else 
			Serial.printf("HTTP Request failed, error: %s\n", http.errorToString(httpCode).c_str());
	}

	void onPollError(HTTPClient& http, int httpCode) { genericOnError(http, httpCode); }

	void initUrl(char url[URL_BUFF_SIZE], const char* path) {
		strcpy(url, config.url);
		strcat(url, path);
	}

	void urlEncode(const char* input, char* output, size_t outputSize) {
		size_t outputIdx = 0;
		for (size_t i = 0; input[i] != '\0' && outputIdx < outputSize - 1; i++) {
			char c = input[i];
			if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || 
				(c >= '0' && c <= '9') || c == '-' || c == '_' || c == '.' || c == '~') {
				// Safe characters - no encoding needed
				output[outputIdx++] = c;
			} else if (c == ' ') {
				// Encode space as %20
				if (outputIdx + 3 < outputSize) {
					output[outputIdx++] = '%';
					output[outputIdx++] = '2';
					output[outputIdx++] = '0';
				}
			} else {
				// Encode other characters as %HH
				if (outputIdx + 3 < outputSize) {
					outputIdx += sprintf(&output[outputIdx], "%%%02X", (unsigned char)c);
				}
			}
		}
		output[outputIdx] = '\0';
	}

	void addUrlParam(char* url, const char* name, const char* value, int& paramCount) {
		const char* delim = paramCount > 0 ? "&" : "?";
		char encodedValue[URL_BUFF_SIZE];
		urlEncode(value, encodedValue, URL_BUFF_SIZE);

		strcat(url, delim);
		strcat(url, name);
		strcat(url, "=");
		strcat(url, encodedValue);
		paramCount++;
	}

	void poll() {
		using namespace wifi;

		Serial.println("ENTERED POLL FUNCTION");

		if (!isConfigValid()) { 
			command::sendCommand("CHAT_CONF_INVALID");
			return;
		}

		RequestOptions options;
		options.isUrlSecure = isUrlSecured(config.url);
		strcpy(options.token, config.bearerToken);
		options.isAuthorized = true;

		char url[chat::URL_BUFF_SIZE];
		int paramCount = 0;

		initUrl(url, "/Chat");
		addUrlParam(url, "unitGuid", config.unitGuid, paramCount);

		request(RequestType::GET, url, &options, onPollSuccess, onPollError);
	}

	void onSendSuccess(HTTPClient& http, int httpCode) { command::sendCommand("CHAT_SENT"); }

	void onSendError(HTTPClient& http, int httpCode) { genericOnError(http, httpCode); }

	void send(char buffer[URL_BUFF_SIZE]) {
		using namespace wifi;

		if (!isConfigValid()) { 
			command::sendCommand("CHAT_CONF_INVALID");
			return;
		}

		RequestOptions options;
		options.isUrlSecure = isUrlSecured(config.url);
		strcpy(options.token, config.bearerToken);
		options.isAuthorized = true;
		
		options.noBody();

		char url[chat::URL_BUFF_SIZE];
		int paramCount = 0;

		initUrl(url, "/Chat");
		addUrlParam(url, "unitGuid", config.unitGuid, paramCount);
		addUrlParam(url, "message", buffer, paramCount);

		Serial.println(url);
		Serial.println(buffer);

		request(RequestType::POST, url, &options, onSendSuccess, onSendError);
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