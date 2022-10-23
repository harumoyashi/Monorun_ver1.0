#pragma once
#include "Particle.h"
#include <vector>
#include <algorithm>

class Effect
{
private:
	static constexpr float particleRaidus_{ 32.0f };

public: // �֐�
	Effect(int indexType);
	~Effect();

	void Activate();
	void Reset();

	void ExpandSquareUpdate();
	void ExpandSquareDraw();

	// getter
	bool GetEffectPlay() { return isPlay_; }
	bool GetAllowChangeScene() { return isAllowChangeScene_; }

private: // �ϐ�
#pragma region �C���X�^���X�ێ�

	NDX12* ndx12_ = nullptr;

#pragma endregion

	int effectType_{ 0 };
	std::vector<std::unique_ptr<Particle>> particles_{};
	int activeCount_{ 0 };

	// effect�J�n���_�ł�pc�����̎��Ԃ��L�^
	int startTime_{ 0 };

	// effect���J�n���Ă���̌o�ߎ��Ԃ��L�^
	float elapsedTime_{ 0.0f };

	// �Đ��̗L��
	bool isPlay_ = false;

	// �V�[���ؑւ��Ă悢��
	bool isAllowChangeScene_ = false;

	// �t���[���J�E���^
	int frameCount_{ 0 };
};