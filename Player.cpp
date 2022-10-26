#include "Player.h"
#include "NSceneManager.h"

// --黒-- //
#define BLACK 0x000000

// --白-- //
#define WHITE 0xFFFFFF

// --向き-- //
#define LEFT -1
#define RIGHT 1

#define UP 1
#define DOWN -1

// --インスタンス読み込み-- //
Player* Player::GetInstance()
{
	static Player instance;
	return &instance;
}

// --リセット処理-- //
void Player::Reset() {
	object_->position.x = -360.0f;
	object_->position.y = -96.0f;
	object_->rotation.z = 0.0f;
	// --プレイヤーの状態-- //
	state_ = Start;

	// --速度-- //
	speedX_ = 0.0f;
	speedY_ = 0.0f;

	// --移動する向き-- //
	directionX_ = LEFT;
	directionY_ = DOWN;

	// --ブーストの経過時間[s]-- //
	rotateTimer_ = 0.0f;

	// --ブーストが始まった時の時間-- //
	rotateStartTime_ = 0;

	// --空中にいるか-- //
	isAir_ = false;

	// --空中キックができるか-- //
	isAirKickActive_ = true;

	// --衝突判定を行うか-- //
	isColActive_ = false;

	deathStartCount_ = 0;

	isReaction_ = false;

	reactionCount_ = 0;

	material.SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

	isDeathParticle = true;
}

// --初期化処理-- //
void Player::Initialize(NDX12* dx12) {
	audio = audio = new NAudio();
	audio->Initialize();
	soundData[0] = audio->LoadWave("jump.wav");		// じゃんぷ音
	soundData[1] = audio->LoadWave("hit_wall.wav");		// 壁に当たる音

	//マテリアル(定数バッファ)
	material.Initialize(dx12->GetDevice());
	material.SetColor({ 1,1,1,1 });

	// --プレイヤーオブジェクト(定数バッファ)-- //
	object_ = std::make_unique<NObj3d>();
	object_->Initialize(dx12->GetDevice());
	object_->texNum = PLAYER;
	object_->scale = { 24.0f, 24.0f, 24.0f };
	object_->position.x = -360.0f;
	object_->position.y = -96.0f;

	//パーティクル
	particle = std::make_unique<NParticle>();
	particle->Initialize(dx12->GetDevice());
	deathParticle = std::make_unique<NParticle>();
	deathParticle->Initialize(dx12->GetDevice());

	// --プレイヤーの状態-- //
	state_ = Start;

	// --速度-- //
	speedX_ = 0.0f;
	speedY_ = 0.0f;

	// --移動する向き-- //
	directionX_ = LEFT;
	directionY_ = DOWN;

	// --ブーストの経過時間[s]-- //
	rotateTimer_ = 0.0f;

	// --ブーストが始まった時の時間-- //
	rotateStartTime_ = 0;

	// --空中にいるか-- //
	isAir_ = false;

	// --空中キックができるか-- //
	isAirKickActive_ = true;

	// --衝突判定を行うか-- //
	isColActive_ = false;

	isReaction_ = false;

	reactionCount_ = 0;

	material.SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
}

// --更新処理-- //
void Player::Update(NDX12* dx12, XMMATRIX matView, XMMATRIX matProjection) {
	particle->WallHit(isParticle, directionX_, matView, matProjection, object_.get());
	isParticle = false;

	scrollY_ = 0.0f;

#pragma region 通常状態かつ壁伝い中の処理
	if (state_ == Start) {

	}

	else if (state_ == Goal) {
		// --ゴール状態になってからの経過時間-- //
		float nowCount = static_cast<float>(Util::GetNowCount());
		float nowTime = (nowCount - easeStartCount_) / 1000.0f;

		float easeRota = nowTime / goalEaseTime_;
		easeRota = Util::Clamp(easeRota, 1.0f, 0.0f);

		speedY_ = 0.0f;

		object_->position.x = Util::EaseOutCubic(easeStartPosX_, easeEndPosX_, easeRota);

		object_->rotation.z += rotaSpeed;
	}

	else if (state_ == DeathReaction) {
		// --死亡状態になってからの経過時間-- //
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
		isParticle = true;	//パーティクル出せ～

		isColActive_ = true;

		// --[SPACE]を押したら-- //
		if (NInput::IsKeyTrigger(DIK_SPACE)) {
			audio->PlayWave(soundData[0], false,2.0f);

			isReaction_ = true;

			// --左右の向きを変える-- //
			ChangeDireX();

			// --状態を変える-- //
			state_ = NormalAir;// -> 通常状態かつ空中

			// --X軸の速度を変える-- //
			speedX_ = wallKickSpeedX_;// --壁キックしたときの速度-- //

			// --Y軸の移動方向が上だったら-- //
			if (directionY_ == UP) {
				speedY_ -= accelerationY_ * 2.0f;
			}

			// --Y軸の移動方向が下だったら-- //
			else if (directionY_ == DOWN) {
				speedY_ += accelerationY_;
			}
		}

		// --減速処理-- //
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

#pragma region 通常状態かつ空中にいる
	else if (state_ == NormalAir) {

	}
#pragma endregion

#pragma region 回転状態かつ壁伝い中
	else if (state_ == RotateWallHit) {
		// --[SPACE]を押したら-- //
		if (NInput::IsKeyTrigger(DIK_SPACE)) {
			audio->PlayWave(soundData[0], false, 2.0f);

			isReaction_ = true;

			// --左右の向きを変える-- //
			ChangeDireX();

			// --状態を変える-- //
			state_ = NormalAir;// -> 通常状態かつ空中

			material.SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

			// --X軸の速度を変える-- //
			speedX_ = wallKickSpeedX_;// --壁キックしたときの速度-- //

			object_->rotation.z = 0.0f;
		}
		else {
			object_->rotation.z += rotaSpeed * directionX_;
		}

		// --ブースト状態になってからの経過時間-- //
		float nowCount = static_cast<float>(Util::GetNowCount());
		float nowTime = (nowCount - rotateStartTime_) / 1000.0f;

		// --指定されているブースト時間が過ぎたら-- //
		if (rotateTime_ <= nowTime) {
			// --ブースト状態から通常状態に変更-- //
			state_ = NormalWallHit;

			material.SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

			object_->rotation.z = 0.0f;
		}
	}
#pragma endregion

#pragma region 回転状態かつ空中にいる
	else if (state_ == RotateAir) {
		// --ブースト状態になってからの経過時間-- //
		float nowCount = static_cast<float>(Util::GetNowCount());
		float nowTime = (nowCount - rotateStartTime_) / 1000.0f;

		// --指定されているブースト時間が過ぎたら-- //
		if (rotateTime_ <= nowTime) {
			// --ブースト状態から通常状態に変更-- //
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

	// --プレイヤーのX軸に加算-- //
	object_->position.x += speedX_ * directionX_;

	// --プレイヤーの移動分スクロール-- //
	object_->position.y += speedY_ * directionY_;
	scrollY_ = speedY_ * directionY_;

	if (state_ != Start) {
		// --x座標が最低座標以下になったら-- //
		if (object_->position.x < minPosX_) {
			// --x座標を変更-- //
			object_->position.x = minPosX_;

			audio->PlayWave(soundData[1], false, 3.5f);

			// --状態を変更-- //
			if (state_ == RotateAir) {
				state_ = RotateWallHit;// -> 回転状態かつ壁伝い中
			}

			else if (state_ == NormalAir) {
				material.SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
				state_ = NormalWallHit;// -> 通常状態かつ壁伝い中
			}

			// --X軸の速度を変える-- //
			speedX_ = 0.0f;// -> 動かないように

			if (!isCameraShake_) {
				SetCamShakeState(true);
			}
		}

		// --x座標が最高座標以上になったら-- //
		else if (object_->position.x > maxPosX_) {
			// --X座標を変更-- //
			object_->position.x = maxPosX_;

			audio->PlayWave(soundData[1], false, 3.5f);

			if (state_ == RotateAir) {
				state_ = RotateWallHit;// -> 回転状態かつ壁伝い中
			}

			else if (state_ == NormalAir) {
				material.SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
				state_ = NormalWallHit;// -> 通常状態かつ壁伝い中
			}

			// --X軸の速度を変える-- //
			speedX_ = 0.0f;// -> 動かないように

			if (!isCameraShake_) {
				SetCamShakeState(true);
			}
		}
	}

	object_->UpdateMatrix(matView, matProjection);
}

// --描画処理-- //
void Player::Draw(NDX12* dx12, NCube* cube) {
	particle->Draw(dx12);
	deathParticle->Draw(dx12);

	// --オブジェクト描画-- //
	if (state_ != Death && state_ != DeathReaction)
	{
		object_->CommonBeginDraw(dx12->GetCommandList(), NSceneManager::GetPipeline3d()->pipelineSet.pipelineState, NSceneManager::GetPipeline3d()->pipelineSet.rootSig.entity, dx12->GetSRVHeap());
		object_->Draw(dx12->GetCommandList(), material, dx12->GetSRVHeap(), cube->vbView, cube->ibView, cube->numIB, NSceneManager::GetTex()[0].incrementSize);
	}
}

// --終了処理-- //
void Player::Finalize() {
	delete audio;
}

// --プレイヤーの状態を変更-- //
int Player::GetState() { return state_; }

// --プレイヤーの状態を変更-- //
void Player::SetState(int state) { state_ = state; }

void Player::SetGame() {
	state_ = NormalWallHit;
	speedY_ = defSpeedY_;
}
//
// --死亡状態に変更-- //
void Player::SetDeath() {
	state_ = DeathReaction;
	speedY_ = 0.0f;
	speedX_ = 0.0f;
	deathStartCount_ = Util::GetNowCount();
	isColActive_ = false;
}

// --X軸の向きを変える-- //
void Player::ChangeDireX() {
	if (directionX_ == LEFT) directionX_ = RIGHT;
	else directionX_ = LEFT;
}

void Player::ChangeDireY() {
	if (directionY_ == UP) directionY_ = DOWN;
	else directionY_ = UP;
}

// --回転状態にする-- //
void Player::SetRotate() {
	speedY_ = maxSpeedY_;
	material.SetColor({ 0.95f, 0.1f, 0.1f, 1.0f });

	if (state_ == NormalAir) state_ = RotateAir;
	else if (state_ == NormalWallHit) state_ = RotateWallHit;
	else if (state_ == RotateAir) state_ = RotateAir;
	else if (state_ == RotateWallHit) state_ = RotateWallHit;
	rotateStartTime_ = Util::GetNowCount();
}

// --ゴール状態にする-- //
void Player::SetGoal() {
	state_ = Goal;
	speedY_ = 5.0f;
	easeStartCount_ = Util::GetNowCount();
	easeStartPosX_ = object_->position.x;
	isColActive_ = false;
}

// --構造体で返す-- //
BoxObj Player::GetBoxObj() {
	return { {object_->position.x, object_->position.y}, 24.0f };
}