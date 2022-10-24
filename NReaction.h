#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class NReaction
{
private:
	const float maxTime = 1.0f;		//全体の時間
	float t;						//経過時間
	float timeRate = 0.05f;			//1フレームごとの増加量
	bool isReaction1 = false;
	bool isReaction2 = false;

	float originScale = 1.0f;	//オブジェクトの元サイズ
	float scale = 1.0f;
	XMFLOAT3 objScale = {};

public:
	void Bounce(bool isReaction);
	XMFLOAT3 GetObjScale() { return objScale; }
	void SetIsReaction(bool isCol) { this->isReaction1 = isCol; }
	
private:
	float EaseInBack(float t);
	float EaseOutQuart(float t);
};