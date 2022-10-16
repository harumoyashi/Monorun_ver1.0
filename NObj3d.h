#pragma once
#include "NDX12.h"
#include "NMaterial.h"

#include<memory>
#include <wrl.h>

//�萔�o�b�t�@�p�f�[�^�\���́i3D�ϊ��s��j
struct  ConstBufferDataTransform
{
	XMMATRIX mat;	//3D�ϊ��s��
};

class NObj3d
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	D3D12_HEAP_PROPERTIES heapProp{};	//�q�[�v
	D3D12_RESOURCE_DESC resDesc{};		//���\�[�X

	ConstBufferDataTransform* constMapTransform;	//3D�ϊ��s��
	ComPtr<ID3D12Resource> constBuffTransform;		//�萔�o�b�t�@��GPU���\�[�X�̃|�C���^

	XMMATRIX matWorld{};	//3D�ϊ��s��
public:
	XMFLOAT3 scale = { 1.0f,1.0f,1.0f };	//�X�P�[�����O�{��
	XMFLOAT3 rotation = { 0.0f,0.0f,0.0f };	//��]�p
	XMFLOAT3 position = { 0.0f,0.0f,0.0f };	//���W

	NObj3d* parent = nullptr;	//�e�̃|�C���^

	int texNum = 0;	//�e�N�X�`���w��p

public:
#pragma region �������܂��
	//������
	void Initialize(ComPtr<ID3D12Device> device);

	//�q�[�v�ݒ�
	void SetCBHeap();
	//���\�[�X�ݒ�
	void SetCBResource();
	//�萔�o�b�t�@�̐���
	void CreateCB(ComPtr<ID3D12Device> device);
	//�萔�o�b�t�@�̃}�b�s���O
	void MappingCB();
#pragma endregion
#pragma region �X�V�܂��
	//�L�[�{�[�h����
	void MoveKey();
	//���[���h�s��̍���
	void UpdateMatrix();
	//�萔�o�b�t�@�֑��M
	void TransferMatrix(XMMATRIX matView, XMMATRIX matProjection);
#pragma endregion
#pragma region �`��܂��
	//���ʃO���t�B�b�N�X�R�}���h
	void CommonBeginDraw(ComPtr<ID3D12GraphicsCommandList> commandList,const ComPtr<ID3D12PipelineState> pipelineState,
		ComPtr<ID3D12RootSignature> rootSignature, ComPtr<ID3D12DescriptorHeap> srvHeap);
	//�`��
	void Draw(ComPtr<ID3D12GraphicsCommandList> commandList, NMaterial material, ComPtr<ID3D12DescriptorHeap> srvHeap,
		D3D12_VERTEX_BUFFER_VIEW vbView, D3D12_INDEX_BUFFER_VIEW ibView, UINT indexSize, UINT incrementSize);
	//�萔�o�b�t�@�r���[(CRV)�̐ݒ�R�}���h(�}�e���A��)
	void SetMaterialCBV(ComPtr<ID3D12GraphicsCommandList> commandList, NMaterial material);
	void SetSRVHeap(ComPtr<ID3D12DescriptorHeap> srvHeap, ComPtr<ID3D12GraphicsCommandList> commandList, UINT incrementSize);
	//���_�o�b�t�@�r���[�̐ݒ�R�}���h
	void SetVB(ComPtr<ID3D12GraphicsCommandList> commandList, D3D12_VERTEX_BUFFER_VIEW vbView);
	//�C���f�b�N�X�o�b�t�@�r���[�̐ݒ�R�}���h
	void SetIB(ComPtr<ID3D12GraphicsCommandList> commandList, D3D12_INDEX_BUFFER_VIEW ibView);
	//�萔�o�b�t�@�r���[(CRV)�̐ݒ�R�}���h(3D�ϊ��s��)
	void SetMatCBV(ComPtr<ID3D12GraphicsCommandList> commandList);
	//�`��R�}���h
	void DrawCommand(ComPtr<ID3D12GraphicsCommandList> commandList, UINT indexSize);
#pragma endregion
};