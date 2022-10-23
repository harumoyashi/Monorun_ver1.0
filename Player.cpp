#include "Player.h"
#include "NSceneManager.h"

// --��-- //
#define BLACK 0x000000

// --��-- //
#define WHITE 0xFFFFFF

// --����-- //
#define LEFT -1
#define RIGHT 1

#define UP 1
#define DOWN -1

// --�C���X�^���X�ǂݍ���-- //
Player* Player::GetInstance()
{
	static Player instance;
	return &instance;
}

// --���Z�b�g����-- //
void Player::Reset() {
	object_.position.x = minPosX_;
	object_.position.y = 0.0f;
}

// --����������-- //
void Player::Initialize(NDX12* dx12) {
	//�}�e���A��(�萔�o�b�t�@)
	material.Initialize(dx12->GetDevice());
	material.SetColor({ 1,1,1,1 });

	// --�v���C���[�I�u�W�F�N�g(�萔�o�b�t�@)-- //
	object_.Initialize(dx12->GetDevice());
	object_.texNum = PLAYER;
	object_.scale = { 24.0f, 24.0f, 24.0f };
	object_.position.x = minPosX_;

	// --�v���C���[�̏��-- //
	state_ = NormalWallHit;

	// --���x-- //
	speedX_ = 0.0f;
	speedY_ = defSpeedY_;

	// --�ړ��������-- //
	directionX_ = LEFT;
	directionY_ = DOWN;

	// --�u�[�X�g�̌o�ߎ���[s]-- //
	rotateTimer_ = 0.0f;

	// --�u�[�X�g���n�܂������̎���-- //
	rotateStartTime_ = 0;

	// --�󒆂ɂ��邩-- //
	isAir_ = false;

	// --�󒆃L�b�N���ł��邩-- //
	isAirKickActive_ = true;
}

// --�X�V����-- //
void Player::Update(XMMATRIX& matView, XMMATRIX& matProjection, XMFLOAT3& eye, XMFLOAT3& target, XMFLOAT3& up) {

#pragma region �ʏ��Ԃ��Ǔ`�����̏���
	if (state_ == NormalWallHit) {
		// --[SPACE]����������-- //
		if (NInput::IsKeyTrigger(DIK_SPACE)) {
			// --���E�̌�����ς���-- //
			ChangeDireX();

			// --��Ԃ�ς���-- //
			state_ = NormalAir;// -> �ʏ��Ԃ���

			// --X���̑��x��ς���-- //
			speedX_ = wallKickSpeedX_;// --�ǃL�b�N�����Ƃ��̑��x-- //

			// --Y���̈ړ��������ゾ������-- //
			if (directionY_ == UP) {
				speedY_ -= accelerationY_ * 2.0f;
			}

			// --Y���̈ړ�����������������-- //
			else if (directionY_ == DOWN) {
				speedY_ += accelerationY_;
			}
		}

		// --��������-- //
		if (directionY_ == DOWN) {
			speedY_ -= decelerationY_;
			speedY_ = Util::Clamp(speedY_, maxSpeedY_, minSpeedY_);
		}

		else if (directionY_ == UP) {
			speedY_ -= decelerationY_;
			if (speedY_ <= 0) {
				directionY_ = DOWN;
			}
		}
	}
#pragma endregion

#pragma region �ʏ��Ԃ��󒆂ɂ���
	else if (state_ == NormalAir) {

	}
#pragma endregion

#pragma region ��]��Ԃ��Ǔ`����
	else if (state_ == RotateWallHit) {
		// --[SPACE]����������-- //
		if (NInput::IsKeyTrigger(DIK_SPACE)) {
			// --���E�̌�����ς���-- //
			ChangeDireX();

			// --��Ԃ�ς���-- //
			state_ = NormalAir;// -> �ʏ��Ԃ���

			// --X���̑��x��ς���-- //
			speedX_ = wallKickSpeedX_;// --�ǃL�b�N�����Ƃ��̑��x-- //

			object_.rotation.z = 0.0f;
		}
		else {
			object_.rotation.z += rotaSpeed * directionX_;
		}

		// --�u�[�X�g��ԂɂȂ��Ă���̌o�ߎ���-- //
		float nowCount = Util::GetNowCount();
		float nowTime = (nowCount - rotateStartTime_) / 1000.0f;

		// --�w�肳��Ă���u�[�X�g���Ԃ��߂�����-- //
		if (rotateTime_ <= nowTime) {
			// --�u�[�X�g��Ԃ���ʏ��ԂɕύX-- //
			state_ = NormalWallHit;

			object_.rotation.z = 0.0f;
		}
	}
#pragma endregion

#pragma region ��]��Ԃ��󒆂ɂ���
	else if (state_ == RotateAir) {
		// --�u�[�X�g��ԂɂȂ��Ă���̌o�ߎ���-- //
		float nowCount = Util::GetNowCount();
		float nowTime = (nowCount - rotateStartTime_) / 1000.0f;

		// --�w�肳��Ă���u�[�X�g���Ԃ��߂�����-- //
		if (rotateTime_ <= nowTime) {
			// --�u�[�X�g��Ԃ���ʏ��ԂɕύX-- //
			state_ = NormalAir;

			object_.rotation.z = 0.0f;
		}

		object_.rotation.z += rotaSpeed * directionX_;
	}
#pragma endregion

	// --�v���C���[��X���ɉ��Z-- //
	object_.position.x += speedX_ * directionX_;

	// --�v���C���[�̈ړ����X�N���[��-- //
	object_.position.y += speedY_ * directionY_;
	eye.y += speedY_ * directionY_;
	target.y += speedY_ * directionY_;

	// --x���W���Œ���W�ȉ��ɂȂ�����-- //
	if (object_.position.x < minPosX_ ) {
		// --x���W��ύX-- //
		object_.position.x = minPosX_;

		// --��Ԃ�ύX-- //
		if (state_ == RotateAir) {
			state_ = RotateWallHit;// -> ��]��Ԃ��Ǔ`����
		}

		else if (state_ == NormalAir) {
			state_ = NormalWallHit;// -> �ʏ��Ԃ��Ǔ`����
		}

		// --X���̑��x��ς���-- //
		speedX_ = 0.0f;// -> �����Ȃ��悤��
	}

	// --x���W���ō����W�ȏ�ɂȂ�����-- //
	else if (object_.position.x > maxPosX_) {
		// --X���W��ύX-- //
		object_.position.x = maxPosX_;

		if (state_ == RotateAir) {
			state_ = RotateWallHit;// -> ��]��Ԃ��Ǔ`����
		}

		else if (state_ == NormalAir) {
			state_ = NormalWallHit;// -> �ʏ��Ԃ��Ǔ`����
		}

		// --X���̑��x��ς���-- //
		speedX_ = 0.0f;// -> �����Ȃ��悤��
	}

	//�r���[�ϊ��s��쐬
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
	object_.UpdateMatrix(matView, matProjection);
}

// --�`�揈��-- //
void Player::Draw(NDX12* dx12,NCube* cube) {
	// --�I�u�W�F�N�g�`��-- //
	object_.CommonBeginDraw(dx12->GetCommandList(), NSceneManager::GetPipeline3d()->pipelineSet.pipelineState, NSceneManager::GetPipeline3d()->pipelineSet.rootSig.entity, dx12->GetSRVHeap());
	object_.Draw(dx12->GetCommandList(), material, dx12->GetSRVHeap(), cube->vbView, cube->ibView, cube->numIB, NSceneManager::GetTex()[0].incrementSize);
}

// --�I������-- //
void Player::Finalize() {

}

// --�v���C���[�̏�Ԃ�ύX-- //
int Player::GetState() { return state_; }

// --�v���C���[�̏�Ԃ�ύX-- //
void Player::SetState(int state) { state_ = state; }
//
// --���S��ԂɕύX-- //
void Player::SetDeath() {
	NSceneManager::SetScene(TITLESCENE);
}

float& Player::GetSpeedX() { return speedX_; }
float& Player::GetSpeedY() { return speedY_; }

// --X���̌�����ς���-- //
void Player::ChangeDireX() {
	if (directionX_ == LEFT) directionX_ = RIGHT;
	else directionX_ = LEFT;
}

void Player::ChangeDireY() {
	if (directionY_ == UP) directionY_ = DOWN;
	else directionY_ = UP;
}

// --��]��Ԃɂ���-- //
void Player::SetRotate() {
	speedY_ = maxSpeedY_;

	if (state_ == NormalAir) state_ = RotateAir;
	else if (state_ == NormalWallHit) state_ = RotateWallHit;
	else if (state_ == RotateAir) state_ = RotateAir;
	else if (state_ == RotateWallHit) state_ = RotateWallHit;
	rotateStartTime_ = Util::GetNowCount();
}

// --�\���̂ŕԂ�-- //
BoxObj Player::GetBoxObj() {
	return { {object_.position.x, object_.position.y}, 24.0f };
}