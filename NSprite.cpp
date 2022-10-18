#include "NSprite.h"

void NSprite::CreateSprite(ComPtr<ID3D12Device> device)
{
	//���_
	SetVert();
	SetVertHeap();
	SetVertResource();
	CreateVertBuff(device);
	VertMaping();
	CreateVertBuffView();
	//�萔�o�b�t�@
	SetConstHeap();
	SetConstResource();
	CreateCB(device);
	MappingCB();
	SetColor();
	ParallelProjection();
	Unmap();
	//���s���e����
	matProjection = constMapTransform->mat = XMMatrixOrthographicOffCenterLH(
		0.0f, static_cast<float>(NWindows::win_width),
		static_cast<float>(NWindows::win_height), 0.0f,
		0.0f, 1.0f
	);
}

void NSprite::CreateSprite(ComPtr<ID3D12Device> device, ComPtr<ID3D12Resource> texBuff,
	XMFLOAT2 anchorPoint, bool isFlipX, bool isFlipY)
{
	//���_
	SetVert();
	SetVertHeap();
	SetVertResource();
	CreateVertBuff(device);
	MatchTexSize(texBuff);	//�����Ńe�N�X�`���T�C�Y�ɍ��킹�Ă�
	SetAncor(anchorPoint);
	SetIsFlip(isFlipX, isFlipY);
	SetClipRange();
	TransferVertex();
	VertMaping();
	CreateVertBuffView();
	//�萔�o�b�t�@
	SetConstHeap();
	SetConstResource();
	CreateCB(device);
	MappingCB();
	SetColor();
	ParallelProjection();
	Unmap();
	//���s���e����
	matProjection = constMapTransform->mat = XMMatrixOrthographicOffCenterLH(
		0.0f, static_cast<float>(NWindows::win_width),
		static_cast<float>(NWindows::win_height), 0.0f,
		0.0f, 1.0f
	);
}

void NSprite::CreateClipSprite(ComPtr<ID3D12Device> device, ComPtr<ID3D12Resource> texBuff,
	XMFLOAT2 texLeftTop, XMFLOAT2 texSize, XMFLOAT2 anchorPoint, bool isFlipX, bool isFlipY)
{
	//���_
	SetVert();
	SetVertHeap();
	SetVertResource();
	CreateVertBuff(device);
	MatchTexSize(texBuff);	//�����Ńe�N�X�`���T�C�Y�ɍ��킹�Ă�
	SetAncor(anchorPoint);
	SetIsFlip(isFlipX, isFlipY);
	SetClipRange(texLeftTop, texSize);
	TransferVertex();
	VertMaping();
	CreateVertBuffView();
	//�萔�o�b�t�@
	SetConstHeap();
	SetConstResource();
	CreateCB(device);
	MappingCB();
	SetColor();
	ParallelProjection();
	Unmap();
	//���s���e����
	matProjection = constMapTransform->mat = XMMatrixOrthographicOffCenterLH(
		0.0f, static_cast<float>(NWindows::win_width),
		static_cast<float>(NWindows::win_height), 0.0f,
		0.0f, 1.0f
	);
}

void NSprite::SetVert()
{
	VertexUV vert[] = {
		//	x		y		z	 	u	v
		{{   0.0f, 100.0f, 0.0f }, {0.0f,1.0f}},	// ����
		{{   0.0f,   0.0f, 0.0f }, {0.0f,0.0f}},	// ����
		{{ 100.0f, 100.0f, 0.0f }, {1.0f,1.0f}},	// �E��
		{{ 100.0f,   0.0f, 0.0f }, {1.0f,0.0f}},	// �E��
	};

	//�ݒ肵���瑼�ł��g����ϐ��ɑ��
	std::copy(std::begin(vert), std::end(vert), vertices);

	//���_�o�b�t�@�̃T�C�Y����
	singleSizeVB = static_cast<UINT>(sizeof(vertices[0]));
	sizeVB = static_cast<UINT>(sizeof(vertices));
}

void NSprite::SetVertHeap()
{
	//�q�[�v�ݒ�
	heapPropVert.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p
	heapPropVert.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapPropVert.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
}

void NSprite::SetVertResource()
{
	//���\�[�X�ݒ�
	resDescVert.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDescVert.Width = sizeVB;			//���_�f�[�^�S�̂̃T�C�Y
	resDescVert.Height = 1;
	resDescVert.DepthOrArraySize = 1;
	resDescVert.MipLevels = 1;
	resDescVert.Format = DXGI_FORMAT_UNKNOWN;
	resDescVert.SampleDesc.Count = 1;
	resDescVert.Flags = D3D12_RESOURCE_FLAG_NONE;
	resDescVert.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
}

void NSprite::CreateVertBuff(ComPtr<ID3D12Device> device)
{
	HRESULT result;

	// ���_�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&heapPropVert, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDescVert, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));
}

void NSprite::MatchTexSize(ComPtr<ID3D12Resource> texBuff)
{
	resDescVert = texBuff->GetDesc();
	size = { (float)resDescVert.Width,(float)resDescVert.Height };
}

void NSprite::SetAncor(XMFLOAT2 anchorPoint)
{
	this->anchorPoint = anchorPoint;
}

void NSprite::SetIsFlip(bool isFlipX, bool isFlipY)
{
	this->isFlipX = isFlipX;
	this->isFlipY = isFlipY;
}

void NSprite::SetClipRange(XMFLOAT2 texLeftTop, XMFLOAT2 texSize)
{
	this->texLeftTop = texLeftTop;
	this->texSize = texSize;
}

void NSprite::SetClipRange()
{
	texLeftTop = { 0,0 };
	texSize = size;
}

void NSprite::VertMaping()
{
	HRESULT result;

	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// �S���_�ɑ΂���
	std::copy(std::begin(vertices), std::end(vertices), vertMap);	//���W���R�s�[
	// �q���������
	vertBuff->Unmap(0, nullptr);
}

void NSprite::CreateVertBuffView()
{
	// ���_�o�b�t�@�r���[�̍쐬
	// GPU���z�A�h���X
	//����n�����ƂŁAGPU�����ǂ̃f�[�^����΂������킩��悤�ɂȂ�
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	// ���_�o�b�t�@�̃T�C�Y
	vbView.SizeInBytes = sizeVB;
	// ���_1���̃f�[�^�T�C�Y
	vbView.StrideInBytes = singleSizeVB;
}

void NSprite::SetConstHeap()
{
	heapPropConst.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPU�ւ̓]���p
}

void NSprite::SetConstResource()
{
	resDescConst.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDescConst.Width = (sizeof(constMapTransform) + 0xff) & ~0xff;	//256�o�C�g�A���C�������g
	resDescConst.Height = 1;
	resDescConst.DepthOrArraySize = 1;
	resDescConst.MipLevels = 1;
	resDescConst.SampleDesc.Count = 1;
	resDescConst.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
}

void NSprite::CreateCB(ComPtr<ID3D12Device> device)
{
	HRESULT result;

	result = device->CreateCommittedResource(
		&heapPropConst,	//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDescConst,	//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform)
	);
	assert(SUCCEEDED(result));
}

void NSprite::MappingCB()
{
	HRESULT result;

	result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform);	//�}�b�s���O

	assert(SUCCEEDED(result));
}

void NSprite::SetColor(float R, float G, float B, float A)
{
	constMapTransform->color = XMFLOAT4(R, G, B, A);

}

void NSprite::ParallelProjection()
{
	constMapTransform->mat = XMMatrixOrthographicOffCenterLH(
		0.0f, static_cast<float>(NWindows::win_width),
		static_cast<float>(NWindows::win_height), 0.0f,
		0.0f, 1.0f
	);
}

void NSprite::Unmap()
{
	constBuffTransform->Unmap(0, nullptr);
}

void NSprite::UpdateMatrix()
{
	//���[���h�s��
	XMMATRIX matRot = XMMatrixIdentity();	//��]�s��
	matRot = XMMatrixRotationZ(XMConvertToRadians(rotation));	//Z�������0�x��]���Ă���

	XMMATRIX matTrans;	//���s�ړ��s��
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);

	matWorld = XMMatrixIdentity();	//�P�ʍs����
	matWorld *= matRot;		//���[���h���W�ɉ�]�𔽉f
	matWorld *= matTrans;	//���[���h���W�ɕ��s�ړ��𔽉f
}

void NSprite::TransferMatrix()
{
	MappingCB();
	constMapTransform->mat = matWorld * matProjection;
	Unmap();
}

void NSprite::TransferVertex()
{
	//UV�����f�t�H���g�Őݒ�
	VertexUV vert[] = {
		//		u	v
		{{}, {0.0f,1.0f}},	// ����
		{{}, {0.0f,0.0f}},	// ����
		{{}, {1.0f,1.0f}},	// �E��
		{{}, {1.0f,0.0f}},	// �E��
	};

	//�A���J�[�|�C���g�ݒ�
	float left = (0.0f - anchorPoint.x) * size.x;
	float right = (1.0f - anchorPoint.x) * size.x;
	float top = (0.0f - anchorPoint.y) * size.y;
	float bottom = (1.0f - anchorPoint.y) * size.y;

	//���E���]
	if (isFlipX)
	{
		left = -left;
		right = -right;
	}
	//�㉺���]
	if (isFlipY)
	{
		top = -top;
		bottom = -bottom;
	}

	//�ݒ肵���A���J�[�|�C���g�ɍ��킹�Ē��_�ݒ肵�Ȃ���
	vert[0].pos = { left ,bottom,0.0f };	// ����
	vert[1].pos = { left ,top   ,0.0f };	// ����
	vert[2].pos = { right,bottom,0.0f };	// �E��
	vert[3].pos = { right,top   ,0.0f };	// �E��

	//�e�N�X�`���T�C�Y�����Ƃɐ؂��镔����uv���v�Z
	float tex_left = texLeftTop.x / resDescVert.Width;
	float tex_right = (texLeftTop.x + texSize.x) / resDescVert.Width;
	float tex_top = texLeftTop.y / resDescVert.Height;
	float tex_bottom = (texLeftTop.y + texSize.y) / resDescVert.Height;

	//�v�Z����uv�ɍ��킹�Đݒ肵�Ȃ���
	vert[0].uv = { tex_left ,tex_bottom };	// ����
	vert[1].uv = { tex_left ,tex_top };	// ����
	vert[2].uv = { tex_right,tex_bottom };	// �E��
	vert[3].uv = { tex_right,tex_top };	// �E��

	//�ݒ肵���瑼�ł��g����ϐ��ɑ��
	std::copy(std::begin(vert), std::end(vert), vertices);

	//���_�o�b�t�@�̃T�C�Y����
	singleSizeVB = static_cast<UINT>(sizeof(vertices[0]));
	sizeVB = static_cast<UINT>(sizeof(vertices));

	//���_�o�b�t�@�ւ̃f�[�^�]��
	VertMaping();
}

void NSprite::CommonBeginDraw(ComPtr<ID3D12GraphicsCommandList> commandList,
	const ComPtr<ID3D12PipelineState> pipelineState, ComPtr<ID3D12RootSignature> rootSignature, ComPtr<ID3D12DescriptorHeap> srvHeap)
{
	// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
	commandList->SetPipelineState(pipelineState.Get());
	commandList->SetGraphicsRootSignature(rootSignature.Get());

	// �v���~�e�B�u�`��̐ݒ�R�}���h
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); // �O�p�`�X�g���b�v

	ID3D12DescriptorHeap* ppHeaps[] = { srvHeap.Get() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
}

void NSprite::Draw(ComPtr<ID3D12DescriptorHeap> srvHeap, UINT incrementSize, ComPtr<ID3D12GraphicsCommandList> commandList)
{
	//��\���t���O�����Ă���`�悹���ɏ���������
	if (isInvisible)
	{
		return;
	}

	//�V�F�[�_���\�[�X�r���[(SRV)�q�[�v�̐擪�n���h�����擾(SRV���w���Ă�͂�)
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();
	//�n���h�����w��̂Ƃ��܂Ői�߂�
	srvGpuHandle.ptr += incrementSize * texNum;

	//�w��̃q�[�v�ɂ���SRV�����[�g�p�����[�^1�Ԃɐݒ�
	commandList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	commandList->IASetVertexBuffers(0, 1, &vbView);

	//���[�g�p�����[�^0�Ԃɒ萔�o�b�t�@��n��
	commandList->SetGraphicsRootConstantBufferView(0, constBuffTransform->GetGPUVirtualAddress());
	commandList->DrawInstanced(4, 1, 0, 0);
}