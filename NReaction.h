#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class NReaction
{
private:
	const float maxTime = 1.0f;		//�S�̂̎���
	float t;						//�o�ߎ���
	float timeRate = 0.08f;			//1�t���[�����Ƃ̑�����
	bool isReaction1 = false;		//1�ڂ̃��A�N�V�����t���O(�傫���Ȃ�)
	bool isReaction2 = false;		//2�ڂ̃��A�N�V�����t���O(�������Ȃ�)

	float originScale = 1.0f;	//�I�u�W�F�N�g�̌��T�C�Y(1��)
	float scale = 1.0f;			//���A�N�V�����ɂ���ĕύX����T�C�Y
	XMFLOAT3 objScale = {};		//3������scale�܂Ƃ߂邽�߂̂��

public:
	//�u���b�N���v���C���[��e�����A�N�V����
	//�����̃t���O�����ł���������Ō�܂Ŏ��s����
	void Bounce(bool isReaction);
	//���A�N�V�����ŕύX����scale��n���p
	XMFLOAT3 GetObjScale() { return objScale; }
	//���A�N�V��������I�u�W�F�N�g�̌���scale������Ă���p
	void SetObjScale(XMFLOAT3 scale) { this->objScale = scale; }
	
private:
	//�C�[�W���O�Q//
	float EaseInBack(float t);
	float EaseOutQuart(float t);
};