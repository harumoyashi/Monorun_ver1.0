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
	NormalWallHit,// -> 通常状態で壁伝い中
	NormalAir,// -> 通常状態で空中にいる
	RotateWallHit,// -> 回転状態で壁伝い中
	RotateAir,// -> 回転状態で空中にいる
	DeathReaction,
	Death,
	Goal
};

class Player {
	/// --メンバ変数-- ///
public:
	// --オブジェクト-- //
	std::unique_ptr<NObj3d> object_;// ------> オブジェクト(定数バッファ)

private:

#pragma region クラス宣言
	NMaterial material;				//マテリアル

	std::unique_ptr<NParticle> particle;	//パーティクル
#pragma endregion

	// --プレイヤーの状態-- //
	int state_;

	bool isCollision_ = true;

#pragma region プレイヤーの速度変数
	// --実際に加算する速度変数-- //
	float speedX_;// -> X軸
	float speedY_;// -> Y軸
	const float defSpeedY_ = 2.5f;// -> Y軸基本速度

	// --回転速度-- //
	const float rotaSpeed = 18.0f;

	// --Y軸の最高速度-- //
	const float maxSpeedY_ = 20.0f;

	// --Y軸の最低速度-- //
	const float minSpeedY_ = 4.0f;

	// --壁キックの速度-- //
	const float wallKickSpeedX_ = 20.0f;

	// --Y軸に減速速値-- //
	const float decelerationY_ = 0.3f;

	// --Y軸の加速値-- //
	const float accelerationY_ = 2.0f;
#pragma endregion

#pragma region ブースト時間変数
	// --ブーストの時間[s]-- //
	const float rotateTime_ = 0.7f;

	// --ブーストの経過時間[s]-- //
	float rotateTimer_;

	// --ブーストが始まった時の時間-- //
	int rotateStartTime_;
#pragma endregion

	// --X座標の最高座標-- //
	const float maxPosX_ = 296.0f;

	// --X座標の最低座標-- //
	const float minPosX_ = -296.0f;

	// --空中にいるか-- //
	bool isAir_;

	// --空中キックができるか-- //
	bool isAirKickActive_;

	// --移動する向き-- //
	int directionX_;
	int directionY_;

	// --Y軸スクロールの速度-- //
	float scrollY_;

	// getter用
	bool isCameraShake_ = false;

	// --プレイヤーの衝突判定を行うか-- //
	bool isColActive_;

#pragma region ゴールした時の演出用
	// --イージング開始時のX軸座標-- //
	float easeStartPosX_;

	// --イージング終了時のX軸座標-- //
	const float easeEndPosX_ = 0.0f;

	// --イージング開始時のカウント-- //
	int easeStartCount_;

	// --イージング時間-- //
	const float goalEaseTime_ = 2.5f;
#pragma endregion

#pragma region 死亡時のタイマー
	// --死亡状態開始時のカウント-- //
	int deathStartCount_;

	const float deathWaitingTime = 3.0f;
#pragma endregion

	bool isParticle = false;	//パーティクル出すか否か

	/// --メンバ変数END-- ///
	/// --------------- ///
	/// --メンバ関数-- ///
public:
	// --インスタンス取得-- //
	static Player* GetInstance();

	// --リセット処理-- //
	void Reset();

	// --初期化処理-- //
	void Initialize(NDX12* dx12);

	// --更新処理-- //
	void Update(NDX12* dx12, XMMATRIX matView, XMMATRIX matProjection);

	// --描画処理-- //
	void Draw(NDX12* dx12,NCube* cube);

	// --終了処理-- //
	void Finalize();

	// --プレイヤーの状態を参照-- //
	int GetState();

	bool GetIsColActive() { return isColActive_; }

	// --プレイヤーの状態を変更-- //
	void SetState(int state);

	void SetGame();

	float GetSpeedX() { return speedX_; }
	float GetSpeedY() { return speedY_; }

	int GetDirectionY() { return directionY_; }

	// --死亡状態に変更-- //
	void SetDeath();

	// --X軸の向きを変える-- //
	void ChangeDireX();
	void ChangeDireY();

	// --回転状態にする-- //
	void SetRotate();

	// --ゴール状態にする-- //
	void SetGoal();

	// --構造体で返す-- //
	BoxObj GetBoxObj();

	// --Y軸スクロールの値を参照-- //
	float GetScrollY() { return scrollY_; }

	// cameraShake
	bool GetCamShake() { return isCameraShake_; }
	void SetCamShakeState(bool flag) { isCameraShake_ = flag; }

private:

	//void Collision();

	/// --メンバ関数END-- ///
};