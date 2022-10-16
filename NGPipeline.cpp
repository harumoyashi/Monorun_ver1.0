#include "NGPipeline.h"

void NGPipeline::Initialize3d(ComPtr<ID3D12Device> device)
{
	//���_
	SetVert();
	SetVertHeap();
	SetVertResource();
	SetIndex();	//�����ŃC���f�b�N�X�̐ݒ肾���s��
	SetNormal();
	CreateVertBuff(device);
	VertMaping();
	CreateVertBuffView();

	//�C���f�b�N�X�f�[�^
	SetIndexResource();
	CreateIndexBuff(device);
	IndexMaping();
	CreateIndexBuffView();
}

void NGPipeline::SetVert()
{
	// ���_�f�[�^�ݒ�
	Vertex vert[] = {
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

	//�ݒ肵���瑼�ł��g����ϐ��ɑ��
	std::copy(std::begin(vert), std::end(vert), vertices);

	//���_�o�b�t�@�̃T�C�Y����
	singleSizeVB = static_cast<UINT>(sizeof(vertices[0]));
	sizeVB = static_cast<UINT>(sizeof(vertices));
}

void NGPipeline::SetVertHeap()
{
	//�q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p
	heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
}

void NGPipeline::SetVertResource()
{
	//���\�[�X�ݒ�
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB;			//���_�f�[�^�S�̂̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.Format = DXGI_FORMAT_UNKNOWN;
	resDesc.SampleDesc.Count = 1;
	resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
}

void NGPipeline::CreateVertBuff(ComPtr<ID3D12Device> device)
{
	HRESULT result;

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

void NGPipeline::VertMaping()
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

void NGPipeline::CreateVertBuffView()
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

void NGPipeline::SetIndex()
{
	//�C���f�b�N�X�f�[�^
	unsigned short index[] =
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

	//�ݒ肵���瑼�ł��g����ϐ��ɑ��
	std::copy(std::begin(index), std::end(index), indices);

	//���_�o�b�t�@�̃T�C�Y����
	sizeIB = static_cast<UINT>(sizeof(uint16_t) * _countof(indices));
}

void NGPipeline::SetIndexResource()
{
	//���\�[�X�ݒ�
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB;			//���_�f�[�^�S�̂̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.Format = DXGI_FORMAT_UNKNOWN;
	resDesc.SampleDesc.Count = 1;
	resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
}

void NGPipeline::SetNormal()
{
	for (int i = 0; i < _countof(indices) / 3; i++)
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

void NGPipeline::CreateIndexBuff(ComPtr<ID3D12Device> device)
{
	HRESULT result;

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

void NGPipeline::IndexMaping()
{
	HRESULT result;

	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	assert(SUCCEEDED(result));
	// �S���_�ɑ΂���
	for (int i = 0; i < _countof(indices); i++)
	{
		indexMap[i] = indices[i];	//�C���f�b�N�X���R�s�[
	}
	// �q���������
	indexBuff->Unmap(0, nullptr);
}

void NGPipeline::CreateIndexBuffView()
{
	// GPU���z�A�h���X
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	//�C���f�b�N�X1���̃T�C�Y
	ibView.Format = DXGI_FORMAT_R16_UINT;
	// �C���f�b�N�X�o�b�t�@�̃T�C�Y
	ibView.SizeInBytes = sizeIB;
}

void NGPipeline::LoadVertShader3d()
{
	HRESULT result;

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/BasicVS.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "vs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&vsBlob, &errorBlob);

	// �G���[�Ȃ�
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}
}

void NGPipeline::LoadPixelShader3d()
{
	HRESULT result;

	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/BasicPS.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "ps_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&psBlob, &errorBlob);

	// �G���[�Ȃ�
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}
}

void NGPipeline::LoadVertShaderSprite()
{
	HRESULT result;

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/SpriteVS.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "vs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&vsBlob, &errorBlob);

	// �G���[�Ȃ�
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}
}

void NGPipeline::LoadPixelShaderSprite()
{
	HRESULT result;

	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/SpritePS.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "ps_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&psBlob, &errorBlob);

	// �G���[�Ȃ�
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}
}

void NGPipeline::SetVertLayout3d()
{
	// ���_���C�A�E�g
	//���W
	vertLayout3d[0] = {
	"POSITION",										//�Z�}���e�B�b�N��
	0,												//�����̃Z�}���e�B�b�N������Ƃ��g���C���f�b�N�X
	DXGI_FORMAT_R32G32B32_FLOAT,					//�v�f���ƃr�b�g����\��
	0,												//���̓X���b�g�C���f�b�N�X
	D3D12_APPEND_ALIGNED_ELEMENT,					//�f�[�^�̃I�t�Z�b�g�n(���͎̂����ݒ�)
	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,		//���̓f�[�^���
	0												//��x�ɕ`�悷��C���X�^���X��(0�ł悢)
	};// (1�s�ŏ������ق������₷��)
	//���W�ȊO�ɐF�A�e�N�X�`��UV�Ȃǂ�n���ꍇ�͂���ɑ�����
	//�@���x�N�g��
	vertLayout3d[1] = {
		"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
	};
	//UV
	vertLayout3d[2] = {
		"TEXCOORD",0,
		DXGI_FORMAT_R32G32_FLOAT,0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
	};
}

void NGPipeline::SetVertLayoutSprite()
{
	// ���_���C�A�E�g
	//���W
	vertLayoutSprite[0] = {
	"POSITION",										//�Z�}���e�B�b�N��
	0,												//�����̃Z�}���e�B�b�N������Ƃ��g���C���f�b�N�X
	DXGI_FORMAT_R32G32B32_FLOAT,					//�v�f���ƃr�b�g����\��
	0,												//���̓X���b�g�C���f�b�N�X
	D3D12_APPEND_ALIGNED_ELEMENT,					//�f�[�^�̃I�t�Z�b�g�n(���͎̂����ݒ�)
	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,		//���̓f�[�^���
	0												//��x�ɕ`�悷��C���X�^���X��(0�ł悢)
	};// (1�s�ŏ������ق������₷��)
	//���W�ȊO�ɐF�A�e�N�X�`��UV�Ȃǂ�n���ꍇ�͂���ɑ�����
	//UV
	vertLayoutSprite[1] = {
		"TEXCOORD",0,
		DXGI_FORMAT_R32G32_FLOAT,0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
	};
}

void NGPipeline::SetShader()
{
	// �V�F�[�_�[�̐ݒ�
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();
}

void NGPipeline::SetRasterizer(bool isCull)
{
	// �T���v���}�X�N�̐ݒ�
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�

	// ���X�^���C�U�̐ݒ�
	if (isCull)
	{
		pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;	// �w�ʂ��J�����O
	}
	else
	{
		pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;	// �J�����O���Ȃ�
	}
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;	// �|���S�����h��Ԃ�(D3D12_FILL_MODE_WIREFRAME�ɂ���ƃ��C���[�t���[����)
	pipelineDesc.RasterizerState.DepthClipEnable = true; // �[�x�N���b�s���O��L����
}

void NGPipeline::SetBlend()
{
	pipelineDesc.BlendState.AlphaToCoverageEnable = false;			//�ԗ����l�����ău�����h���邩
	pipelineDesc.BlendState.IndependentBlendEnable = false;			//���ꂼ��̃����_�[�^�[�Q�b�g�ɕʁX�̃u�����h���邩

	//�����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	blenddesc.BlendEnable = false;									//���Z�A��Z�A���Ȃǂ��邩
	blenddesc.LogicOpEnable = false;								//�_�����Z���邩
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	//�}�X�N�l�FRBGA�S�Ẵ`�����l����`��
	//�ݒ肵���u�����h��K�p
	pipelineDesc.BlendState.RenderTarget[0] = blenddesc;

	blenddesc.BlendEnable = true;					//�u�����h��L���ɂ���
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//���Z
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;		//�\�[�X�̒l��100%�g��
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//�f�X�g�̒l��0%�g��

	//���Z����
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;				//���Z
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;			//�\�[�X�̃A���t�@�l
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;	//1.0f-�\�[�X�̃A���t�@�l
}

void NGPipeline::SetInputLayout(bool is3d)
{
	if (is3d)
	{
		pipelineDesc.InputLayout.pInputElementDescs = vertLayout3d;
		pipelineDesc.InputLayout.NumElements = _countof(vertLayout3d);
	}
	else
	{
		pipelineDesc.InputLayout.pInputElementDescs = vertLayoutSprite;
		pipelineDesc.InputLayout.NumElements = _countof(vertLayoutSprite);
	}
}

void NGPipeline::SetTopology()
{
	//�g���C�A���O���X�g���b�v��؂藣�����ǂ���
	pipelineDesc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;	//�J�b�g�Ȃ�
	//�g�|���W�[�w��
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
}

void NGPipeline::SetDepth(bool isDepth)
{
	if (isDepth)
	{
		pipelineDesc.DepthStencilState.DepthEnable = true;						//�[�x�e�X�g���邩
	}
	else
	{
		pipelineDesc.DepthStencilState.DepthEnable = false;						//�[�x�e�X�g���邩
	}
	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;	//�������݋���
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;		//��������΍��i
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;								//�[�x�l�t�H�[�}�b�g
}

void NGPipeline::SetRenderTarget()
{
	pipelineDesc.NumRenderTargets = 1;								//�`��Ώۂ�1��
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	//0~255�w���RGBA
}

void NGPipeline::SetAntiAliasing()
{
	pipelineDesc.SampleDesc.Count = 1;		//1�s�N�Z���ɂ�1��T���v�����O
	pipelineDesc.SampleDesc.Quality = 0;	//�Œ�N�I���e�B
}

void NGPipeline::SetRootSignature(ComPtr<ID3D12Device> device, std::vector<D3D12_ROOT_PARAMETER> rootParams)
{
	pipelineSet.rootSig.SetRootSignature(errorBlob, rootParams, samplerDesc);
	pipelineSet.rootSig.CreateRootSignature(device);

	pipelineDesc.pRootSignature = pipelineSet.rootSig.entity.Get();
}

void NGPipeline::CreatePS(ComPtr<ID3D12Device> device)
{
	HRESULT result;

	result = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineSet.pipelineState));
	assert(SUCCEEDED(result));
}

void NGPipeline::SetTexSampler()
{
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//���J��Ԃ��i�^�C�����O�j
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//�c�J��Ԃ��i�^�C�����O�j
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//���s�J��Ԃ��i�^�C�����O�j
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	//�{�[�_�[�̎��͍�
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					//�S�ă��j�A���
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;									//�~�b�v�}�b�v�ő�l
	samplerDesc.MinLOD = 0.0f;												//�~�b�v�}�b�v�ŏ��l
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			//�s�N�Z���V�F�[�_����̂ݎg�p�\
}

NGPipeline::PipelineSet NGPipeline::CreatePipeline3d(ComPtr<ID3D12Device> device, std::vector<D3D12_ROOT_PARAMETER> rootParams)
{
	//�V�F�[�_�[
	LoadVertShader3d();
	LoadPixelShader3d();

	//���_���C�A�E�g�ݒ�
	SetVertLayout3d();

	//�p�C�v���C���X�e�[�g
	SetShader();
	SetRasterizer(true);
	SetBlend();
	SetInputLayout(true);
	SetTopology();
	SetRenderTarget();
	SetAntiAliasing();
	SetDepth(true);

	//�e�N�X�`���T���v���[�̐ݒ�
	SetTexSampler();

	//���[�g�V�O�l�`��
	SetRootSignature(device, rootParams);

	//�p�C�v���C���X�e�[�g����
	CreatePS(device);

	return pipelineSet;
}

NGPipeline::PipelineSet NGPipeline::CreatePipelineSprite(ComPtr<ID3D12Device> device, std::vector<D3D12_ROOT_PARAMETER> rootParams)
{
	//�V�F�[�_�[
	LoadVertShaderSprite();
	LoadPixelShaderSprite();

	//���_���C�A�E�g�ݒ�
	SetVertLayoutSprite();

	//�p�C�v���C���X�e�[�g
	SetShader();
	SetRasterizer(false);
	SetBlend();
	SetInputLayout(false);
	SetTopology();
	SetRenderTarget();
	SetAntiAliasing();
	SetDepth(false);

	//�e�N�X�`���T���v���[�̐ݒ�
	SetTexSampler();

	//���[�g�V�O�l�`��
	SetRootSignature(device, rootParams);

	//�p�C�v���C���X�e�[�g����
	CreatePS(device);

	return pipelineSet;
}
