#include "globals.hpp"
#include "wifi.hpp"
#include "command.hpp"
#include "chat.hpp"

namespace command {
	void wifiStat(char buffer[CMD_BUFF_SIZE]) { sendCommand("WIFI_STAT", wifi::creds.connected ? "1" : "0"); }

	void setToken(char buffer[CMD_BUFF_SIZE]) { strcpy(chat::config.bearerToken, buffer); }
	void getToken(char buffer[CMD_BUFF_SIZE]) { sendCommand("TOKEN", chat::config.bearerToken); }; 

	void setUnitGuid(char buffer[CMD_BUFF_SIZE]) { strcpy(chat::config.unitGuid, buffer); }
	void getUnitGuid(char buffer[CMD_BUFF_SIZE]) { sendCommand("TOKEN", chat::config.unitGuid); }

	void pollChat(char buffer[CMD_BUFF_SIZE]) { if (chat::canPoll()) chat::poll(LOGGING); }

	// TODO: USE A PREPROCESSOR INSTEAD 

	// TODO: USE AN ENUM INSTEAD TO SAVE ON MEMORY, OR USE A MACRO SOMEHOW
	const Command COMMANDS[] = {
		{"SET_WIFI_CREDS", wifi::setCredentials},
		{"WIFI_STAT", wifiStat},
		{"SET_TOKEN", setToken},
		{"GET_TOKEN", getToken},
		{"SET_UNIT_GUID", setUnitGuid},
		{"GET_UNIT_GUID", getUnitGuid},
		{"SAVE_CHAT_CONFIG", chat::saveChatConfig},
		{"POLL_CHAT", pollChat},
		{""}
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