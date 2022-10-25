#pragma once
#include "Particle.h"
#include <vector>
#include <algorithm>
#include "NObj3d.h"
#include "NMaterial.h"
#include "NCube.h"

class Effect
{
private:
	static constexpr float particleRaidus_{ 32.0f };

public: // 関数
	Effect(int indexType);
	~Effect();

	void Activate();
	void Reset();

#pragma region Effect一覧
	void ExpandSquareUpdate(void);
	void ExpandSquareDraw(void);

	void BrokenDeathBlockUpdate(XMMATRIX matView, XMMATRIX matProjection);
	void BrokenDeathBlockDraw(void);
#pragma endregion

	// getter
	bool GetEffectPlay() { return isPlay_; }
	bool GetAllowChangeScene() { 
		bool tmp{ isAllowChangeScene_ && isObserve1frame_ };
		isObserve1frame_ = false;
		return tmp; 
	}

private: // 変数
#pragma region インスタンス保持

	NDX12* ndx12_ = nullptr;

#pragma endregion

	int effectType_{ 0 };
	std::vector<std::unique_ptr<Particle>> particles_{};
	int activeCount_{ 0 };

	NMaterial material_;
	std::vector<std::unique_ptr<NObj3d>> objs_;
	std::unique_ptr<NCube> c_;

	// effect開始時点でのpc内部の時間を記録
	int startTime_{ 0 };

	// effectを開始してからの経過時間を記録
	float elapsedTime_{ 0.0f };

	// 再生の有無
	bool isPlay_ = false;

	// シーン切替してよいか
	bool isAllowChangeScene_ = false;

	bool isObserve1frame_ = false;

	// フレームカウンタ
	int frameCount_{ 0 };
};