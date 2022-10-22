#include "NCube.h"

//�ÓI�����o�ϐ��̎���
std::vector<Vertex> NCube::vertices;
std::vector<unsigned short>NCube::indices;


void NCube::Initialize(ComPtr<ID3D12Device> device)
{
	//���_
	SetVert();
	SetIndex();	//�����ŃC���f�b�N�X�̐ݒ肾���s��
	SetNormal();
	CreateVertBuff(device);
	VertMaping();
	CreateVertBuffView();

	//�C���f�b�N�X�f�[�^
	CreateIndexBuff(device);
	IndexMaping();
	CreateIndexBuffView();
}

void NCube::SetVert()
{
	// ���_�f�[�^�ݒ�
	vertices = {
		//	x		y		z	 �@��	u	v
		//�O
		{{ -5.0f, -5.0f, -5.0f }, {}, {0.0f,1.0f}},	// ����
		{{ -5.0f,  5.0f, -5.0f }, {}, {0.0f,0.0f}},	// ����
		{{  5.0f, -5.0f, -5.0f }, {}, {1.0f,1.0f}},	// �E��
		{{  5.0f,  5.0f, -5.0f }, {}, {1.0f,0.0f}},	// �E��

		//��
		{{ -5.0f, -5.0f, 5.0f }, {}, {0.0f,1.0f}},	// ����
		{{ -5.0f,  5.0f, 5.0f }, {}, {0.0f,0.0f}},	// ����
		{{  5.0f, -5.0f, 5.0f }, {}, {1.0f,1.0f}},	// �E��
		{{  5.0f,  5.0f, 5.0f }, {}, {1.0f,0.0f}},	// �E��

		 // ��
		{{-5.0f,-5.0f,-5.0f }, {}, {0.0f, 1.0f}},    // ����
		{{-5.0f,-5.0f, 5.0f }, {}, {0.0f, 0.0f}},    // ����
		{{-5.0f, 5.0f,-5.0f }, {}, {1.0f, 1.0f}},    // �E��
		{{-5.0f, 5.0f, 5.0f }, {}, {1.0f, 0.0f}},    // �E��

		// �E
		{{ 5.0f,-5.0f,-5.0f }, {}, {0.0f, 1.0f}},    // ����
		{{ 5.0f,-5.0f, 5.0f }, {}, {0.0f, 0.0f}},    // ����
		{{ 5.0f, 5.0f,-5.0f }, {}, {1.0f, 1.0f}},    // �E��
		{{ 5.0f, 5.0f, 5.0f }, {}, {1.0f, 0.0f}},    // �E��

		// ��
		{{-5.0f,-5.0f,-5.0f }, {}, {0.0f, 1.0f}},    // ����
		{{ 5.0f,-5.0f,-5.0f }, {}, {0.0f, 0.0f}},    // ����
		{{-5.0f,-5.0f, 5.0f }, {}, {1.0f, 1.0f}},    // �E��
		{{ 5.0f,-5.0f, 5.0f }, {}, {1.0f, 0.0f}},    // �E��

		// ��
		{{-5.0f, 5.0f,-5.0f }, {}, {0.0f, 1.0f}},    // ����
		{{ 5.0f, 5.0f,-5.0f }, {}, {0.0f, 0.0f}},    // ����
		{{-5.0f, 5.0f, 5.0f }, {}, {1.0f, 1.0f}},    // �E��
		{{ 5.0f, 5.0f, 5.0f }, {}, {1.0f, 0.0f}},    // �E��
	};

	////�ݒ肵���瑼�ł��g����ϐ��ɑ��
	//std::copy(std::begin(vert), std::end(vert), vertices);

	//���_�o�b�t�@�̃T�C�Y����
	singleSizeVB = static_cast<UINT>(sizeof(vertices[0]));
	sizeVB = static_cast<UINT>(sizeof(Vertex) * vertices.size());
}

void NCube::CreateVertBuff(ComPtr<ID3D12Device> device)
{
	HRESULT result;

	// �q�[�v�v���p�e�B
	CD3DX12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// ���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC resDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeVB);

	// ���_�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&heapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));
}

void NCube::VertMaping()
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

void NCube::CreateVertBuffView()
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

void NCube::SetIndex()
{
	//�C���f�b�N�X�f�[�^
	indices =
	{
		//�O
		0,1,2,	//�O�p�`1��
		2,1,3,	//�O�p�`2��
		//��
		5,4,6,	//�O�p�`3��
		5,6,7,	//�O�p�`4��
		//��
		8,9,10,	//�O�p�`5��
		10,9,11,//�O�p�`6��
		//�E
		13,12,14,	//�O�p�`7��
		13,14,15,	//�O�p�`8��
		//��
		16,17,18,	//�O�p�`9��
		18,17,19,	//�O�p�`10��
		//��
		21,20,22,	//�O�p�`11��
		21,22,23,	//�O�p�`12��
	};

	////�ݒ肵���瑼�ł��g����ϐ��ɑ��
	//std::copy(std::begin(index), std::end(index), indices);

	//���_�o�b�t�@�̃T�C�Y����
	sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices.size());
	numIB = static_cast<UINT>(sizeof(unsigned short) * indices.size() / sizeof(indices[0]));
}

void NCube::SetNormal()
{
	for (int i = 0; i < numIB / 3; i++)
	{//�O�p�`1���ƂɌv�Z���Ă���
		//�O�p�`�̃C���f�b�N�X�����o���āA�ꎞ�I�ȕϐ��ɂ����
		unsigned short index0 = indices[i * 3 + 0];
		unsigned short index1 = indices[i * 3 + 1];
		unsigned short index2 = indices[i * 3 + 2];

		//�O�p�`���\�����钸�_���W���x�N�g���ɑ��
		XMVECTOR p0 = XMLoadFloat3(&vertices[index0].pos);
		XMVECTOR p1 = XMLoadFloat3(&vertices[index1].pos);
		XMVECTOR p2 = XMLoadFloat3(&vertices[index2].pos);

		//p0��p1�x�N�g���Ap0��p2�x�N�g�����v�Z(�x�N�g���̌��Z)
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);

		//�O�ς͗������琂���ȃx�N�g��
		XMVECTOR normal = XMVector3Cross(v1, v2);

		//���K��
		normal = XMVector3Normalize(normal);

		//���߂��@���𒸓_�f�[�^�ɑ��
		XMStoreFloat3(&vertices[index0].normal, normal);
		XMStoreFloat3(&vertices[index1].normal, normal);
		XMStoreFloat3(&vertices[index2].normal, normal);
	}
}

void NCube::CreateIndexBuff(ComPtr<ID3D12Device> device)
{
	HRESULT result;

	// �q�[�v�v���p�e�B
	CD3DX12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// ���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC resDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeIB);

	// ���_�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&heapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));
	assert(SUCCEEDED(result));
}

void NCube::IndexMaping()
{
	HRESULT result;

	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	assert(SUCCEEDED(result));
	// �S���_�ɑ΂���
	for (int i = 0; i < numIB; i++)
	{
		indexMap[i] = indices[i];	//�C���f�b�N�X���R�s�[
	}
	// �q���������
	indexBuff->Unmap(0, nullptr);
}

void NCube::CreateIndexBuffView()
{
	// GPU���z�A�h���X
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	//�C���f�b�N�X1���̃T�C�Y
	ibView.Format = DXGI_FORMAT_R16_UINT;
	// �C���f�b�N�X�o�b�t�@�̃T�C�Y
	ibView.SizeInBytes = sizeIB;
}