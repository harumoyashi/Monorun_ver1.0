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
	object_->position.x = -360.0f;
	object_->position.y = -96.0f;
	object_->rotation.z = 0.0f;
	// --�v���C���[�̏��-- //
	state_ = Start;

	// --���x-- //
	speedX_ = 0.0f;
	speedY_ = 0.0f;

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

	// --�Փ˔�����s����-- //
	isColActive_ = false;

	deathStartCount_ = 0;

	isReaction_ = false;

	reactionCount_ = 0;

	material.SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

	isDeathParticle = true;
}

// --����������-- //
void Player::Initialize(NDX12* dx12) {
	audio = audio = new NAudio();
	audio->Initialize();
	soundData[0] = audio->LoadWave("jump.wav");		// �����Չ�
	soundData[1] = audio->LoadWave("hit_wall.wav");		// �ǂɓ����鉹

	//�}�e���A��(�萔�o�b�t�@)
	material.Initialize(dx12->GetDevice());
	material.SetColor({ 1,1,1,1 });

	// --�v���C���[�I�u�W�F�N�g(�萔�o�b�t�@)-- //
	object_ = std::make_unique<NObj3d>();
	object_->Initialize(dx12->GetDevice());
	object_->texNum = PLAYER;
	object_->scale = { 24.0f, 24.0f, 24.0f };
	object_->position.x = -360.0f;
	object_->position.y = -96.0f;

	//�p�[�e�B�N��
	particle = std::make_unique<NParticle>();
	particle->Initialize(dx12->GetDevice());
	deathParticle = std::make_unique<NParticle>();
	deathParticle->Initialize(dx12->GetDevice());

	// --�v���C���[�̏��-- //
	state_ = Start;

	// --���x-- //
	speedX_ = 0.0f;
	speedY_ = 0.0f;

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

	// --�Փ˔�����s����-- //
	isColActive_ = false;

	isReaction_ = false;

	reactionCount_ = 0;

	material.SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
}

// --�X�V����-- //
void Player::Update(NDX12* dx12, XMMATRIX matView, XMMATRIX matProjection) {
	particle->WallHit(isParticle, directionX_, matView, matProjection, object_.get());
	isParticle = false;

	scrollY_ = 0.0f;

#pragma region �ʏ��Ԃ��Ǔ`�����̏���
	if (state_ == Start) {

	}

	else if (state_ == Goal) {
		// --�S�[����ԂɂȂ��Ă���̌o�ߎ���-- //
		float nowCount = static_cast<float>(Util::GetNowCount());
		float nowTime = (nowCount - easeStartCount_) / 1000.0f;

		float easeRota = nowTime / goalEaseTime_;
		easeRota = Util::Clamp(easeRota, 1.0f, 0.0f);

		speedY_ = 0.0f;

		object_->position.x = Util::EaseOutCubic(easeStartPosX_, easeEndPosX_, easeRota);

		object_->rotation.z += rotaSpeed;
	}

	else if (state_ == DeathReaction) {
		// --���S��ԂɂȂ��Ă���̌o�ߎ���-- //
		float nowCount = static_cast<float>(Util::GetNowCount());
		float nowTime = (nowCount - deathStartCount_) / 1000.0f;

		speedY_ = 0.0f;

		if (nowTime >= deathWaitingTime) {
			object_->position.y = 2000.0f;
			state_ = Death;
		}

		deathParticle->PlayerBreak(isDeathParticle, 50, matView, matProjection, object_.get());
		isDeathParticle = false;
	}

	else if (state_ == NormalWallHit) {
		isParticle = true;	//�p�[�e�B�N���o���`

		isColActive_ = true;

		// --[SPACE]����������-- //
		if (NInput::IsKeyTrigger(DIK_SPACE)) {
			audio->PlayWave(soundData[0], false,2.0f);

			isReaction_ = true;

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
			audio->PlayWave(soundData[0], false, 2.0f);

			isReaction_ = true;

			// --���E�̌�����ς���-- //
			ChangeDireX();

			// --��Ԃ�ς���-- //
			state_ = NormalAir;// -> �ʏ��Ԃ���

			material.SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

			// --X���̑��x��ς���-- //
			speedX_ = wallKickSpeedX_;// --�ǃL�b�N�����Ƃ��̑��x-- //

			object_->rotation.z = 0.0f;
		}
		else {
			object_->rotation.z += rotaSpeed * directionX_;
		}

		// --�u�[�X�g��ԂɂȂ��Ă���̌o�ߎ���-- //
		float nowCount = static_cast<float>(Util::GetNowCount());
		float nowTime = (nowCount - rotateStartTime_) / 1000.0f;

		// --�w�肳��Ă���u�[�X�g���Ԃ��߂�����-- //
		if (rotateTime_ <= nowTime) {
			// --�u�[�X�g��Ԃ���ʏ��ԂɕύX-- //
			state_ = NormalWallHit;

			material.SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

			object_->rotation.z = 0.0f;
		}
	}
#pragma endregion

#pragma region ��]��Ԃ��󒆂ɂ���
	else if (state_ == RotateAir) {
		// --�u�[�X�g��ԂɂȂ��Ă���̌o�ߎ���-- //
		float nowCount = static_cast<float>(Util::GetNowCount());
		float nowTime = (nowCount - rotateStartTime_) / 1000.0f;

		// --�w�肳��Ă���u�[�X�g���Ԃ��߂�����-- //
		if (rotateTime_ <= nowTime) {
			// --�u�[�X�g��Ԃ���ʏ��ԂɕύX-- //
			state_ = NormalAir;
			material.SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

			object_->rotation.z = 0.0f;
		}

		object_->rotation.z += rotaSpeed * directionX_;
	}
#pragma endregion

	if (isReaction_ == true) {
		float easeRota = reactionCount_ / 10.0f;
		object_->scale.x = Util::EaseOutCubic(70.0f, 24.0f, easeRota);
		object_->scale.y = Util::EaseOutCubic(40.0f, 24.0f, easeRota);
		reactionCount_++;

		if (reactionCount_ >= 10) {
			isReaction_ = false;
			reactionCount_ = 0;
			object_->scale.x = 24.0f;
			object_->scale.y = 24.0f;
		}
	}

	// --�v���C���[��X���ɉ��Z-- //
	object_->position.x += speedX_ * directionX_;

	// --�v���C���[�̈ړ����X�N���[��-- //
	object_->position.y += speedY_ * directionY_;
	scrollY_ = speedY_ * directionY_;

	if (state_ != Start) {
		// --x���W���Œ���W�ȉ��ɂȂ�����-- //
		if (object_->position.x < minPosX_) {
			// --x���W��ύX-- //
			object_->position.x = minPosX_;

			audio->PlayWave(soundData[1], false, 3.5f);

			// --��Ԃ�ύX-- //
			if (state_ == RotateAir) {
				state_ = RotateWallHit;// -> ��]��Ԃ��Ǔ`����
			}

			else if (state_ == NormalAir) {
				material.SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
				state_ = NormalWallHit;// -> �ʏ��Ԃ��Ǔ`����
			}

			// --X���̑��x��ς���-- //
			speedX_ = 0.0f;// -> �����Ȃ��悤��

			if (!isCameraShake_) {
				SetCamShakeState(true);
			}
		}

		// --x���W���ō����W�ȏ�ɂȂ�����-- //
		else if (object_->position.x > maxPosX_) {
			// --X���W��ύX-- //
			object_->position.x = maxPosX_;

			audio->PlayWave(soundData[1], false, 3.5f);

			if (state_ == RotateAir) {
				state_ = RotateWallHit;// -> ��]��Ԃ��Ǔ`����
			}

			else if (state_ == NormalAir) {
				material.SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
				state_ = NormalWallHit;// -> �ʏ��Ԃ��Ǔ`����
			}

			// --X���̑��x��ς���-- //
			speedX_ = 0.0f;// -> �����Ȃ��悤��

			if (!isCameraShake_) {
				SetCamShakeState(true);
			}
		}
	}

	object_->UpdateMatrix(matView, matProjection);
}

// --�`�揈��-- //
void Player::Draw(NDX12* dx12, NCube* cube) {
	particle->Draw(dx12);
	deathParticle->Draw(dx12);

	// --�I�u�W�F�N�g�`��-- //
	if (state_ != Death && state_ != DeathReaction)
	{
		object_->CommonBeginDraw(dx12->GetCommandList(), NSceneManager::GetPipeline3d()->pipelineSet.pipelineState, NSceneManager::GetPipeline3d()->pipelineSet.rootSig.entity, dx12->GetSRVHeap());
		object_->Draw(dx12->GetCommandList(), material, dx12->GetSRVHeap(), cube->vbView, cube->ibView, cube->numIB, NSceneManager::GetTex()[0].incrementSize);
	}
}

// --�I������-- //
void Player::Finalize() {
	delete audio;
}

// --�v���C���[�̏�Ԃ�ύX-- //
int Player::GetState() { return state_; }

// --�v���C���[�̏�Ԃ�ύX-- //
void Player::SetState(int state) { state_ = state; }

void Player::SetGame() {
	state_ = NormalWallHit;
	speedY_ = defSpeedY_;
}
//
// --���S��ԂɕύX-- //
void Player::SetDeath() {
	state_ = DeathReaction;
	speedY_ = 0.0f;
	speedX_ = 0.0f;
	deathStartCount_ = Util::GetNowCount();
	isColActive_ = false;
}

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
	material.SetColor({ 0.95f, 0.1f, 0.1f, 1.0f });

	if (state_ == NormalAir) state_ = RotateAir;
	else if (state_ == NormalWallHit) state_ = RotateWallHit;
	else if (state_ == RotateAir) state_ = RotateAir;
	else if (state_ == RotateWallHit) state_ = RotateWallHit;
	rotateStartTime_ = Util::GetNowCount();
}

// --�S�[����Ԃɂ���-- //
void Player::SetGoal() {
	state_ = Goal;
	speedY_ = 5.0f;
	easeStartCount_ = Util::GetNowCount();
	easeStartPosX_ = object_->position.x;
	isColActive_ = false;
}

// --�\���̂ŕԂ�-- //
BoxObj Player::GetBoxObj() {
	return { {object_->position.x, object_->position.y}, 24.0f };
}