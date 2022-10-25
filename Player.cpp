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
	object_->position.x = minPosX_;
	object_->position.y = 0.0f;
	object_->rotation.z = 0.0f;
	// --プレイヤーの状態-- //
	state_ = NormalWallHit;

	// --速度-- //
	speedX_ = 0.0f;
	speedY_ = defSpeedY_;

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
	isColActive_ = true;

	deathStartCount_ = 0;
}

// --初期化処理-- //
void Player::Initialize(NDX12* dx12) {
	//マテリアル(定数バッファ)
	material.Initialize(dx12->GetDevice());
	material.SetColor({ 1,1,1,1 });

	// --プレイヤーオブジェクト(定数バッファ)-- //
	object_ = std::make_unique<NObj3d>();
	object_->Initialize(dx12->GetDevice());
	object_->texNum = PLAYER;
	object_->scale = { 24.0f, 24.0f, 24.0f };
	object_->position.x = minPosX_;

	//パーティクル
	particle = std::make_unique<NParticle>();
	particle->Initialize(dx12->GetDevice());

	// --プレイヤーの状態-- //
	state_ = NormalWallHit;

	// --速度-- //
	speedX_ = 0.0f;
	speedY_ = defSpeedY_;

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
	isColActive_ = true;

	speedSprite[0] = std::make_unique<NSprite>();
	speedSprite[0]->texNum = static_cast<int>(NUMBER);

	speedSprite[1] = std::make_unique<NSprite>();
	speedSprite[1]->texNum = static_cast<int>(NUMBER);

	speedSprite[2] = std::make_unique<NSprite>();
	speedSprite[2]->texNum = static_cast<int>(NUMBER);
}

// --更新処理-- //
void Player::Update(XMMATRIX matView, XMMATRIX matProjection) {
	particle->WallHit(isParticle, directionX_, matView, matProjection, object_.get());
	isParticle = false;

	scrollY_ = 0.0f;
	if (state_ != Goal && state_ != Death && state_ != DeathResult) {
#pragma region 通常状態かつ壁伝い中の処理
		if (state_ == NormalWallHit) {
			isParticle = true;	//パーティクル出せ〜

			// --[SPACE]を押したら-- //
			if (NInput::IsKeyTrigger(DIK_SPACE)) {
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
				// --左右の向きを変える-- //
				ChangeDireX();

				// --状態を変える-- //
				state_ = NormalAir;// -> 通常状態かつ空中

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

				object_->rotation.z = 0.0f;
			}

			object_->rotation.z += rotaSpeed * directionX_;
		}
#pragma endregion

		// --プレイヤーのX軸に加算-- //
		object_->position.x += speedX_ * directionX_;

		// --プレイヤーの移動分スクロール-- //
		object_->position.y += speedY_ * directionY_;
		scrollY_ = speedY_ * directionY_;

		// --x座標が最低座標以下になったら-- //
		if (object_->position.x < minPosX_) {
			// --x座標を変更-- //
			object_->position.x = minPosX_;

			// --状態を変更-- //
			if (state_ == RotateAir) {
				state_ = RotateWallHit;// -> 回転状態かつ壁伝い中
			}

			else if (state_ == NormalAir) {
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

			if (state_ == RotateAir) {
				state_ = RotateWallHit;// -> 回転状態かつ壁伝い中
			}

			else if (state_ == NormalAir) {
				state_ = NormalWallHit;// -> 通常状態かつ壁伝い中
			}

			// --X軸の速度を変える-- //
			speedX_ = 0.0f;// -> 動かないように

			if (!isCameraShake_) {
				SetCamShakeState(true);
			}
		}
	}

	else if (state_ == Goal) {
		// --ゴール状態になってからの経過時間-- //
		float nowCount = static_cast<float>(Util::GetNowCount());
		float nowTime = (nowCount - easeStartCount_) / 1000.0f;

		float easeRota = nowTime / goalEaseTime_;
		easeRota = Util::Clamp(easeRota, 1.0f, 0.0f);

		object_->position.x = Util::EaseOutCubic(easeStartPosX_, easeEndPosX_, easeRota);

		object_->rotation.z += rotaSpeed;
	}

	else if (state_ == Death || state_ == DeathResult) {
		// --ゴール状態になってからの経過時間-- //
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

// --描画処理-- //
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


	// --オブジェクト描画-- //
	object_->CommonBeginDraw(dx12->GetCommandList(), NSceneManager::GetPipeline3d()->pipelineSet.pipelineState, NSceneManager::GetPipeline3d()->pipelineSet.rootSig.entity, dx12->GetSRVHeap());
	object_->Draw(dx12->GetCommandList(), material, dx12->GetSRVHeap(), cube->vbView, cube->ibView, cube->numIB, NSceneManager::GetTex()[0].incrementSize);

	particle->Draw(dx12);
}

// --終了処理-- //
void Player::Finalize() {

}

// --プレイヤーの状態を変更-- //
int Player::GetState() { return state_; }

// --プレイヤーの状態を変更-- //
void Player::SetState(int state) { state_ = state; }
//
// --死亡状態に変更-- //
void Player::SetDeath() {
	state_ = Death;
	deathStartCount_ = Util::GetNowCount();
}

float& Player::GetSpeedX() { return speedX_; }
float& Player::GetSpeedY() { return speedY_; }

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