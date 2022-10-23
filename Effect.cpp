#include "Effect.h"
#include "NDX12.h"
#include "Util.h"
#include "NWindows.h"

Effect::Effect(int indexType) :
	effectType_(indexType),
	ndx12_{ NDX12::GetInstance() }
{
}

Effect::~Effect()
{
}

void Effect::Activate()
{
	if (effectType_ == static_cast<int>(EffectType::CToA)) {
		particles_.push_back(std::make_unique<Particle>(NVector2{NWindows::win_width / 2, 0}, particleRaidus_, static_cast<int>(EffectType::CToA) ));
		particles_.front().get()->Init();
		particles_.front().get()->Activate();
	}
	startTime_ = Util::GetNowCount();
	isPlay_ = true;
}

void Effect::Reset()
{
	startTime_ = 0;
	elapsedTime_ = 0.0f;
	isPlay_ = false;
	isAllowChangeScene_ = false;
	isObserve1frame_ = false;
	frameCount_ = 0;
	activeCount_ = 0;
}

void Effect::ExpandSquareUpdate()
{
	if (isPlay_) {
		// --エフェクト開始->経過時間-- //
		elapsedTime_ = (Util::GetNowCount() - startTime_) / 1000.0f;

		particles_.front().get()->InOutBoxAllUpdate();

		// 0.71 か 0.9で迷ってる。※particle.classも変更すること。

		if (elapsedTime_ <= 0.71f) {
			particles_.front().get()->SetPos(NVector2{
				particles_.front().get()->GetPos().x,
				OutBounce((float)frameCount_, 50, NWindows::win_height / 2, 0) }
			);
			frameCount_++;
		}

		// エフェクトの座標自体は0.71 ~ 1.1まで待機
		if (0.71f <= elapsedTime_ && elapsedTime_ <= 1.1f) {

		}

		// particleの途中でシーン切替許可を出す
		if (1.1f <= elapsedTime_ && elapsedTime_ <= 1.2f && !isObserve1frame_ && !isAllowChangeScene_) {
			isObserve1frame_ = true;
			isAllowChangeScene_ = true;
		}

		// frameCountリセット
		if (1.5f <= elapsedTime_ && elapsedTime_ <= 1.6f) {
			frameCount_ = 0;
		}

		// 再生終了後画面外に移動
		if (1.6f <= elapsedTime_ && elapsedTime_ <= 2.5f) {
			particles_.front().get()->SetPos(NVector2{
				particles_.front().get()->GetPos().x,
				InOutBack((float)frameCount_, 30, NWindows::win_height + 20, NWindows::win_height / 2) }
			);
			frameCount_++;
		}

		// --指定されている時間が過ぎたら-- //
		if (2.5f <= elapsedTime_) {
			Reset();
		}
	}
}

void Effect::ExpandSquareDraw()
{
	if (isPlay_) {

		particles_.front().get()->InOutBoxAllDraw();

		// 0.71 か 0.9で迷ってる。※particle.classも変更すること。

		if (elapsedTime_ <= 0.71f) {

		}

		// エフェクトの座標自体は0.71 ~ 1.5まで待機
		if (0.71f <= elapsedTime_ && elapsedTime_ <= 1.1f) {

		}

		// particleの途中でシーン切替許可を出す
		if (1.1f <= elapsedTime_ && elapsedTime_ <= 1.4f) {

		}

		// frameCountリセット
		if (1.5f <= elapsedTime_ && elapsedTime_ <= 1.6f) {

		}

		// 再生終了後画面外に移動
		if (1.6f <= elapsedTime_ && elapsedTime_ <= 2.5f) {

		}

		// --指定されている時間が過ぎたら-- //
		if (2.5f <= elapsedTime_) {
			//Reset();
		}
	}
}
