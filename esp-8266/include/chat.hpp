#pragma once
#include <Chat.h>

namespace chat {
	constexpr size_t POLL_INTERVAL = 5000;

	extern char url[URL_BUFF_SIZE];
	extern char bearerToken[BEARER_TOKEN_SIZE];
	extern bool isUrlSecure;

	bool canPoll();
	void poll(bool log);
	void send(bool log);
	void initAPI();
}