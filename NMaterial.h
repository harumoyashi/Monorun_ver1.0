#pragma once
#include "NDX12.h"

#include<memory>
#include <wrl.h>
template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

//�萔�o�b�t�@�p�\����
struct ConstBufferDataMaterial
{
	XMFLOAT4 color;	//�F(RGBA)
};

class NMaterial
{
private:
	D3D12_HEAP_PROPERTIES heapProp{};	//�q�[�v
	D3D12_RESOURCE_DESC resDesc{};		//���\�[�X

public:
	ComPtr<ID3D12Resource> constBuffMaterial;
	//std::unique_ptr<ConstBufferDataMaterial> constMapMaterial;
	ConstBufferDataMaterial* constMapMaterial;		//�F(RGBA)

public:
	~NMaterial();
#pragma region �������܂��
	//������
	void Initialize(ComPtr<ID3D12Device> device);
	//�q�[�v�ݒ�
	void SetHeap();
	//���\�[�X�ݒ�
	void SetResource();
	//�萔�o�b�t�@�̐���
	void CreateCB(ComPtr<ID3D12Device> device);
	//�萔�o�b�t�@�̃}�b�s���O
	void MappingCB();
	//�F���]��
	void TransferColor();
#pragma endregion
#pragma region �`��܂��
	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	void SetCBV(ComPtr<ID3D12GraphicsCommandList> commandList);
#pragma endregion
};

