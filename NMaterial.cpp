#include "NMaterial.h"


NMaterial::~NMaterial()
{
	//constMapTransform.reset();
	//constMapMaterial.reset();
}

void NMaterial::Initialize(ComPtr<ID3D12Device> device)
{
	SetHeap();
	SetResource();
	CreateCB(device);
	MappingCB();
	TransferColor();
}

void NMaterial::SetHeap()
{
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPU�ւ̓]���p
}

void NMaterial::SetResource()
{
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;	//256�o�C�g�A���C�������g
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
}

void NMaterial::CreateCB(ComPtr<ID3D12Device> device)
{
	HRESULT result;

	result = device->CreateCommittedResource(
		&heapProp,	//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc,		//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffMaterial)
	);
	assert(SUCCEEDED(result));
}

void NMaterial::MappingCB()
{
	HRESULT result;

	result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial);	//�}�b�s���O
	assert(SUCCEEDED(result));
}

void NMaterial::TransferColor()
{
	//�l���������ނƎ����I�ɓ]�������
	constMapMaterial->color = XMFLOAT4(1, 1, 1, 1);
}

void NMaterial::SetColor(XMFLOAT4 color)
{
	constMapMaterial->color = color;
}

void NMaterial::SetCBV(ComPtr<ID3D12GraphicsCommandList> commandList)
{
	commandList->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());
}
