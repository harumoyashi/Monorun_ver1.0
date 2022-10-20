#pragma once
#include <chrono>

namespace Util {
	using namespace std::chrono;

	// ���݂̎������~���b�P�ʂŎ擾 ������ۏ؂Ȃ��B
	inline int GetNowCount(void) {
		return static_cast<int>(duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count());
	}
}