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
	void Update(XMMATRIX& matView, XMMATRIX& matProjection, XMFLOAT3& eye, XMFLOAT3& target, XMFLOAT3& up);

private:
	// --コンストラクタ-- //
	Collision();

	/// --メンバ関数END-- ///
};