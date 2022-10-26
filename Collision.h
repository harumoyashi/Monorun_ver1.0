#pragma once
#include "Player.h"
#include "StageManager.h"
#include "Util.h"
#include "NParticle.h"
#include "NAudio.h"
#include <vector>

class Collision {
	/// --�����o�ϐ�-- ///
public:

private:
	// --�C���X�^���X-- //
	static Collision* myInstance_;

	Player* player_;

	StageManager* stage_;

	BoxObj oldObj_;

	NAudio* audio = nullptr;

	static const int maxSoundData = 10;
	uint32_t soundData[maxSoundData] = {};

	//std::vector<NParticle> particles;
	std::unique_ptr<NParticle[]> particles;

	// --�����蔻����Ƃ邩-- //
	bool isCollision_ = true;

	// --�u���b�N�̃T�C�Y-- //
	float blockSize_ = 64.0f;

	// --Y���X�N���[���̒l-- //
	float scrollY_;

	// getter�p
	bool isCameraShake_ = false;

	/// --�����o�ϐ�END-- ///
	/// --------------- ///
	/// --�����o�֐�-- ///
public:
	// --�C���X�^���X�擾-- //
	static Collision* GetInstance();

	// --���������-- //
	static void Release();

	// --�f�X�g���N�^-- //
	~Collision();

	void Reset();

	void Finalize();

	// --����������-- //
	void Initialize();

	// --�X�V����-- //
	void Update(NDX12* dx12, XMMATRIX matView, XMMATRIX matProjection);

	// --�`�揈��-- //
	void Draw(NDX12* dx12);

	float GetScrollY() { return scrollY_; }

	// cameraShake
	bool GetCamShake() { return isCameraShake_; }
	void SetCamShakeState(bool flag) { isCameraShake_ = flag; }

private:
	// --�R���X�g���N�^-- //
	Collision();

	/// --�����o�֐�END-- ///
};