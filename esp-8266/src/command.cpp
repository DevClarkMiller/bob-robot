#include "globals.hpp"
#include "wifi.hpp"
#include "command.hpp"
#include "chat.hpp"

namespace command {
	void wifiStat(char buffer[CMD_BUFF_SIZE]) { sendCommand("WIFI_STAT", wifi::creds.connected ? "1" : "0"); }

	void setToken(char buffer[CMD_BUFF_SIZE]) { strcpy(chat::config.bearerToken, buffer); }
	void getToken(char buffer[CMD_BUFF_SIZE]) { sendCommand("TOKEN", chat::config.bearerToken); };
	
	void setUrl(char buffer[CMD_BUFF_SIZE]) { strcpy(chat::config.url, buffer); }
	void getUrl(char buffer[CMD_BUFF_SIZE]) { sendCommand("URL", chat::config.url); };

	void setUnitGuid(char buffer[CMD_BUFF_SIZE]) { strcpy(chat::config.unitGuid, buffer); }
	void getUnitGuid(char buffer[CMD_BUFF_SIZE]) { sendCommand("UNIT_GUID", chat::config.unitGuid); }

	void pollChat(char buffer[CMD_BUFF_SIZE]) { 
		if (chat::canPoll()) chat::poll(); 
		else sendCommand("POLL_FAIL", "Not connected to network");
	}

	// TODO: USE AN ENUM INSTEAD TO SAVE ON MEMORY, OR USE A MACRO SOMEHOW
	// also maybe add a description for each command
	const Command COMMANDS[] = {
		{"SAVE_WIFI_CREDS", wifi::saveCredentials},
		{"SET_SSID", wifi::setSSID},
		{"SET_PASSWD", wifi::setPASSWD},
		{"WIFI_STAT", wifiStat},
		{"SET_TOKEN", setToken},
		{"GET_TOKEN", getToken},
		{"SET_URL", setUrl},
		{"GET_URL", getUrl},
		{"SET_UNIT_GUID", setUnitGuid},
		{"GET_UNIT_GUID", getUnitGuid},
		{"SAVE_CHAT_CONFIG", chat::saveChatConfig},
		{"POLL_CHAT", pollChat},
		{"SEND_CHAT", chat::send},
		{"HELP", help}
	};

	void help(char buffer[CMD_BUFF_SIZE]) {
		Serial.println("Commands");
		int n = sizeof(COMMANDS) / sizeof(Command);
		for (int i = 0; i < n; i++) {
			const Command& command = COMMANDS[i];
			Serial.println(command.name);
		}
	}	 

	void handleCommand(char buffer[CMD_BUFF_SIZE]) {
		char cmdNameBuff[CMD_NAME_BUFF_SIZE];
		parseCommand(buffer, cmdNameBuff);

		int n = sizeof(COMMANDS) / sizeof(Command);
		const Command* cmd = command::getCommand(cmdNameBuff, COMMANDS, n);
		if (cmd != nullptr) cmd->handler(buffer);
		else sendCommand("CMD_404", "1");
	}
}