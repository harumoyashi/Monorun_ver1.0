#pragma once
#include "NVector2.h"

enum class EffectType {
	BToT
};

class Particle
{
public: // 関数
	Particle(NVector2 pos, float radius, int indexType);
	~Particle();

	// 基本
	void Activate();
	void Reset();

	//setter
	void SetRadius(float radius) { radius_ = radius; }
	void SetPos(NVector2 pos) { pos_ = pos; }

	// getter
	NVector2& GetPos() { return pos_; }

	// 種類
	void InOutBoxAll();

private: // 変数
// 座標
	NVector2 pos_{};

	const NVector2 startPos_{};

	int effectType_{ 0 };
	// 半径
	float radius_{ 0.0f };

	const int startRadius_{};

	// effect開始時点でのpc内部の時間を記録
	int startTime_{ 0 };

	// effectを開始してからの経過時間を記録
	float elapsedTime_{ 0.0f };

	// 再生の有無
	bool isPlay_ = false;
};

