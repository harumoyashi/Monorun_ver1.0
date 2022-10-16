#pragma once
#include "NDX12.h"

#include <wrl.h>

class NTexture
{
public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	TexMetadata metadata{};
	ScratchImage scratchImg{};

	D3D12_HEAP_PROPERTIES texHeapProp{};	//�q�[�v
	D3D12_RESOURCE_DESC texResDesc{};		//���\�[�X
	ComPtr<ID3D12Resource> texBuff;			//�e�N�X�`���o�b�t�@
	ScratchImage mipChain{};				//�~�b�v�}�b�v

	
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle;		//�V�F�[�_�[���\�[�X�r���[�n���h��
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};	//�ݒ�\����

	UINT incrementSize;	//���̃e�N�X�`�����̏ꏊ�ɐi�ނ��߂Ɏg��

public:
	//WIC�e�N�X�`���̃��[�h
	void Load(const wchar_t* pictureName);
	//�~�b�v�}�b�v����
	void CreateMipmap();

	//�q�[�v�ݒ�
	void SetTBHeap();
	//���\�[�X�ݒ�
	void SetTBResource();
	//�e�N�X�`���o�b�t�@�̐���
	void CreateTexBuff(ComPtr<ID3D12Device> device);
	//�~�b�v�}�b�v�f�[�^�̓]��
	void TexBuffMaping();
	//�V�F�[�_�[���\�[�X�r���[�ݒ�
	void SetSRV();
	//�V�F�[�_�[���\�[�X�r���[����
	void CreateSRV(ComPtr<ID3D12Device> device, ComPtr<ID3D12DescriptorHeap> srvHeap, int texNum);
};

