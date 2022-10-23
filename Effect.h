#pragma once
#include "Particle.h"
#include <vector>
#include <algorithm>

class Effect
{
private:
	static constexpr float particleRaidus_{ 32.0f };

public: // 関数
	Effect(int indexType);
	~Effect();

	void Activate();
	void Reset();

	void ExpandSquareUpdate();
	void ExpandSquareDraw();

	// getter
	bool GetEffectPlay() { return isPlay_; }
	bool GetAllowChangeScene() { return isAllowChangeScene_; }

private: // 変数
#pragma region インスタンス保持

	NDX12* ndx12_ = nullptr;

#pragma endregion

	int effectType_{ 0 };
	std::vector<std::unique_ptr<Particle>> particles_{};
	int activeCount_{ 0 };

	// effect開始時点でのpc内部の時間を記録
	int startTime_{ 0 };

	// effectを開始してからの経過時間を記録
	float elapsedTime_{ 0.0f };

	// 再生の有無
	bool isPlay_ = false;

	// シーン切替してよいか
	bool isAllowChangeScene_ = false;

	// フレームカウンタ
	int frameCount_{ 0 };
};