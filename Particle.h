#pragma once
#include "NVector2.h"
#include "NSprite.h"
#include <algorithm>
#include "NDX12.h"

enum class EffectType {
	CToA, // ���S������͂ɍL����
};

class Particle
{
public: // �֐�
	Particle(NVector2 pos, float radius, int indexType);
	~Particle();

	// ��{
	void Init();
	void Activate();
	void Reset();

	//setter
	void SetRadius(float radius);
	void SetPos(NVector2 pos);

	// getter
	NVector2& GetPos() { return pos_; }

	// ���
	void InOutBoxAllUpdate();
	void InOutBoxAllDraw();

private: // �ϐ�

#pragma region �C���X�^���X�ێ�

	NDX12* ndx12_ = nullptr;

#pragma endregion

// ���W
	NVector2 pos_{};

	const NVector2 startPos_{};

	int effectType_{ 0 };
	// ���a
	float radius_{ 0.0f };

	const float startRadius_{};

	// effect�J�n���_�ł�pc�����̎��Ԃ��L�^
	int startTime_{ 0 };

	// effect���J�n���Ă���̌o�ߎ��Ԃ��L�^
	float elapsedTime_{ 0.0f };

	// �Đ��̗L��
	bool isPlay_ = false;

	// -- �摜
	std::unique_ptr<NSprite> whiteBox_;
	std::unique_ptr<NSprite> blackBox_;
};

