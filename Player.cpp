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
	object_.position.x = minPosX_;
	object_.position.y = 0.0f;
}

// --初期化処理-- //
void Player::Initialize(NDX12* dx12) {
	//マテリアル(定数バッファ)
	material.Initialize(dx12->GetDevice());
	material.SetColor({ 1,1,1,1 });

	// --プレイヤーオブジェクト(定数バッファ)-- //
	object_.Initialize(dx12->GetDevice());
	object_.texNum = PLAYER;
	object_.scale = { 24.0f, 24.0f, 24.0f };
	object_.position.x = minPosX_;

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
}

// --更新処理-- //
void Player::Update(XMMATRIX& matView, XMMATRIX& matProjection, XMFLOAT3& eye, XMFLOAT3& target, XMFLOAT3& up) {

#pragma region 通常状態かつ壁伝い中の処理
	if (state_ == NormalWallHit) {
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

			object_.rotation.z = 0.0f;
		}
		else {
			object_.rotation.z += rotaSpeed * directionX_;
		}

		// --ブースト状態になってからの経過時間-- //
		float nowCount = Util::GetNowCount();
		float nowTime = (nowCount - rotateStartTime_) / 1000.0f;

		// --指定されているブースト時間が過ぎたら-- //
		if (rotateTime_ <= nowTime) {
			// --ブースト状態から通常状態に変更-- //
			state_ = NormalWallHit;

			object_.rotation.z = 0.0f;
		}
	}
#pragma endregion

#pragma region 回転状態かつ空中にいる
	else if (state_ == RotateAir) {
		// --ブースト状態になってからの経過時間-- //
		float nowCount = Util::GetNowCount();
		float nowTime = (nowCount - rotateStartTime_) / 1000.0f;

		// --指定されているブースト時間が過ぎたら-- //
		if (rotateTime_ <= nowTime) {
			// --ブースト状態から通常状態に変更-- //
			state_ = NormalAir;

			object_.rotation.z = 0.0f;
		}

		object_.rotation.z += rotaSpeed * directionX_;
	}
#pragma endregion

	// --プレイヤーのX軸に加算-- //
	object_.position.x += speedX_ * directionX_;

	// --プレイヤーの移動分スクロール-- //
	object_.position.y += speedY_ * directionY_;
	eye.y += speedY_ * directionY_;
	target.y += speedY_ * directionY_;

	// --x座標が最低座標以下になったら-- //
	if (object_.position.x < minPosX_ ) {
		// --x座標を変更-- //
		object_.position.x = minPosX_;

		// --状態を変更-- //
		if (state_ == RotateAir) {
			state_ = RotateWallHit;// -> 回転状態かつ壁伝い中
		}

		else if (state_ == NormalAir) {
			state_ = NormalWallHit;// -> 通常状態かつ壁伝い中
		}

		// --X軸の速度を変える-- //
		speedX_ = 0.0f;// -> 動かないように
	}

	// --x座標が最高座標以上になったら-- //
	else if (object_.position.x > maxPosX_) {
		// --X座標を変更-- //
		object_.position.x = maxPosX_;

		if (state_ == RotateAir) {
			state_ = RotateWallHit;// -> 回転状態かつ壁伝い中
		}

		else if (state_ == NormalAir) {
			state_ = NormalWallHit;// -> 通常状態かつ壁伝い中
		}

		// --X軸の速度を変える-- //
		speedX_ = 0.0f;// -> 動かないように
	}

	//ビュー変換行列作成
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
	object_.UpdateMatrix(matView, matProjection);
}

// --描画処理-- //
void Player::Draw(NDX12* dx12,NCube* cube) {
	// --オブジェクト描画-- //
	object_.CommonBeginDraw(dx12->GetCommandList(), NSceneManager::GetPipeline3d()->pipelineSet.pipelineState, NSceneManager::GetPipeline3d()->pipelineSet.rootSig.entity, dx12->GetSRVHeap());
	object_.Draw(dx12->GetCommandList(), material, dx12->GetSRVHeap(), cube->vbView, cube->ibView, cube->numIB, NSceneManager::GetTex()[0].incrementSize);
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
	NSceneManager::SetScene(TITLESCENE);
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

// --構造体で返す-- //
BoxObj Player::GetBoxObj() {
	return { {object_.position.x, object_.position.y}, 24.0f };
}