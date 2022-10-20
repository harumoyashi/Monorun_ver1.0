#pragma once
#include <chrono>

namespace Util {
	using namespace std::chrono;

	// 現在の時刻をミリ秒単位で取得 ※動作保証なし。
	inline int GetNowCount(void) {
		return static_cast<int>(duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count());
	}
}