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

	XMFLOAT3 oldEye_{};

	int shakeCount_{ 5 };

public:
	//�r���[�ϊ��s��쐬
	void CreateMatView();
	//�ˉe���e�ϊ�//
	void ProjectiveProjection();
	// --�c�X�N���[���̃X�s�[�h�擾-- //
	void SetScrollY(float scrollY);

	void MemorizeEye(void) { oldEye_ = eye; }
	void RecoveryEye(void) { eye = oldEye_; }
	void SetShakeCount(int count) { shakeCount_ = count; }

	//�Z�b�^�[
	void SetEye(XMFLOAT3 eye) { this->eye = eye; }
	void SetTarget(XMFLOAT3 target) { this->target = target; }
	void SetUp(XMFLOAT3 up) { this->up = up; }

	//�Q�b�^�[
	XMMATRIX GetMatProjection() { return matProjection; }
	XMMATRIX GetMatView() { return matView; }
	XMFLOAT3 GetEye() { return eye; }
	int GetShakeCount(void) { return shakeCount_; }
};