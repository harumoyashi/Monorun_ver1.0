#pragma once
#include <chrono>

//struct BoxObj {
//	// --���W-- //
//	Vector2 pos;
//
//	// --���a-- //
//	float radiusX;// -> X��
//	float radiusY;// -> Y��
//};
//
//struct Image {
//	// --���W-- //
//	Vector2 pos;
//
//	// --X���̔��a-- //
//	float radiusX;
//
//	// --Y���̔��a-- //
//	float radiusY;
//
//	// --�摜�̃n���h��-- //
//	int handle;
//};

double Sign(double a);

namespace Util {

	const float PI = 3.141592654f;

	// --�������玩��֐�-- //

	/// <summary>
	/// ���ꂽ�l���ő�l�������Ă�����ő�l��Ԃ��A�ŏ��l��������Ă�����ŏ��l��Ԃ��B
	/// �ǂ���ɂ����Ă͂܂�Ȃ��ꍇ�́A�l�����̂܂ܕԂ�
	/// </summary>
	/// <param name="value"> �l </param>
	/// <param name="max"> �ő�l </param>
	/// <param name="min"> �ŏ��l </param>
	/// <returns></returns>
	int Clamp(int value, int max, int min);

	/// <summary>
	/// ���ꂽ�l���ő�l�������Ă�����ő�l��Ԃ��A�ŏ��l��������Ă�����ŏ��l��Ԃ��B
	/// �ǂ���ɂ����Ă͂܂�Ȃ��ꍇ�́A�l�����̂܂ܕԂ�
	/// </summary>
	/// <param name="value"> �l </param>
	/// <param name="max"> �ő�l </param>
	/// <param name="min"> �ŏ��l </param>
	/// <returns></returns>
	float Clamp(float value, float max, float min);

	/// <summary>
	/// �ʓx�@�̒l��x���@�ɂ��ĕԂ�
	/// </summary>
	/// <param name="radian"> �ʓx�@�̒l </param>
	/// <returns></returns>
	float Radian2Degree(float radian);

	/// <summary>
	/// �x���@�̒l���ʓx�@�ɂ��ĕԂ�
	/// </summary>
	/// <param name="degree"> �x���@�̒l </param>
	/// <returns></returns>
	float Degree2Radian(float degree);
	using namespace std::chrono;

	// ���݂̎������~���b�P�ʂŎ擾 ������ۏ؂Ȃ��B
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