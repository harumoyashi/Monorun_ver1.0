#pragma once
#include "NDX12.h"

#include <vector>
#include <wrl.h>

#include "NRootParam.h"
#include "NMaterial.h"
#include "NGPipeline.h"
#include "NRootSignature.h"
#include "NObj3d.h"
#include "NTexture.h"
#include "NSprite.h"
#include "NPreDraw.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

enum State {
	NormalWallHit,// -> �ʏ��ԂŕǓ`����
	NormalAir,// -> �ʏ��Ԃŋ󒆂ɂ���
	RotateWallHit,// -> ��]��ԂŕǓ`����
	RotateAir,// -> ��]��Ԃŋ󒆂ɂ���
	Death// --���S���
};

class Player {
	/// --�����o�ϐ�-- ///
public:
	// --�I�u�W�F�N�g-- //
	NObj3d object_;// ------> �I�u�W�F�N�g(�萔�o�b�t�@)

private:

#pragma region �N���X�錾
	
#pragma endregion

	// --�v���C���[�̏��-- //
	int state_;

#pragma region �v���C���[�̑��x�ϐ�
	// --���ۂɉ��Z���鑬�x�ϐ�-- //
	float speedX_;// -> X��
	float speedY_;// -> Y��
	const float defSpeedY_ = 5.0f;// -> Y����{���x

	// --Y���̍ō����x-- //
	const float maxSpeedY_ = 30.0f;

	// --Y���̍Œᑬ�x-- //
	const float minSpeedY_ = 5.0f;

	// --�ǃL�b�N�̑��x-- //
	const float wallKickSpeedX_ = 20.0f;

	// --Y���Ɍ������l-- //
	const float decelerationY_ = 0.2f;

	// --Y���̉����l-- //
	const float accelerationY_ = 5.0f;
#pragma endregion

#pragma region �u�[�X�g���ԕϐ�
	// --�u�[�X�g�̎���[s]-- //
	const float rotateTime_ = 0.5f;

	// --�u�[�X�g�̌o�ߎ���[s]-- //
	float rotateTimer_;

	// --�u�[�X�g���n�܂������̎���-- //
	int rotateStartTime_;
#pragma endregion

	// --X���W�̍ō����W-- //
	const float maxPosX_ = 296.0f;

	// --X���W�̍Œ���W-- //
	const float minPosX_ = -296.0f;

	// --�󒆂ɂ��邩-- //
	bool isAir_;

	// --�󒆃L�b�N���ł��邩-- //
	bool isAirKickActive_;

	// --�ړ��������-- //
	float directionX_;
	float directionY_;

	/// --�����o�ϐ�END-- ///
	/// --------------- ///
	/// --�����o�֐�-- ///
public:
	// --�C���X�^���X�擾-- //
	Player* GetInstance();

	// --����������-- //
	void Initialize(NDX12* dx12);

	// --�X�V����-- //
	void Update(XMMATRIX &matView, XMMATRIX &matProjection, XMFLOAT3 &eye, XMFLOAT3 &target, XMFLOAT3 &up);

	// --�`�揈��-- //
	void Draw(NDX12* dx12, NMaterial material);

	// --�I������-- //
	void Finalize();

	// --�v���C���[�̏�Ԃ��Q��-- //
	int GetState();

	// --�v���C���[�̏�Ԃ�ύX-- //
	void SetState(int state);

	float& GetSpeedX();
	float& GetSpeedY();

	// --�ʏ��ԂɕύX-- //
	void SetNormal();

	// --�u�[�X�g��ԂɕύX-- //
	void SetBoost();

	// --���S��ԂɕύX-- //
	void SetDeath();

	// --X���̌�����ς���-- //
	void ChangeDireX();
	void ChangeDireY();

	// --��]��Ԃɂ���-- //
	void SetRotate();

private:

	//void Collision();

	/// --�����o�֐�END-- ///
};