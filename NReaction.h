#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class NReaction
{
private:
	const float maxTime = 1.0f;		//�S�̂̎���
	float t;						//�o�ߎ���
	float timeRate = 0.05f;			//1�t���[�����Ƃ̑�����
	bool isReaction1 = false;
	bool isReaction2 = false;

	float originScale = 1.0f;	//�I�u�W�F�N�g�̌��T�C�Y
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