#include "NGPipeline.h"

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

void NGPipeline::SetBlend(bool AlphaToCoverageEnable)
{
	if (AlphaToCoverageEnable)
	{
		//�X�v���C�g�͂������̕�����������
		pipelineDesc.BlendState.AlphaToCoverageEnable = true;		//�ԗ����l�����ău�����h���邩
	}
	else
	{
		//�I�u�W�F�N�g�͂������ɂ��Ȃ��Ə����邩��
		pipelineDesc.BlendState.AlphaToCoverageEnable = false;		//�ԗ����l�����ău�����h���邩
	}
	pipelineDesc.BlendState.IndependentBlendEnable = false;			//���ꂼ��̃����_�[�^�[�Q�b�g�ɕʁX�̃u�����h���邩

	//�����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	blenddesc.BlendEnable = true;					//�u�����h��L���ɂ���
	blenddesc.LogicOpEnable = false;				//�_�����Z���邩
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	//�}�X�N�l�FRBGA�S�Ẵ`�����l����`��

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//���Z
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;		//�\�[�X�̒l��100%�g��
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//�f�X�g�̒l��0%�g��

	//�A���t�@�u�����h
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;				//���Z
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;			//�\�[�X�̃A���t�@�l
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;	//1.0f-�\�[�X�̃A���t�@�l

	//�ݒ肵���u�����h��K�p
	pipelineDesc.BlendState.RenderTarget[0] = blenddesc;
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
	SetBlend(false);
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
	SetBlend(false);
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
