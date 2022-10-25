#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class NCamera
{
private:
	//�J����
	XMMATRIX matProjection;
	XMMATRIX matView;

	//�J����
	XMFLOAT3 eye = { 0, 100, -1500 };	//���_���W
	XMFLOAT3 target = { 0, 0, 0 };		//�����_���W
	XMFLOAT3 up = { 0, 1, 0 };			//������x�N�g��
	float angle = 0.0f;					//�J�����̉�]�p

	int shakeCount_{ 0 };

	XMFLOAT3 totalEye{ };
	XMFLOAT3 totalTarget{};

public:
	//�r���[�ϊ��s��쐬
	void CreateMatView();
	void CreateShakeMatView(float value, float value2);
	//�ˉe���e�ϊ�//
	void ProjectiveProjection();
	// --�c�X�N���[���̃X�s�[�h�擾-- //
	void SetScrollY(float scrollY);

	void SetShakeCount(int count) { shakeCount_ = count; }

	//�Z�b�^�[
	void SetEye(XMFLOAT3 eye) { this->eye = eye; }
	void SetTarget(XMFLOAT3 target) { this->target = target; }
	void SetUp(XMFLOAT3 up) { this->up = up; }

	//�Q�b�^�[
	XMMATRIX GetMatProjection() { return matProjection; }
	XMMATRIX GetMatView() { return matView; }
	XMFLOAT3 GetEye() { return eye; }
	XMFLOAT3 GetTarget() { return target; }
	int GetShakeCount(void) { return shakeCount_; }
};