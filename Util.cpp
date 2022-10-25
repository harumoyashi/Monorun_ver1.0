#include "Util.h"
#include <random>

std::random_device seed_gen;
std::mt19937 engine(seed_gen());

double Sign(double a)
{
	return (a > 0) - (a < 0);
}

int Util::Clamp(int value, int max, int min)
{
	// �l���ő�l�������Ă�����ő�l��Ԃ�
	if (value >= max) return max;

	// �l���ŏ��l��������Ă�����ŏ��l��Ԃ�
	if (value <= min) return min;

	// �ǂ���ɂ����Ă͂܂�Ȃ�������l�����̂܂ܕԂ�
	return value;
}

float Util::Clamp(float value, float max, float min)
{
	// �l���ő�l�������Ă�����ő�l��Ԃ�
	if (value >= max) return max;

	// �l���ŏ��l��������Ă�����ŏ��l��Ԃ�
	if (value <= min) return min;

	// �ǂ���ɂ����Ă͂܂�Ȃ�������l�����̂܂ܕԂ�
	return value;
}

float Util::Radian2Degree(float radian)
{
	return radian * 180.0f / PI;
}

float Util::Degree2Radian(float degree)
{
	return degree * PI / 180.0f;
}

void Util::CameraShake(NCamera* camera, int value)
{
	camera->MemorizeEye();
	std::uniform_real_distribution<> dist(-value, value);
	camera->SetEye(XMFLOAT3{ camera->GetEye().x + static_cast<float>(dist(engine)), camera->GetEye().y, camera->GetEye().z + static_cast<float>(dist(engine)) });

	if(0 < camera->GetShakeCount())
	camera->SetShakeCount(camera->GetShakeCount() - 1);
}

float Util::EaseOutCubic(float start, float end, float t) {
	float time = 1 - pow(1 - t, 3);
	return start * (1.0f - time) + end * time;
	//return start * (1.0f - pow(1.0f - t, 3)) + end * t;
}