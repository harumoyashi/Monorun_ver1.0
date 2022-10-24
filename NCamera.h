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

public:
	//�r���[�ϊ��s��쐬
	void CreateMatView();
	//�ˉe���e�ϊ�//
	void ProjectiveProjection();

	//�Z�b�^�[
	void SetEye(XMFLOAT3 eye) { this->eye = eye; }
	void SetTarget(XMFLOAT3 target) { this->target = target; }
	void SetUp(XMFLOAT3 up) { this->up = up; }

	//�Q�b�^�[
	XMMATRIX GetMatProjection() { return matProjection; }
	XMMATRIX GetMatView() { return matView; }
};