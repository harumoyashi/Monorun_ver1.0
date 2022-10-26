#pragma once
#include "Player.h"
#include "StageManager.h"
#include "Util.h"
#include "NParticle.h"
#include "NAudio.h"
#include <vector>

class Collision {
	/// --メンバ変数-- ///
public:

private:
	// --インスタンス-- //
	static Collision* myInstance_;

	Player* player_;

	StageManager* stage_;

	BoxObj oldObj_;

	NAudio* audio = nullptr;

	static const int maxSoundData = 10;
	uint32_t soundData[maxSoundData] = {};

	//std::vector<NParticle> particles;
	std::unique_ptr<NParticle[]> particles;

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

	void Reset();

	void Finalize();

	// --初期化処理-- //
	void Initialize();

	// --更新処理-- //
	void Update(NDX12* dx12, XMMATRIX matView, XMMATRIX matProjection);

	// --描画処理-- //
	void Draw(NDX12* dx12);

	float GetScrollY() { return scrollY_; }

	// cameraShake
	bool GetCamShake() { return isCameraShake_; }
	void SetCamShakeState(bool flag) { isCameraShake_ = flag; }

private:
	// --コンストラクタ-- //
	Collision();

	/// --メンバ関数END-- ///
};