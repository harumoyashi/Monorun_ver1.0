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

	int impactTimer = 0; //�p�[�e�B�N���o������
	bool isImpact = false; //�p�[�e�B�N���o�����t���O

	int directionX = -1;	//1:�E���� -1:������

public:
	void Initialize(ComPtr<ID3D12Device> device);
	//�Ǔ`���̎��o��p�[�e�B�N��
	void WallHit(bool isParticle, int isDirectionR, XMMATRIX matView, XMMATRIX matProjection, NObj3d* player);
	void Draw(NDX12* dx12, const ComPtr<ID3D12PipelineState> pipelineState, ComPtr<ID3D12RootSignature> rootSignature, UINT incrementSize);
};