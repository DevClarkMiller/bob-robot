#pragma once
#include <Io.h>
#include "EEPROM.h"

namespace io {
	// Max of 12 things are being stored in flash
	constexpr int STORED_DATA_SIZE = 2;
	constexpr size_t RAW_DATA_HEALTH_ID = 0xDEADBEEF;

	struct StoredDataInfo {
		size_t size;
		char const* name;
	};

	extern StoredDataInfo storedDataInfoArr[STORED_DATA_SIZE];	
	extern size_t storedDataAddrArr[STORED_DATA_SIZE];

	void handleInput();
	void initStorage();

	size_t getStoredDataAddr(const char* name);
	size_t initStoredDataAddresses();

	template <typename STORED_DATA_TYPE, typename DATA_TYPE>
	using DataStorageEventCallback = void (*)(STORED_DATA_TYPE& storedData, DATA_TYPE& data);

	template <typename STORED_DATA_TYPE, typename DATA_TYPE> 
	bool load(DATA_TYPE* data, int address, DataStorageEventCallback<STORED_DATA_TYPE, DATA_TYPE> onRead) {
		STORED_DATA_TYPE storedData;
		EEPROM.get(address, storedData);
		if (!storedData.isStored()) return false; 

		onRead(storedData, *data); 
		return true;
	}

	template <typename STORED_DATA_TYPE, typename DATA_TYPE> 
	void save(DATA_TYPE* data, int address, DataStorageEventCallback<STORED_DATA_TYPE, DATA_TYPE> onWrite) {
		STORED_DATA_TYPE storedData;
		storedData.init();
		onWrite(storedData, *data);

		EEPROM.put(address, storedData);
		EEPROM.commit();
	}
}