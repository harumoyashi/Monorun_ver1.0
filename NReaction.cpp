#include "NReaction.h"

void NReaction::Bounce(bool isReaction)
{
	//リアクションフラグ立てる
	if (isReaction)
	{
		isReaction1 = true;
		originScale = objScale.x;
	}
	//リアクションフラグ立ってたら
	if (isReaction1)
	{
		if (t <= maxTime)
		{
			t += timeRate;
			//最後のoriginScale以外はプラスする大きさなので、
			//変えたければ別変数作って変えた方がいい
			scale = originScale * EaseInBack(t) + originScale;
			objScale = { scale,scale,scale };	//大きさ適用
		}
		else
		{
			t = 0.0f;
			isReaction1 = false;
			isReaction2 = true;	//次のリアクションへ
		}
	}

	if (isReaction2)
	{
		if (t <= maxTime)
		{
			t += timeRate;
			//最後のoriginScale以外はプラスする大きさなので、
			//変えたければ別変数作って変えた方がいい
			scale = originScale - originScale * EaseOutQuart(t) + originScale;
			objScale = { scale,scale,scale };	//大きさ適用
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
