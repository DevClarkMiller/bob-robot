#include "globals.hpp"
#include "wifi.hpp"
#include "command.hpp"
#include "chat.hpp"

namespace command {
	void wifiStat(char buffer[CMD_BUFF_SIZE]) { sendCommand("WIFI_STAT", wifi::creds.connected ? "1" : "0"); }

	void setCredentials(char buffer[CMD_BUFF_SIZE]) {
		using namespace wifi;
		const char* cmdName = "WIFI_STAT";

		// Pull ssid and passwd buffers from the buffer since we already know their exact size
		char ssidBuff[SSID_BUFF_SIZE];
		char passwdBuff[PASSWD_BUFF_SIZE];

		int j = 0;
		// Copy over the ssid
		for (int i = 0; i < SSID_BUFF_SIZE; i++) 
			ssidBuff[i] = buffer[j++];

		for (int i = 0; i < PASSWD_BUFF_SIZE; i++) 
			passwdBuff[i] = buffer[j++];

		// Copy over the passwd
		strcpy(creds.ssid, ssidBuff);
		strcpy(creds.passwd, passwdBuff);

		if (connect(creds.ssid, creds.passwd, LOGGING)) {
			writeCredsToStorage(); // Only actually save the creds if they're valid
			sendCommand(cmdName, "1");
		} else sendCommand(cmdName, "0");
	}

	void setToken(char buffer[CMD_BUFF_SIZE]) { strcpy(chat::config.bearerToken, buffer); }
	void getToken(char buffer[CMD_BUFF_SIZE]) { sendCommand("TOKEN", chat::config.bearerToken); }; 

	void setUnitGuid(char buffer[CMD_BUFF_SIZE]) { strcpy(chat::config.unitGuid, buffer); }
	void getUnitGuid(char buffer[CMD_BUFF_SIZE]) { sendCommand("TOKEN", chat::config.unitGuid); }

	void saveChatConfig(char buffer[CMD_BUFF_SIZE]) {
		using namespace chat;

		if (!isConfigValid()) { 
			sendCommand("CHAT_CONFIG_INVALID", "1");
			return;
		}
	
		writeConfigToStorage();

		sendCommand("CHAT_SAVED", "1");
	}

	const Command COMMANDS[] = {
		{"SET_WIFI_CREDS", setCredentials},
		{"WIFI_STAT", wifiStat},
		{"SET_TOKEN", setToken},
		{"GET_TOKEN", getToken},
		{"SET_UNIT_GUID", setUnitGuid},
		{"GET_UNIT_GUID", getUnitGuid},
		{"SAVE_CHAT_CONFIG", saveChatConfig}
	};

	void handleCommand(char buffer[CMD_BUFF_SIZE]) {
		char cmdNameBuff[CMD_NAME_BUFF_SIZE];
		parseCommand(buffer, cmdNameBuff);

		int n = sizeof(COMMANDS) / sizeof(Command);
		const Command* cmd = command::getCommand(cmdNameBuff, COMMANDS, n);
		if (cmd != nullptr) cmd->handler(buffer);
		else sendCommand("CMD_404", "1");
	}
}