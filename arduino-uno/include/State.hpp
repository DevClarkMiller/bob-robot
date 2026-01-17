#pragma once
#include "Arduino.h"
#include <Command.h>

struct Context;

struct State {
  	Context* ctx = nullptr;
  	void setContext(Context* c) { ctx = c; }

	virtual void onBackClick() {}
	virtual void onBackHold();
	virtual void onBackDoubleClick() {}
  
	virtual void onLeftClick() {}
	virtual void onLeftHold() {}
	virtual void onLeftDoubleClick() {}

	virtual void onActionClick() {}
	virtual void onActionHold() {}
	virtual void onActionDoubleClick() {}

	virtual void onRightClick() {}
	virtual void onRightHold() {}
	virtual void onRightDoubleClick() {}

	virtual void handle() = 0;
	virtual void start();
  	virtual const char* name() const = 0;
    virtual ~State() = default;
};

struct ReceiveState: State {
	void onBackHold() override; 

	void start() override;
	void handle() override;
	const char* name() const override { return "Receive"; };
};

struct PrintState: State {
	State* prevState;

	void copyIntoMsgBuffer(char buffer[command::CMD_BUFF_SIZE]);

	void start() override;
	void handle() override;
	const char* name() const override { return "Print"; };

private:
	char msgBuffer[command::CMD_BUFF_SIZE];
};

struct WifiInputState: State {
	void start() override;
	void handle() override;
	const char* name() const override { return "WifiInput"; };

private:
	bool transferCredentials();
};

struct TextInputState: State {
	State* prevState;
	String* outputString;

	virtual void onBackClick();
	virtual void onBackHold();
	virtual void onBackDoubleClick();

	void onLeftClick() override;
	void onLeftHold() override;
	void onLeftDoubleClick() override;

	void onActionClick() override;
	void onActionHold() override;
	void onActionDoubleClick() override;

	void onRightClick() override;
	void onRightHold() override;
	void onRightDoubleClick() override;

	void handle() override;
	void start() override;
	const char* name() const override { return "TextInput"; };

private:
	bool isUppercase = false;
	int cursorX;
	int prevPage;

	int getStartXOnCurrPage();
	void checkPage();
	int getCurrPage();
	int getCurrCol();
	const char* getKeyboardStr();
	int getKeyArrSize();
	void onInputAction();
	void printKeyboard(bool doTransition = true);
};

struct MessageInputState: State {
	void handle() override;
	void start() override;
	const char* name() const override { return "MessageInput"; }

private:
	void sendMessage();
};

struct MenuState: State {
	int cursorX;
	
	void onLeftClick() override;
	void onLeftHold() override;

	void onActionClick() override;
	void onActionHold() override;

	void onRightClick() override;
	void onRightHold() override;

	void handle() override;
	void start() override;
	const char* name() const override { return "Menu"; }
};