#include "Particle.h"
#include "Util.h"
#include "NSceneManager.h"

#define DIFF_INOUT_BToT 4.0f

Particle::Particle(NVector2 pos, float radius, int indexType) :
	pos_(pos),
	startPos_(pos),
	radius_(radius),
	startRadius_(radius),
	effectType_(indexType),
	ndx12_{ NDX12::GetInstance() }
{
}

Particle::~Particle()
{
}

void Particle::Init()
{
	if (effectType_ == static_cast<int>(EffectType::CToA)) {
		// -- 元の画像のサイズに合わせる場合は実引数ラストに.texBuffを指定する
		// 白
		whiteBox_ = std::make_unique<NSprite>();
		whiteBox_->texNum = WHITEPNG;
		whiteBox_->CreateSprite(ndx12_->GetDevice(), NSceneManager::GetTex()[whiteBox_->texNum].texBuff);
		whiteBox_->size = { radius_ * 2, radius_ * 2 };
		whiteBox_->TransferVertex();
		whiteBox_->position.x = NWindows::win_width / 2;
		whiteBox_->position.y = NWindows::win_height / 2;
		whiteBox_->UpdateMatrix();
		// 黒
		blackBox_ = std::make_unique<NSprite>();
		blackBox_->texNum = WHITEPNG;
		blackBox_->CreateSprite(ndx12_->GetDevice(), NSceneManager::GetTex()[whiteBox_->texNum].texBuff);
		blackBox_->size = { radius_ * 2 - DIFF_INOUT_BToT, radius_ * 2 - DIFF_INOUT_BToT };
		blackBox_->SetColor(0.0f, 0.0f, 0.0f, 1.0f);
		blackBox_->TransferVertex();
		blackBox_->position.x = NWindows::win_width / 2;
		blackBox_->position.y = NWindows::win_height / 2;
		blackBox_->UpdateMatrix();
	}
}

void Particle::Activate()
{
	startTime_ = Util::GetNowCount();
	isPlay_ = true;
}

void Particle::Reset()
{
	pos_ = startPos_;
	radius_ = startRadius_;
	startTime_ = 0;
	elapsedTime_ = 0.0f;
	isPlay_ = false;
}

void Particle::SetRadius(float radius)
{
	radius_ = radius;

	if (effectType_ == static_cast<int>(EffectType::CToA)) {
		whiteBox_->size = { radius_ * 2, radius_ * 2 };
		whiteBox_->TransferVertex();
		blackBox_->size = { radius_ * 2 - DIFF_INOUT_BToT, radius_ * 2 - DIFF_INOUT_BToT };
		blackBox_->TransferVertex();
	}
}

void Particle::SetPos(NVector2 pos)
{
	pos_ = pos;

	if (effectType_ == static_cast<int>(EffectType::CToA)) {
		whiteBox_->position.x = pos.x;
		whiteBox_->position.y = pos.y;
		blackBox_->position.x = pos.x;
		blackBox_->position.y = pos.y;

		whiteBox_->UpdateMatrix();
		blackBox_->UpdateMatrix();
	}
}

void Particle::InOutBoxAll()
{
	using namespace Util;
	if (isPlay_ && effectType_ == static_cast<int>(EffectType::CToA)) {
		// --エフェクト開始->経過時間-- //
		elapsedTime_ = (GetNowCount() - startTime_) / 1000.0f;

		// 拡大まで待機
		if (elapsedTime_ <= 0.71f) {
			whiteBox_->CommonBeginDraw(ndx12_->GetCommandList(), NSceneManager::GetPipelineSprite()->pipelineSet.pipelineState, NSceneManager::GetPipelineSprite()->pipelineSet.rootSig.entity, ndx12_->GetSRVHeap());
			whiteBox_->Draw(ndx12_->GetSRVHeap(), NSceneManager::GetTex()[0].incrementSize, ndx12_->GetCommandList());
			blackBox_->CommonBeginDraw(ndx12_->GetCommandList(), NSceneManager::GetPipelineSprite()->pipelineSet.pipelineState, NSceneManager::GetPipelineSprite()->pipelineSet.rootSig.entity, ndx12_->GetSRVHeap());
			blackBox_->Draw(ndx12_->GetSRVHeap(), NSceneManager::GetTex()[0].incrementSize, ndx12_->GetCommandList());
		}

		// 拡大
		if (0.71f <= elapsedTime_ && elapsedTime_ <= 1.1f) {
			whiteBox_->CommonBeginDraw(ndx12_->GetCommandList(), NSceneManager::GetPipelineSprite()->pipelineSet.pipelineState, NSceneManager::GetPipelineSprite()->pipelineSet.rootSig.entity, ndx12_->GetSRVHeap());
			whiteBox_->Draw(ndx12_->GetSRVHeap(), NSceneManager::GetTex()[0].incrementSize, ndx12_->GetCommandList());
			blackBox_->CommonBeginDraw(ndx12_->GetCommandList(), NSceneManager::GetPipelineSprite()->pipelineSet.pipelineState, NSceneManager::GetPipelineSprite()->pipelineSet.rootSig.entity, ndx12_->GetSRVHeap());
			blackBox_->Draw(ndx12_->GetSRVHeap(), NSceneManager::GetTex()[0].incrementSize, ndx12_->GetCommandList());

			radius_ += static_cast<int>(radius_++ / 2);
			whiteBox_->size = { radius_ * 2, radius_ * 2 };
			whiteBox_->TransferVertex();
			blackBox_->size = { radius_ * 2 - DIFF_INOUT_BToT, radius_ * 2 - DIFF_INOUT_BToT };
			blackBox_->TransferVertex();
		}

		// 縮小まで待機
		if (1.1f <= elapsedTime_ && elapsedTime_ <= 1.3f) {
			whiteBox_->CommonBeginDraw(ndx12_->GetCommandList(), NSceneManager::GetPipelineSprite()->pipelineSet.pipelineState, NSceneManager::GetPipelineSprite()->pipelineSet.rootSig.entity, ndx12_->GetSRVHeap());
			whiteBox_->Draw(ndx12_->GetSRVHeap(), NSceneManager::GetTex()[0].incrementSize, ndx12_->GetCommandList());
			blackBox_->CommonBeginDraw(ndx12_->GetCommandList(), NSceneManager::GetPipelineSprite()->pipelineSet.pipelineState, NSceneManager::GetPipelineSprite()->pipelineSet.rootSig.entity, ndx12_->GetSRVHeap());
			blackBox_->Draw(ndx12_->GetSRVHeap(), NSceneManager::GetTex()[0].incrementSize, ndx12_->GetCommandList());
		}

		// 縮小
		if (1.3f <= elapsedTime_ && elapsedTime_ <= 1.7f) {
			whiteBox_->CommonBeginDraw(ndx12_->GetCommandList(), NSceneManager::GetPipelineSprite()->pipelineSet.pipelineState, NSceneManager::GetPipelineSprite()->pipelineSet.rootSig.entity, ndx12_->GetSRVHeap());
			whiteBox_->Draw(ndx12_->GetSRVHeap(), NSceneManager::GetTex()[0].incrementSize, ndx12_->GetCommandList());
			blackBox_->CommonBeginDraw(ndx12_->GetCommandList(), NSceneManager::GetPipelineSprite()->pipelineSet.pipelineState, NSceneManager::GetPipelineSprite()->pipelineSet.rootSig.entity, ndx12_->GetSRVHeap());
			blackBox_->Draw(ndx12_->GetSRVHeap(), NSceneManager::GetTex()[0].incrementSize, ndx12_->GetCommandList());

			if (radius_ >= startRadius_)
				radius_ -= static_cast<int>(radius_-- / 2);

			whiteBox_->size = { radius_ * 2, radius_ * 2 };
			whiteBox_->TransferVertex();
			blackBox_->size = { radius_ * 2 - DIFF_INOUT_BToT, radius_ * 2 - DIFF_INOUT_BToT };
			blackBox_->TransferVertex();
		}

		// 終了まで待機
		if (1.7f <= elapsedTime_ && elapsedTime_ <= 2.5f) {
			whiteBox_->CommonBeginDraw(ndx12_->GetCommandList(), NSceneManager::GetPipelineSprite()->pipelineSet.pipelineState, NSceneManager::GetPipelineSprite()->pipelineSet.rootSig.entity, ndx12_->GetSRVHeap());
			whiteBox_->Draw(ndx12_->GetSRVHeap(), NSceneManager::GetTex()[0].incrementSize, ndx12_->GetCommandList());
			blackBox_->CommonBeginDraw(ndx12_->GetCommandList(), NSceneManager::GetPipelineSprite()->pipelineSet.pipelineState, NSceneManager::GetPipelineSprite()->pipelineSet.rootSig.entity, ndx12_->GetSRVHeap());
			blackBox_->Draw(ndx12_->GetSRVHeap(), NSceneManager::GetTex()[0].incrementSize, ndx12_->GetCommandList());
		}

		// --指定されている時間が過ぎたら-- //
		if (2.5f <= elapsedTime_) {
			Reset();
		}
	}
}
