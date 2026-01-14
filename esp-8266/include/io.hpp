#pragma once
#include <Io.h>
#include "EEPROM.h"

namespace io {
	// Max of 12 things are being stored in flash
	constexpr int STORED_DATA_ADDR_ARR_SIZE = 12;
	constexpr size_t RAW_DATA_HEALTH_ID = 0xDEADBEEF;

	void handleInput();

	extern size_t storedDataAddrArr[STORED_DATA_ADDR_ARR_SIZE];

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