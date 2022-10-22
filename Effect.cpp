//#include "Effect.h"
//#include "NDX12.h"
//#include "Util.h"
//
//Effect::Effect(int indexType) :
//	effectType_(indexType),
//	ndx12_{ NDX12::GetInstance() }
//{
//}
//
//Effect::~Effect()
//{
//}
//
//void Effect::Activate()
//{
//	if (effectType_ == static_cast<int>(EffectType::CToA)) {
//		particles_.push_back(Particle{ NVector2(NWindows::win_width / 2,0),10,static_cast<int>(EffectType::CToA) });
//		particles_.front().Activate();
//	}
//	startTime_ = Util::GetNowCount();
//	isPlay_ = true;
//}
//
//void Effect::Reset()
//{
//	startTime_ = 0;
//	elapsedTime_ = 0.0f;
//	isPlay_ = false;
//	frameCount_ = 0;
//	activeCount_ = 0;
//}
