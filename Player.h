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
#include "NCube.h"
#include "NPreDraw.h"
#include "Util.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

enum State {
	NormalWallHit,// -> 通常状態で壁伝い中
	NormalAir,// -> 通常状態で空中にいる
	RotateWallHit,// -> 回転状態で壁伝い中
	RotateAir,// -> 回転状態で空中にいる
	Death// --死亡状態
};

class Player {
	/// --メンバ変数-- ///
public:
	// --オブジェクト-- //
	NObj3d object_;// ------> オブジェクト(定数バッファ)

private:

#pragma region クラス宣言
	NMaterial material;				//マテリアル
#pragma endregion

	// --プレイヤーの状態-- //
	int state_;

#pragma region プレイヤーの速度変数
	// --実際に加算する速度変数-- //
	float speedX_;// -> X軸
	float speedY_;// -> Y軸
	const float defSpeedY_ = 2.5f;// -> Y軸基本速度

	// --回転速度-- //
	const float rotaSpeed = 60.0f;

	// --Y軸の最高速度-- //
	const float maxSpeedY_ = 20.0f;

	// --Y軸の最低速度-- //
	const float minSpeedY_ = 2.5f;

	// --壁キックの速度-- //
	const float wallKickSpeedX_ = 20.0f;

	// --Y軸に減速速値-- //
	const float decelerationY_ = 0.3f;

	// --Y軸の加速値-- //
	const float accelerationY_ = 2.0f;
#pragma endregion

#pragma region ブースト時間変数
	// --ブーストの時間[s]-- //
	const float rotateTime_ = 3.0f;

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
	float directionX_;
	float directionY_;

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
	void Update(XMMATRIX &matView, XMMATRIX &matProjection, XMFLOAT3 &eye, XMFLOAT3 &target, XMFLOAT3 &up);

	// --描画処理-- //
	void Draw(NDX12* dx12,NCube* cube);

	// --終了処理-- //
	void Finalize();

	// --プレイヤーの状態を参照-- //
	int GetState();

	// --プレイヤーの状態を変更-- //
	void SetState(int state);

	float& GetSpeedX();
	float& GetSpeedY();

	// --通常状態に変更-- //
	void SetNormal();

	// --ブースト状態に変更-- //
	void SetBoost();

	// --死亡状態に変更-- //
	void SetDeath();

	// --X軸の向きを変える-- //
	void ChangeDireX();
	void ChangeDireY();

	// --回転状態にする-- //
	void SetRotate();

	// --構造体で返す-- //
	BoxObj GetBoxObj();

private:

	//void Collision();

	/// --メンバ関数END-- ///
};