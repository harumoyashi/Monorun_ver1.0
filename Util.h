#pragma once
#include <chrono>

//struct BoxObj {
//	// --座標-- //
//	Vector2 pos;
//
//	// --半径-- //
//	float radiusX;// -> X軸
//	float radiusY;// -> Y軸
//};
//
//struct Image {
//	// --座標-- //
//	Vector2 pos;
//
//	// --X軸の半径-- //
//	float radiusX;
//
//	// --Y軸の半径-- //
//	float radiusY;
//
//	// --画像のハンドル-- //
//	int handle;
//};

double Sign(double a);

namespace Util {

	const float PI = 3.141592654f;

	// --ここから自作関数-- //

	/// <summary>
	/// 入れた値が最大値を上回っていたら最大値を返し、最小値を下回っていたら最小値を返す。
	/// どちらにも当てはまらない場合は、値をそのまま返す
	/// </summary>
	/// <param name="value"> 値 </param>
	/// <param name="max"> 最大値 </param>
	/// <param name="min"> 最小値 </param>
	/// <returns></returns>
	int Clamp(int value, int max, int min);

	/// <summary>
	/// 入れた値が最大値を上回っていたら最大値を返し、最小値を下回っていたら最小値を返す。
	/// どちらにも当てはまらない場合は、値をそのまま返す
	/// </summary>
	/// <param name="value"> 値 </param>
	/// <param name="max"> 最大値 </param>
	/// <param name="min"> 最小値 </param>
	/// <returns></returns>
	float Clamp(float value, float max, float min);

	/// <summary>
	/// 弧度法の値を度数法にして返す
	/// </summary>
	/// <param name="radian"> 弧度法の値 </param>
	/// <returns></returns>
	float Radian2Degree(float radian);

	/// <summary>
	/// 度数法の値を弧度法にして返す
	/// </summary>
	/// <param name="degree"> 度数法の値 </param>
	/// <returns></returns>
	float Degree2Radian(float degree);
	using namespace std::chrono;

	// 現在の時刻をミリ秒単位で取得 ※動作保証なし。
	inline int GetNowCount(void) {
		return static_cast<int>(duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count());
	}
} // namespace MathUtility

[[nodiscard]] inline float OutBounce(float time, float totaltime, float end = 1.0f, float start = 0.0f)
{
	constexpr float n1 = 7.5625f;
	constexpr float d1 = 2.75f;

	float timeRate = time / totaltime < 1.0f ? time / totaltime : 1.0f;

	if (timeRate < 1 / d1) {
		return start + (end - start) * n1 * timeRate * timeRate;
	}
	else if (timeRate < 2 / d1) {
		return start + (end - start) * (n1 * (timeRate -= 1.5 / d1) * timeRate + 0.75f);
	}
	else if (timeRate < 2.5 / d1) {
		return start + (end - start) * (n1 * (timeRate -= 2.25 / d1) * timeRate + 0.9375);
	}
	else {
		return start + (end - start) * (n1 * (timeRate -= 2.625 / d1) * timeRate + 0.984375);
	}
}

[[nodiscard]] static inline float InOutBack(float time, float totaltime, float end = 1.0f, float start = 0.0f)
{
	constexpr float c1 = 1.70158f;
	constexpr float c2 = c1 * 1.525f;

	float timeRate = time / totaltime < 1.0f ? time / totaltime : 1.0f;

	return timeRate < 0.5
		? start + (end - start) * (pow(2 * timeRate, 2) * ((c2 + 1) * 2 * timeRate - c2)) / 2
		: start + (end - start) * (pow(2 * timeRate - 2, 2) * ((c2 + 1) * (timeRate * 2 - 2) + c2) + 2) / 2;
}