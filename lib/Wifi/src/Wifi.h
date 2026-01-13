#pragma once
#include <cstddef>

namespace wifi {
	constexpr std::size_t WIFI_TIMEOUT = 10000;
	constexpr int SSID_BUFF_SIZE = 34;
	constexpr int PASSWD_BUFF_SIZE = 65;

	struct WifiCredentials {
		char ssid[SSID_BUFF_SIZE];
		char passwd[PASSWD_BUFF_SIZE];
		bool connected = false;
	};
}