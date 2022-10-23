#pragma once
#include "NVector2.h"
#include "NSprite.h"
#include <algorithm>
#include "NDX12.h"

enum class EffectType {
	CToA, // 中心から周囲に広がる
};

class Particle
{
public: // 関数
	Particle(NVector2 pos, float radius, int indexType);
	~Particle();

	// 基本
	void Init();
	void Activate();
	void Reset();

	//setter
	void SetRadius(float radius);
	void SetPos(NVector2 pos);

	// getter
	NVector2& GetPos() { return pos_; }

	// 種類
	void InOutBoxAllUpdate();
	void InOutBoxAllDraw();

private: // 変数

#pragma region インスタンス保持

	NDX12* ndx12_ = nullptr;

#pragma endregion

// 座標
	NVector2 pos_{};

	const NVector2 startPos_{};

	int effectType_{ 0 };
	// 半径
	float radius_{ 0.0f };

	const float startRadius_{};

	// effect開始時点でのpc内部の時間を記録
	int startTime_{ 0 };

	// effectを開始してからの経過時間を記録
	float elapsedTime_{ 0.0f };

	// 再生の有無
	bool isPlay_ = false;

	// -- 画像
	std::unique_ptr<NSprite> whiteBox_;
	std::unique_ptr<NSprite> blackBox_;
};

