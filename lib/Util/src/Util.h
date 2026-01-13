#pragma once

namespace util {
	void triml(char* str);
	void trimr(char* str);
	void trim(char* str);

	inline int wrap(int val, int max) { return (val % max + max) % max; }
}