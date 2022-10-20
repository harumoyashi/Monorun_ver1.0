#pragma once
#include "NVector2.h"

enum class EffectType {
	BToT
};

class Particle
{
public: // �֐�
	Particle(NVector2 pos, float radius, int indexType);
	~Particle();

	// ��{
	void Activate();
	void Reset();

	//setter
	void SetRadius(float radius) { radius_ = radius; }
	void SetPos(NVector2 pos) { pos_ = pos; }

	// getter
	NVector2& GetPos() { return pos_; }

	// ���
	void InOutBoxAll();

private: // �ϐ�
// ���W
	NVector2 pos_{};

	const NVector2 startPos_{};

	int effectType_{ 0 };
	// ���a
	float radius_{ 0.0f };

	const int startRadius_{};

	// effect�J�n���_�ł�pc�����̎��Ԃ��L�^
	int startTime_{ 0 };

	// effect���J�n���Ă���̌o�ߎ��Ԃ��L�^
	float elapsedTime_{ 0.0f };

	// �Đ��̗L��
	bool isPlay_ = false;
};

