#include "NReaction.h"

void NReaction::Bounce(bool isReaction)
{
	//���A�N�V�����t���O���Ă�
	if (isReaction)
	{
		isReaction1 = true;
		originScale = objScale.x;
	}
	//���A�N�V�����t���O�����Ă���
	if (isReaction1)
	{
		if (t <= maxTime)
		{
			t += timeRate;
			//�Ō��originScale�ȊO�̓v���X����傫���Ȃ̂ŁA
			//�ς�������Εʕϐ�����ĕς�����������
			scale = originScale * EaseInBack(t) + originScale;
			objScale = { scale,scale,scale };	//�傫���K�p
		}
		else
		{
			t = 0.0f;
			isReaction1 = false;
			isReaction2 = true;	//���̃��A�N�V������
		}
	}

	if (isReaction2)
	{
		if (t <= maxTime)
		{
			t += timeRate;
			//�Ō��originScale�ȊO�̓v���X����傫���Ȃ̂ŁA
			//�ς�������Εʕϐ�����ĕς�����������
			scale = originScale - originScale * EaseOutQuart(t) + originScale;
			objScale = { scale,scale,scale };	//�傫���K�p
		}
		else
		{
			t = 0.0f;
			isReaction2 = false;
		}
	}
}

float NReaction::EaseInBack(float t)
{
	const float c1 = 1.100;
	const float c3 = c1 + 1.0f;

	return c3 * t * t * t - c1 * t * t;
}

float NReaction::EaseOutQuart(float t)
{
	return 1 - pow(1 - t, 4);
}
