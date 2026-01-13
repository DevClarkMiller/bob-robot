#include "globals.hpp"
#include "wifi.hpp"
#include "command.hpp"
#include "chat.hpp"

namespace command {
	void isConnected() { sendCommand("WIFI_STAT", wifi::creds.connected ? "1" : "0"); }

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

	void setToken(char buffer[CMD_BUFF_SIZE]) { 

	}

	void setUnitNumber(char buffer[CMD_BUFF_SIZE]) {

	}

	// Removes the name of the cmd from the buffer and puts it into its own
	void parseCmd(char* buffer, size_t buffsize, char* cmdNameBuff, size_t cmdNameBufSize) {
		// Find the first delim or end of string
		size_t i = 0;
		while (buffer[i] != CMD_DELIM && buffer[i] != '\0' && i < cmdNameBufSize - 1) {
			cmdNameBuff[i] = buffer[i];
			i++;
		}
		cmdNameBuff[i] = '\0';

		// Skip the space (if any)
		size_t j = i;
		if (buffer[j] == ' ') j++;

		// Shift the rest of the buffer left
		size_t k = 0;
		while (j < buffsize && buffer[j] != '\0') {
			buffer[k++] = buffer[j++];
		}
		buffer[k] = '\0';
	}

	void handleCommand(char buffer[CMD_BUFF_SIZE]) {
		char cmdNameBuff[CMD_NAME_BUFF_SIZE];

		parseCmd(buffer, CMD_BUFF_SIZE, cmdNameBuff, CMD_NAME_BUFF_SIZE);
		if (strcmp(cmdNameBuff, "SET_WIFI_CREDS") == 0) setCredentials(buffer);
		else if (strcmp(cmdNameBuff, "WIFI_STAT") == 0) isConnected();
	}
}