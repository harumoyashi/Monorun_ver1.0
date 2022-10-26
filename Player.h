#pragma once
#include "NDX12.h"

#include <vector>
#include <wrl.h>

#include "NRootParam.h"
#include "NMaterial.h"
#include "NGPipeline.h"
#include "NRootSignature.h"
#include "NObj3d.h"
#include "NParticle.h"
#include "NTexture.h"
#include "NSprite.h"
#include "NCube.h"
#include "NPreDraw.h"
#include "Util.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

enum State {
	Start,
	NormalWallHit,// -> �ʏ��ԂŕǓ`����
	NormalAir,// -> �ʏ��Ԃŋ󒆂ɂ���
	RotateWallHit,// -> ��]��ԂŕǓ`����
	RotateAir,// -> ��]��Ԃŋ󒆂ɂ���
	DeathReaction,
	Death,
	Goal
};

class Player {
	/// --�����o�ϐ�-- ///
public:
	// --�I�u�W�F�N�g-- //
	std::unique_ptr<NObj3d> object_;// ------> �I�u�W�F�N�g(�萔�o�b�t�@)

private:

#pragma region �N���X�錾
	NMaterial material;				//�}�e���A��

	std::unique_ptr<NParticle> particle;	//�p�[�e�B�N��
#pragma endregion

	// --�v���C���[�̏��-- //
	int state_;

	bool isCollision_ = true;

#pragma region �v���C���[�̑��x�ϐ�
	// --���ۂɉ��Z���鑬�x�ϐ�-- //
	float speedX_;// -> X��
	float speedY_;// -> Y��
	const float defSpeedY_ = 2.5f;// -> Y����{���x

	// --��]���x-- //
	const float rotaSpeed = 18.0f;

	// --Y���̍ō����x-- //
	const float maxSpeedY_ = 20.0f;

	// --Y���̍Œᑬ�x-- //
	const float minSpeedY_ = 4.0f;

	// --�ǃL�b�N�̑��x-- //
	const float wallKickSpeedX_ = 20.0f;

	// --Y���Ɍ������l-- //
	const float decelerationY_ = 0.3f;

	// --Y���̉����l-- //
	const float accelerationY_ = 2.0f;
#pragma endregion

#pragma region �u�[�X�g���ԕϐ�
	// --�u�[�X�g�̎���[s]-- //
	const float rotateTime_ = 0.7f;

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
	int directionX_;
	int directionY_;

	// --Y���X�N���[���̑��x-- //
	float scrollY_;

	// getter�p
	bool isCameraShake_ = false;

	// --�v���C���[�̏Փ˔�����s����-- //
	bool isColActive_;

#pragma region �S�[���������̉��o�p
	// --�C�[�W���O�J�n����X�����W-- //
	float easeStartPosX_;

	// --�C�[�W���O�I������X�����W-- //
	const float easeEndPosX_ = 0.0f;

	// --�C�[�W���O�J�n���̃J�E���g-- //
	int easeStartCount_;

	// --�C�[�W���O����-- //
	const float goalEaseTime_ = 2.5f;
#pragma endregion

#pragma region ���S���̃^�C�}�[
	// --���S��ԊJ�n���̃J�E���g-- //
	int deathStartCount_;

	const float deathWaitingTime = 3.0f;
#pragma endregion

	bool isParticle = false;	//�p�[�e�B�N���o�����ۂ�

	/// --�����o�ϐ�END-- ///
	/// --------------- ///
	/// --�����o�֐�-- ///
public:
	// --�C���X�^���X�擾-- //
	static Player* GetInstance();

	// --���Z�b�g����-- //
	void Reset();

	// --����������-- //
	void Initialize(NDX12* dx12);

	// --�X�V����-- //
	void Update(NDX12* dx12, XMMATRIX matView, XMMATRIX matProjection);

	// --�`�揈��-- //
	void Draw(NDX12* dx12,NCube* cube);

	// --�I������-- //
	void Finalize();

	// --�v���C���[�̏�Ԃ��Q��-- //
	int GetState();

	bool GetIsColActive() { return isColActive_; }

	// --�v���C���[�̏�Ԃ�ύX-- //
	void SetState(int state);

	void SetGame();

	float GetSpeedX() { return speedX_; }
	float GetSpeedY() { return speedY_; }

	int GetDirectionY() { return directionY_; }

	// --���S��ԂɕύX-- //
	void SetDeath();

	// --X���̌�����ς���-- //
	void ChangeDireX();
	void ChangeDireY();

	// --��]��Ԃɂ���-- //
	void SetRotate();

	// --�S�[����Ԃɂ���-- //
	void SetGoal();

	// --�\���̂ŕԂ�-- //
	BoxObj GetBoxObj();

	// --Y���X�N���[���̒l���Q��-- //
	float GetScrollY() { return scrollY_; }

	// cameraShake
	bool GetCamShake() { return isCameraShake_; }
	void SetCamShakeState(bool flag) { isCameraShake_ = flag; }

private:

	//void Collision();

	/// --�����o�֐�END-- ///
};