#include "Effect.h"
#include "NDX12.h"
#include "Util.h"
#include "NWindows.h"
#include "NSceneManager.h"

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
		particles_.push_back(std::make_unique<Particle>(NVector2{ NWindows::win_width / 2, 0 }, particleRaidus_, static_cast<int>(EffectType::CToA)));
		particles_.front().get()->Init();
		particles_.front().get()->Activate();
	}
	else if (effectType_ == static_cast<int>(EffectType::Explode)) {
		material_.Initialize(ndx12_->GetDevice());

		c_ = std::make_unique<NCube>();
		c_->Initialize(ndx12_->GetDevice(), CRYSTAL);

		objs_.push_back(std::make_unique<NObj3d>());

		for (int i = 0; i < objs_.size(); i++) {
			objs_.at(i)->Initialize(ndx12_->GetDevice());
			objs_.at(i)->texNum = WHITEPNG;
			objs_.at(i)->scale = { 32.0f,32.0f,32.0f };
			objs_.at(i)->position = { 0.0f,-100.0f,0.0f };
			objs_.at(i)->UpdateMatrix();
		}
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

void Effect::ExpandSquareUpdate(void)
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
		if (1.7f <= elapsedTime_ /*&& elapsedTime_ <= 2.5f*/) {
			//particles_.front().get()->SetPos(NVector2{
			//	particles_.front().get()->GetPos().x,
			//	InOutBack((float)frameCount_, 30, NWindows::win_height + 30, NWindows::win_height / 2) }
			//);
			//frameCount_++;
			Reset();
		}

		// --指定されている時間が過ぎたら-- //
		if (2.5f <= elapsedTime_) {
		}
	}
}

void Effect::ExpandSquareDraw(void)
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

void Effect::BrokenDeathBlockUpdate(XMMATRIX matView, XMMATRIX matProjection)
{
	if (isPlay_) {
		//objs_.at(0)->position = {OutExpo()}
		for (int i = 0; i < objs_.size(); i++) {
			objs_.at(i)->UpdateMatrix(matView, matProjection);
		}
	}
}

void Effect::BrokenDeathBlockDraw(void)
{
	if (isPlay_) {
		for (int i = 0; i < objs_.size(); i++) {
			objs_.at(i)->CommonBeginDraw(ndx12_->GetCommandList(), NSceneManager::GetPipeline3d()->pipelineSet.pipelineState, NSceneManager::GetPipeline3d()->pipelineSet.rootSig.entity, ndx12_->GetSRVHeap());
			objs_.at(i)->Draw(ndx12_->GetCommandList(), material_, ndx12_->GetSRVHeap(), c_->vbView, c_->ibView, c_->numIB, NSceneManager::GetTex()[0].incrementSize);
		}
	}
}