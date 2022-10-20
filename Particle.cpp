#include "Particle.h"
#include "Util.h"

Particle::Particle(NVector2 pos, float radius, int indexType) :
	pos_(pos),
	startPos_(pos),
	radius_(radius),
	startRadius_(radius),
	effectType_(indexType)
{
}

Particle::~Particle()
{
}

void Particle::Activate()
{
	startTime_ = Util::GetNowCount();
	isPlay_ = true;
}

void Particle::InOutBoxAll()
{
	using namespace Util;
	if (isPlay_ && effectType_ == static_cast<int>(EffectType::BToT)) {
		// --エフェクト開始->経過時間-- //
		elapsedTime_ = (GetNowCount() - startTime_) / 1000.0f;

		if (elapsedTime_ <= 0.86f) {
			DrawBox(
				static_cast<int>(pos_.x) - radius_,
				static_cast<int>(pos_.y) - radius_,
				static_cast<int>(pos_.x) + radius_,
				static_cast<int>(pos_.y) + radius_,
				0xffffff, true
			);
			DrawBox(
				static_cast<int>(pos_.x) - (radius_ - 2),
				static_cast<int>(pos_.y) - (radius_ - 2),
				static_cast<int>(pos_.x) + (radius_ - 2),
				static_cast<int>(pos_.y) + (radius_ - 2),
				0x000000, true
			);
		}
	}
}
