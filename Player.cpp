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
	object_->position.x = minPosX_;
	object_->position.y = 0.0f;
	object_->rotation.z = 0.0f;
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

	// --�Փ˔�����s����-- //
	isColActive_ = true;

	deathStartCount_ = 0;
}

// --����������-- //
void Player::Initialize(NDX12* dx12) {
	//�}�e���A��(�萔�o�b�t�@)
	material.Initialize(dx12->GetDevice());
	material.SetColor({ 1,1,1,1 });

	// --�v���C���[�I�u�W�F�N�g(�萔�o�b�t�@)-- //
	object_ = std::make_unique<NObj3d>();
	object_->Initialize(dx12->GetDevice());
	object_->texNum = PLAYER;
	object_->scale = { 24.0f, 24.0f, 24.0f };
	object_->position.x = minPosX_;

	//�p�[�e�B�N��
	particle = std::make_unique<NParticle>();
	particle->Initialize(dx12->GetDevice());

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

	// --�Փ˔�����s����-- //
	isColActive_ = true;

	speedSprite[0] = std::make_unique<NSprite>();
	speedSprite[0]->texNum = static_cast<int>(NUMBER);

	speedSprite[1] = std::make_unique<NSprite>();
	speedSprite[1]->texNum = static_cast<int>(NUMBER);

	speedSprite[2] = std::make_unique<NSprite>();
	speedSprite[2]->texNum = static_cast<int>(NUMBER);
}

// --�X�V����-- //
void Player::Update(XMMATRIX matView, XMMATRIX matProjection) {
	particle->WallHit(isParticle, directionX_, matView, matProjection, object_.get());
	isParticle = false;

	scrollY_ = 0.0f;
	if (state_ != Goal && state_ != Death && state_ != DeathResult) {
#pragma region �ʏ��Ԃ��Ǔ`�����̏���
		if (state_ == NormalWallHit) {
			isParticle = true;	//�p�[�e�B�N���o���`

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

				object_->rotation.z = 0.0f;
			}

			object_->rotation.z += rotaSpeed * directionX_;
		}
#pragma endregion

		// --�v���C���[��X���ɉ��Z-- //
		object_->position.x += speedX_ * directionX_;

		// --�v���C���[�̈ړ����X�N���[��-- //
		object_->position.y += speedY_ * directionY_;
		scrollY_ = speedY_ * directionY_;

		// --x���W���Œ���W�ȉ��ɂȂ�����-- //
		if (object_->position.x < minPosX_) {
			// --x���W��ύX-- //
			object_->position.x = minPosX_;

			// --��Ԃ�ύX-- //
			if (state_ == RotateAir) {
				state_ = RotateWallHit;// -> ��]��Ԃ��Ǔ`����
			}

			else if (state_ == NormalAir) {
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

			if (state_ == RotateAir) {
				state_ = RotateWallHit;// -> ��]��Ԃ��Ǔ`����
			}

			else if (state_ == NormalAir) {
				state_ = NormalWallHit;// -> �ʏ��Ԃ��Ǔ`����
			}

			// --X���̑��x��ς���-- //
			speedX_ = 0.0f;// -> �����Ȃ��悤��

			if (!isCameraShake_) {
				SetCamShakeState(true);
			}
		}
	}

	else if (state_ == Goal) {
		// --�S�[����ԂɂȂ��Ă���̌o�ߎ���-- //
		float nowCount = static_cast<float>(Util::GetNowCount());
		float nowTime = (nowCount - easeStartCount_) / 1000.0f;

		float easeRota = nowTime / goalEaseTime_;
		easeRota = Util::Clamp(easeRota, 1.0f, 0.0f);

		object_->position.x = Util::EaseOutCubic(easeStartPosX_, easeEndPosX_, easeRota);

		object_->rotation.z += rotaSpeed;
	}

	else if (state_ == Death || state_ == DeathResult) {
		// --�S�[����ԂɂȂ��Ă���̌o�ߎ���-- //
		float nowCount = static_cast<float>(Util::GetNowCount());
		float nowTime = (nowCount - deathStartCount_) / 1000.0f;

		if (nowTime >= deathWaitingTime) {
			object_->position.x = 1000.0f;
			state_ = DeathResult;
		}
	}

	object_->UpdateMatrix(matView, matProjection);

	int saveNum = speedY_ * 10;
	disPlaySpeed[0] = static_cast<int>(saveNum / 100);
	saveNum = saveNum % 100;
	disPlaySpeed[1] = static_cast<int>(saveNum / 10);
	saveNum = saveNum % 10;
	disPlaySpeed[2] = static_cast<int>(saveNum / 1);
}

// --�`�揈��-- //
void Player::Draw(NDX12* dx12, NCube* cube) {
	speedSprite[0]->CreateClipSprite(dx12->GetDevice(), NSceneManager::GetTex()[speedSprite[0]->texNum].texBuff, { disPlaySpeed[0] * 48.0f, 0.0f }, { 48.0f, 69.0f });
	speedSprite[0]->SetColor(1.0f, 1.0f, 1.0f, 0.5f);
	speedSprite[0]->size = { 48.0f * 3, 69.0f * 3 };
	speedSprite[0]->TransferVertex();
	speedSprite[0]->position = { 180.0f,NWindows::win_height / 2.0f,0 };
	speedSprite[0]->UpdateMatrix();

	speedSprite[1]->CreateClipSprite(dx12->GetDevice(), NSceneManager::GetTex()[speedSprite[1]->texNum].texBuff, { disPlaySpeed[1] * 48.0f, 0.0f }, { 48.0f, 69.0f });
	speedSprite[1]->SetColor(1.0f, 1.0f, 1.0f, 0.5f);
	speedSprite[1]->size = { 48.0f * 3, 69.0f * 3 };
	speedSprite[1]->TransferVertex();
	speedSprite[1]->position = { 300.0f,NWindows::win_height / 2.0f,0 };
	speedSprite[1]->UpdateMatrix();

	speedSprite[2]->CreateClipSprite(dx12->GetDevice(), NSceneManager::GetTex()[speedSprite[2]->texNum].texBuff, { disPlaySpeed[2] * 48.0f, 0.0f }, { 48.0f, 69.0f });
	speedSprite[2]->SetColor(1.0f, 1.0f, 1.0f, 0.5f);
	speedSprite[2]->size = { 48.0f, 69.0f };
	speedSprite[2]->TransferVertex();
	speedSprite[2]->position = { 415.0f,464.0f,0 };
	speedSprite[2]->UpdateMatrix();

	speedSprite[0]->CommonBeginDraw(dx12->GetCommandList(), NSceneManager::GetPipelineSprite()->pipelineSet.pipelineState,
		NSceneManager::GetPipelineSprite()->pipelineSet.rootSig.entity, dx12->GetSRVHeap());
	speedSprite[0]->Draw(dx12->GetSRVHeap(), NSceneManager::GetTex()[0].incrementSize, dx12->GetCommandList());

	//speedSprite[1]->CommonBeginDraw(dx12->GetCommandList(), NSceneManager::GetPipelineSprite()->pipelineSet.pipelineState,
	//	NSceneManager::GetPipelineSprite()->pipelineSet.rootSig.entity, dx12->GetSRVHeap());
	speedSprite[1]->Draw(dx12->GetSRVHeap(), NSceneManager::GetTex()[0].incrementSize, dx12->GetCommandList());
	speedSprite[2]->Draw(dx12->GetSRVHeap(), NSceneManager::GetTex()[0].incrementSize, dx12->GetCommandList());


	// --�I�u�W�F�N�g�`��-- //
	object_->CommonBeginDraw(dx12->GetCommandList(), NSceneManager::GetPipeline3d()->pipelineSet.pipelineState, NSceneManager::GetPipeline3d()->pipelineSet.rootSig.entity, dx12->GetSRVHeap());
	object_->Draw(dx12->GetCommandList(), material, dx12->GetSRVHeap(), cube->vbView, cube->ibView, cube->numIB, NSceneManager::GetTex()[0].incrementSize);

	particle->Draw(dx12, NSceneManager::GetPipeline3d()->pipelineSet.pipelineState, NSceneManager::GetPipeline3d()->pipelineSet.rootSig.entity, NSceneManager::GetTex()[0].incrementSize);
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
	state_ = Death;
	deathStartCount_ = Util::GetNowCount();
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