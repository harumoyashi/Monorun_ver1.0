#pragma once
#include "NObj3d.h"
#include "NMaterial.h"
#include "NCube.h"

class NParticle
{
private:
	//�I�u�W�F�N�g
	NMaterial material;				//�}�e���A��
	static const int maxObj = 50;	//�I�u�W�F�N�g��(������g���܂킷)
	std::unique_ptr<NObj3d> obj3d[maxObj];			//�I�u�W�F�N�g(�萔�o�b�t�@)
	std::unique_ptr<NCube> cube;	//�����̏��(���_�A�C���f�b�N�X)

	bool isActive = false;	//���ꗧ���Ă���菈��������
	const int maxTimer = 20;
	int timer = 0;

	float scale = 0.0f;
	float rot = 0.0f;
	float speedX = 0.0f;
	float speedY = 0.0f;
	float speedZ = 0.0f;

public:
	NParticle();
	~NParticle();
	//�����ăA���t�@�l���傢�����̔ėp�I�ȓz
	void Initialize(ComPtr<ID3D12Device> device);
	//�}�e���A���ƃ��f�����w��ł���
	void Initialize(ComPtr<ID3D12Device> device, int modelNum);
	//�����u���b�N�j�󂷂�ۂ̏�����
	void BlockBreakInitialize(ComPtr<ID3D12Device> device, int modelNum, XMFLOAT2 pos);
	//�Ǔ`���̎��o��p�[�e�B�N��
	void WallHit(bool isParticle, int isDirectionR, XMMATRIX matView, XMMATRIX matProjection, NObj3d* player);
	//�����u���b�N�j�󎞂̃p�[�e�B�N��
	void BlockBreak(int atOnce, XMMATRIX matView, XMMATRIX matProjection);
	void PlayerBreak(bool isParticle, int atOnce, XMMATRIX matView, XMMATRIX matProjection,NObj3d* obj,float maxTimer = 50);
	//�`��
	void Draw(NDX12* dx12);

	//�}�e���A�����Z�b�g
	void SetMaterial(NMaterial material) { this->material = material; }

	// --�p�[�e�B�N�����L�����擾-- //
	bool GetIsActive() { return isActive; }
};