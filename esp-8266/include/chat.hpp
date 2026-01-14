#pragma once
#include <Chat.h>
#include <Io.h>

namespace chat {
	constexpr size_t POLL_INTERVAL = 5000;
	
	struct ChatConfig {
		char url[URL_BUFF_SIZE];
		char bearerToken[BEARER_TOKEN_SIZE];
		bool isUrlSecure;
	};

	struct ChatConfigData: io::StoredData { 
		char url[URL_BUFF_SIZE];
		char bearerToken[BEARER_TOKEN_SIZE];
		bool isUrlSecure;
	};

	extern ChatConfig chatConfig;

	bool canPoll();
	void poll(bool log);
	void send(bool log);
	void initAPI();
}