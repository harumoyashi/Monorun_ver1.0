#include "NDX12.h"

void NDX12::Init(NWindows win)
{
	HRESULT result;

#ifdef _DEBUG
	//�f�o�b�O���C���[���I����
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
		debugController->EnableDebugLayer();
	}
#endif
	// DXGI�t�@�N�g���[�̐���
	result = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
	assert(SUCCEEDED(result));

	ChoiceAdapters();
	CreateDevice();
	CreateCommandGroup();
	CreateSwapChain(win);
	CreateSRVHeapDesc();
	CreateRTVHeapDesc();
	CreateRTV();
	SetDepthRes();
	CreateDepthBuff();
	CreateDescHeap();
	CreateDSV();
	CreateFence();
}

void NDX12::ChoiceAdapters()
{
	// �p�t�H�[�}���X���������̂��珇�ɁA�S�ẴA�_�v�^�[��񋓂���
	for (UINT i = 0;
		dxgiFactory->EnumAdapterByGpuPreference(i,
			DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
			IID_PPV_ARGS(&tmpAdapter)) != DXGI_ERROR_NOT_FOUND;
		i++) {
		// ���I�z��ɒǉ�����
		adapters.push_back(tmpAdapter);
	}

	// �Ó��ȃA�_�v�^��I�ʂ���
	for (size_t i = 0; i < adapters.size(); i++) {
		DXGI_ADAPTER_DESC3 adapterDesc;
		// �A�_�v�^�[�̏����擾����
		adapters[i]->GetDesc3(&adapterDesc);
		// �\�t�g�E�F�A�f�o�C�X�����
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
			// �f�o�C�X���̗p���ă��[�v�𔲂���
			tmpAdapter = adapters[i];
			break;
		}
	}
}

void NDX12::CreateDevice()
{
	HRESULT result;

	// �Ή����x���̔z��
	D3D_FEATURE_LEVEL levels[] = {
	D3D_FEATURE_LEVEL_12_1,
	D3D_FEATURE_LEVEL_12_0,
	D3D_FEATURE_LEVEL_11_1,
	D3D_FEATURE_LEVEL_11_0,
	};

	//�����\�ȃo�[�W����������������ł��؂�
	for (size_t i = 0; i < _countof(levels); i++) {
		// �̗p�����A�_�v�^�[�Ńf�o�C�X�𐶐�
		result = D3D12CreateDevice(tmpAdapter.Get(), levels[i],
			IID_PPV_ARGS(&device));
		if (result == S_OK) {	//HRESULT�^�̊֐���true,false�̑����S_OK,����̌^���Ԃ����
			// �f�o�C�X�𐶐��ł������_�Ń��[�v�𔲂���
			featureLevel = levels[i];
			break;
		}
	}

#ifdef _DEBUG
	ComPtr<ID3D12InfoQueue> infoQueue;
	if (SUCCEEDED(device->QueryInterface(IID_PPV_ARGS(&infoQueue))))
	{
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);	//��΂��G���[�̎��~�܂�
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);		//�G���[�̎��~�܂�
	}

	//�}������G���[
	D3D12_MESSAGE_ID denyIds[] = {
		D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
	};

	//�}�������\�����x��
	D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
	D3D12_INFO_QUEUE_FILTER filter{};
	filter.DenyList.NumIDs = _countof(denyIds);
	filter.DenyList.pIDList = denyIds;
	filter.DenyList.NumSeverities = _countof(severities);
	filter.DenyList.pSeverityList = severities;
	//�w�肵���G���[�̕\����}������
	infoQueue->PushStorageFilter(&filter);
#endif // DEBUG
}

void NDX12::CreateCommandGroup()
{
	HRESULT result;

	// �R�}���h�A���P�[�^�𐶐�
	result = device->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&commandAllocator));
	assert(SUCCEEDED(result));

	// �R�}���h���X�g�𐶐�
	result = device->CreateCommandList(0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		commandAllocator.Get(), nullptr,
		IID_PPV_ARGS(&commandList));
	assert(SUCCEEDED(result));

	//�R�}���h�L���[�𐶐�
	result = device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue));
	assert(SUCCEEDED(result));
}

void NDX12::CreateSwapChain(NWindows win)
{
	HRESULT result;

	swapChainDesc.Width = win_width;
	swapChainDesc.Height = win_height;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // �F���̏���
	swapChainDesc.SampleDesc.Count = 1; // �}���`�T���v�����Ȃ�
	swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER; // �o�b�N�o�b�t�@�p
	swapChainDesc.BufferCount = 2; // �o�b�t�@����2�ɐݒ�
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // �t���b�v��͔j��
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	//�ꎞ�I��ComPtr
	ComPtr<IDXGISwapChain1> swapchain1;

	// �X���b�v�`�F�[���̐���
	result = dxgiFactory->CreateSwapChainForHwnd(
		commandQueue.Get(), win.hwnd, &swapChainDesc, nullptr, nullptr,&swapchain1);
	assert(SUCCEEDED(result));

	//���Ƃ̃X���b�v�`�F�[���ɕϊ�
	swapchain1.As(&swapchain);
}

void NDX12::CreateSRVHeapDesc()
{
	HRESULT result;

	//�V�F�[�_���\�[�X�r���[(SRV)�̍ő��
	const size_t kMaxSRVCount = 2056;
	// �f�X�N���v�^�q�[�v�̐ݒ�
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;	//�V�F�[�_���猩����悤��
	srvHeapDesc.NumDescriptors = kMaxSRVCount;	//�f�X�N���v�^�̎��Ă鐔�ݒ�

	//�ݒ�����Ƃ�SRV�p�f�X�N���v�^�q�[�v�𐶐�
	result = device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));
	assert(SUCCEEDED(result));
}

void NDX12::CreateRTVHeapDesc()
{
	// �f�X�N���v�^�q�[�v�̐ݒ�
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; //�����_�[�^�[�Q�b�g�r���[
	rtvHeapDesc.NumDescriptors = swapChainDesc.BufferCount; // ���\��2��
	// �f�X�N���v�^�q�[�v�̐���
	device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeap));
}

void NDX12::CreateRTV()
{
	// �o�b�N�o�b�t�@
	backBuffers.resize(swapChainDesc.BufferCount);

	// �X���b�v�`�F�[���̑S�Ẵo�b�t�@�ɂ��ď�������
	for (size_t i = 0; i < backBuffers.size(); i++) {
		// �X���b�v�`�F�[������o�b�t�@���擾
		swapchain->GetBuffer((UINT)i, IID_PPV_ARGS(&backBuffers[i]));
		// �f�X�N���v�^�q�[�v�̃n���h�����擾
		rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
		// �����\���ŃA�h���X�������
		rtvHandle.ptr += i * device->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);

		// �V�F�[�_�[�̌v�Z���ʂ�SRGB�ɕϊ����ď�������
		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		// �����_�[�^�[�Q�b�g�r���[�̐���
		device->CreateRenderTargetView(backBuffers[i].Get(), &rtvDesc, rtvHandle);
	}
}

void NDX12::SetDepthRes()
{
	//���\�[�X�ݒ�
	depthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResourceDesc.Width = win_width;	//�����_�[�^�[�Q�b�g�ɍ��킹��
	depthResourceDesc.Height = win_height;	//�����_�[�^�[�Q�b�g�ɍ��킹��
	depthResourceDesc.DepthOrArraySize = 1;
	depthResourceDesc.Format = DXGI_FORMAT_D32_FLOAT;	//�[�x�l�t�H�[�}�b�g
	depthResourceDesc.SampleDesc.Count = 1;
	depthResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;	//�f�v�X�X�e���V��

	//�[�x�l�p�q�[�v�v���p�e�B
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	//�[�x�l�̃N���A�ݒ�
	depthClearValue.DepthStencil.Depth = 1.0f;	//�[�x�l1.0f(�ő�l)�ŃN���A
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;	//�[�x�l�t�H�[�}�b�g
}

void NDX12::CreateDepthBuff()
{
	HRESULT result;

	result = device->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,	//�[�x�l�������݂Ɏg�p
		&depthClearValue,
		IID_PPV_ARGS(&depthBuff)
	);
}

void NDX12::CreateDescHeap()
{
	HRESULT result;

	dsvHeapDesc.NumDescriptors = 1;	//�[�x�r���[��1��
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;	//�f�v�X�X�e���V���r���[

	result = device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));
}

void NDX12::CreateDSV()
{
	HRESULT result;

	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;	//�[�x�l�t�H�[�}�b�g
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device->CreateDepthStencilView(
		depthBuff.Get(),
		&dsvDesc, dsvHeap->GetCPUDescriptorHandleForHeapStart()
	);
}

void NDX12::CreateFence()
{
	HRESULT result;

	result = device->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	assert(SUCCEEDED(result));
}
