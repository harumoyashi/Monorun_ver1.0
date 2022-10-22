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
	frameCount_ = 0;
	activeCount_ = 0;
}

void Effect::ExpandSquare()
{
	if (isPlay_) {
		// --�G�t�F�N�g�J�n->�o�ߎ���-- //
		elapsedTime_ = (Util::GetNowCount() - startTime_) / 1000.0f;

		particles_.front().get()->InOutBoxAll();

		// 0.71 �� 0.9�Ŗ����Ă�B��particle.class���ύX���邱�ƁB

		if (elapsedTime_ <= 0.71f) {
			particles_.front().get()->SetPos(NVector2{
				particles_.front().get()->GetPos().x,
				OutBounce(frameCount_, 50, NWindows::win_height / 2, 0) }
			);
			frameCount_++;
		}

		// particle�̍Đ��I���܂őҋ@
		if (0.71f <= elapsedTime_ && elapsedTime_ <= 2.0f) {

		}

		// frameCount���Z�b�g
		if (2.0f <= elapsedTime_ && elapsedTime_ <= 2.1f) {
			frameCount_ = 0;
		}

		// �Đ��I�����ʊO�Ɉړ�
		if (2.1f <= elapsedTime_ && elapsedTime_ <= 4.2f) {
			particles_.front().get()->SetPos(NVector2{
				particles_.front().get()->GetPos().x,
				InOutBack(frameCount_, 30, NWindows::win_height + 20, 446.69) }
			);
			frameCount_++;
		}

		// --�w�肳��Ă��鎞�Ԃ��߂�����-- //
		if (4.2f <= elapsedTime_) {
			Reset();
		}
	}
}