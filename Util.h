#pragma once
#include <chrono>

namespace Util {
	using namespace std::chrono;

	// Œ»İ‚Ì‚ğƒ~ƒŠ•b’PˆÊ‚Åæ“¾ ¦“®ì•ÛØ‚È‚µB
	inline int GetNowCount(void) {
		return static_cast<int>(duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count());
	}
}