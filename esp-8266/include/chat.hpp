#pragma once
#include <Global.h>
#include <Chat.h>
#include <Io.h>
#include <Command.h>

namespace chat {
	constexpr size_t POLL_INTERVAL = 5000;
	constexpr size_t PAYLOAD_SIZE = 128;

	struct ChatConfig {
		char url[URL_BUFF_SIZE];
		char bearerToken[BEARER_TOKEN_SIZE];
		char unitGuid[global::UNIT_NUMBER_BUFF_SIZE];
		bool isUrlSecure;
	};

	struct ChatConfigData: io::StoredData { 
		char url[URL_BUFF_SIZE];
		char bearerToken[BEARER_TOKEN_SIZE];
		char unitGuid[global::UNIT_NUMBER_BUFF_SIZE];
	};

extern unsigned long lastPollTime;
	extern ChatConfig config;

	bool canPoll();
	void poll(bool log);
	void send(bool log);
	bool initAPI();

	bool isConfigValid();

	bool initConfigFromStorage();
	void writeConfigToStorage();

	void saveChatConfig(char buffer[command::CMD_BUFF_SIZE]);
}