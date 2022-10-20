#include "NObj3d.h"
#include "NInput.h"

void NObj3d::Initialize(ComPtr<ID3D12Device> device)
{
	//�萔�o�b�t�@
	SetCBHeap();
	SetCBResource();
	CreateCB(device);
	MappingCB();
}

void NObj3d::SetCBHeap()
{
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPU�ւ̓]���p
}

void NObj3d::SetCBResource()
{
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = (sizeof(constMapTransform) + 0xff) & ~0xff;	//256�o�C�g�A���C�������g
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
}

void NObj3d::CreateCB(ComPtr<ID3D12Device> device)
{
	HRESULT result;

	result = device->CreateCommittedResource(
		&heapProp,	//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc,	//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform)
	);
	assert(SUCCEEDED(result));
}

void NObj3d::MappingCB()
{
	HRESULT result;

	result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform);	//�}�b�s���O
	assert(SUCCEEDED(result));
}

void NObj3d::MoveKey()
{
	//�����ꂩ�̃L�[���������Ƃ�
	if (NInput::IsKeyDown(DIK_W) || NInput::IsKeyDown(DIK_S) || NInput::IsKeyDown(DIK_D) || NInput::IsKeyDown(DIK_A))
	{
		if (NInput::IsKeyDown(DIK_W)) { position.y += 5.0f; }
		else if (NInput::IsKeyDown(DIK_S)) { position.y -= 5.0f; }
		if (NInput::IsKeyDown(DIK_D)) { position.x += 5.0f; }
		else if (NInput::IsKeyDown(DIK_A)) { position.x -= 5.0f; }
	}
}

void NObj3d::UpdateMatrix(XMMATRIX matView, XMMATRIX matProjection)
{
	//���[���h�s��
	XMMATRIX matScale;	//�X�P�[�����O�s��
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);

	XMMATRIX matRot = XMMatrixIdentity();		//��]�s��
	matRot *= XMMatrixRotationZ(rotation.z);	//Z�������0�x��]���Ă���
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));	//X�������15�x��]���Ă���
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));	//Y�������30�x��]

	XMMATRIX matTrans;	//���s�ړ��s��
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);

	matWorld = XMMatrixIdentity();	//�P�ʍs����
	matWorld *= matScale;	//���[���h���W�ɃX�P�[�����O�𔽉f
	matWorld *= matRot;	//���[���h���W�ɉ�]�𔽉f
	matWorld *= matTrans;	//���[���h���W�ɕ��s�ړ��𔽉f

	//�e�I�u�W�F�N�g�������
	if (parent != nullptr)
	{
		//�e�I�u�W�F�N�g�̃��[���h�s���������
		matWorld *= parent->matWorld;
	}

	TransferMatrix(matView, matProjection);
}

void NObj3d::TransferMatrix(XMMATRIX matView, XMMATRIX matProjection)
{
	constMapTransform->mat = matWorld * matView * matProjection;
}

void NObj3d::CommonBeginDraw(ComPtr<ID3D12GraphicsCommandList> commandList, const ComPtr<ID3D12PipelineState> pipelineState, ComPtr<ID3D12RootSignature> rootSignature, ComPtr<ID3D12DescriptorHeap> srvHeap)
{
	// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
	commandList->SetPipelineState(pipelineState.Get());
	commandList->SetGraphicsRootSignature(rootSignature.Get());

	// �v���~�e�B�u�`��̐ݒ�R�}���h
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // �O�p�`���X�g

	ID3D12DescriptorHeap* ppHeaps[] = { srvHeap.Get() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
}

void NObj3d::Draw(ComPtr<ID3D12GraphicsCommandList> commandList, NMaterial material, ComPtr<ID3D12DescriptorHeap> srvHeap,
	D3D12_VERTEX_BUFFER_VIEW vbView, D3D12_INDEX_BUFFER_VIEW ibView, UINT indexSize, UINT incrementSize)
{
	SetMaterialCBV(commandList, material);
	SetSRVHeap(srvHeap, commandList, incrementSize);
	SetVB(commandList, vbView);
	SetIB(commandList, ibView);
	SetMatCBV(commandList);
	DrawCommand(commandList, indexSize);
}

void NObj3d::SetMaterialCBV(ComPtr<ID3D12GraphicsCommandList> commandList, NMaterial material)
{
	material.SetCBV(commandList);
}

void NObj3d::SetSRVHeap(ComPtr<ID3D12DescriptorHeap> srvHeap, ComPtr<ID3D12GraphicsCommandList> commandList, UINT incrementSize)
{
	//�V�F�[�_���\�[�X�r���[(SRV)�q�[�v�̐擪�n���h�����擾(SRV���w���Ă�͂�)
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();
	//�n���h�����w��̂Ƃ��܂Ői�߂�
	srvGpuHandle.ptr += incrementSize * texNum;
	//�w��̃q�[�v�ɂ���SRV�����[�g�p�����[�^1�Ԃɐݒ�
	commandList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);
}

void NObj3d::SetVB(ComPtr<ID3D12GraphicsCommandList> commandList, D3D12_VERTEX_BUFFER_VIEW vbView)
{
	commandList->IASetVertexBuffers(0, 1, &vbView);
}

void NObj3d::SetIB(ComPtr<ID3D12GraphicsCommandList> commandList, D3D12_INDEX_BUFFER_VIEW ibView)
{
	commandList->IASetIndexBuffer(&ibView);
}

void NObj3d::SetMatCBV(ComPtr<ID3D12GraphicsCommandList> commandList)
{
	//���[�g�p�����[�^2�Ԃ�3D�ϊ��s��̒萔�o�b�t�@��n��
	commandList->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());
}

void NObj3d::DrawCommand(ComPtr<ID3D12GraphicsCommandList> commandList, UINT indexSize)
{
	commandList->DrawIndexedInstanced(indexSize, 1, 0, 0, 0); //�C���f�b�N�X���g���ĕ`��
}
