#pragma once
#include "Player.h"
#include "StageManager.h"
#include "Util.h"

class Collision {
	/// --メンバ変数-- ///
public:

private:
	// --インスタンス-- //
	static Collision* myInstance_;

	Player* player_;

	StageManager* stage_;

	BoxObj oldObj_;

	// --当たり判定をとるか-- //
	bool isCollision_ = true;

	// --ブロックのサイズ-- //
	float blockSize_ = 64.0f;

	// --Y軸スクロールの値-- //
	float scrollY_;

	// getter用
	bool isCameraShake_ = false;

	/// --メンバ変数END-- ///
	/// --------------- ///
	/// --メンバ関数-- ///
public:
	// --インスタンス取得-- //
	static Collision* GetInstance();

	// --メモリ解放-- //
	static void Release();

	// --デストラクタ-- //
	~Collision();

	// --初期化処理-- //
	void Initialize();

	// --更新処理-- //
	void Update(XMMATRIX matView, XMMATRIX matProjection);

	float GetScrollY() { return scrollY_; }

	// cameraShake
	bool GetCamShake() { return isCameraShake_; }
	void SetCamShakeState(bool flag) { isCameraShake_ = flag; }

private:
	// --コンストラクタ-- //
	Collision();

	/// --メンバ関数END-- ///
};