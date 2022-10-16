#include "NPreDraw.h"

void NPreDraw::SetResBarrier(ComPtr<IDXGISwapChain4> swapchain,
	std::vector<ComPtr<ID3D12Resource>> backBuffers, ComPtr<ID3D12GraphicsCommandList> commandList)
{
	// �o�b�N�o�b�t�@�̔ԍ����擾(2�Ȃ̂�0�Ԃ�1��)
	bbIndex = swapchain->GetCurrentBackBufferIndex();
	// 1.���\�[�X�o���A�ŏ������݉\�ɕύX
	barrierDesc.Transition.pResource = backBuffers[bbIndex].Get(); // �o�b�N�o�b�t�@���w��
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT; // �\����Ԃ���
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET; // �`���Ԃ�
	commandList->ResourceBarrier(1, &barrierDesc);
}

void NPreDraw::SetRenderTarget(ComPtr<ID3D12DescriptorHeap> rtvHeap, ComPtr<ID3D12Device> device,
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc, ComPtr<ID3D12DescriptorHeap> dsvHeap, ComPtr<ID3D12GraphicsCommandList> commandList)
{
	// 2.�`���̕ύX
	// �����_�[�^�[�Q�b�g�r���[�̃n���h�����擾
	rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
	rtvHandle.ptr += bbIndex * device->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);
	dsvHandle = dsvHeap->GetCPUDescriptorHandleForHeapStart();
	commandList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);
}

void NPreDraw::ClearScreen(ComPtr<ID3D12GraphicsCommandList> commandList)
{
	// 3.��ʃN���A R G B A
	FLOAT clearColor[] = { 0.1f,0.25f,0.5f,0.0f }; // ���ۂ��F
	commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
	commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void NPreDraw::SetViewport(ComPtr<ID3D12GraphicsCommandList> commandList)
{
	viewport.Width = win_width;
	viewport.Height = win_height;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;	//�ŏ��k�x
	viewport.MaxDepth = 1.0f;	//�ő�[�x
	// �r���[�|�[�g�ݒ�R�}���h���A�R�}���h���X�g�ɐς�
	commandList->RSSetViewports(1, &viewport);
}

void NPreDraw::SetScissorRect(ComPtr<ID3D12GraphicsCommandList> commandList)
{
	scissorRect.left = 0; // �؂蔲�����W��
	scissorRect.right = scissorRect.left + win_width; // �؂蔲�����W�E
	scissorRect.top = 0; // �؂蔲�����W��
	scissorRect.bottom = scissorRect.top + win_height; // �؂蔲�����W��
	// �V�U�[��`�ݒ�R�}���h���A�R�}���h���X�g�ɐς�
	commandList->RSSetScissorRects(1, &scissorRect);
}

void NPreDraw::BarrierReset(D3D12_RESOURCE_BARRIER barrierDesc, ComPtr<ID3D12GraphicsCommandList> commandList)
{
	// 5.���\�[�X�o���A��߂�
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;	//�`���Ԃ���
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;			//�\����Ԃ�
	commandList->ResourceBarrier(1, &barrierDesc);
}

void NPreDraw::CmdListClose(ComPtr<ID3D12GraphicsCommandList> commandList)
{
	HRESULT result;

	result = commandList->Close();
	assert(SUCCEEDED(result));
}

void NPreDraw::ExecuteCmdList(ComPtr<ID3D12GraphicsCommandList> commandList, ComPtr<ID3D12CommandQueue> commandQueue)
{
	ID3D12CommandList* commandLists[] = { commandList.Get() };
	commandQueue->ExecuteCommandLists(1, commandLists);
}

void NPreDraw::BufferSwap(ComPtr<IDXGISwapChain4> swapchain)
{
	HRESULT result;

	result = swapchain->Present(1, 0);
	assert(SUCCEEDED(result));
}

void NPreDraw::CommandWait(ComPtr<ID3D12CommandQueue> commandQueue, ComPtr<ID3D12Fence> fence, UINT64 fenceVal)
{
	commandQueue->Signal(fence.Get(), ++fenceVal);
	if (fence->GetCompletedValue() != fenceVal) {
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}
}

void NPreDraw::ClearQueue(ComPtr<ID3D12CommandAllocator> commandAllocator)
{
	HRESULT result;

	result = commandAllocator->Reset();
	assert(SUCCEEDED(result));
}

void NPreDraw::CmdListReset(ComPtr<ID3D12GraphicsCommandList> commandList, ComPtr<ID3D12CommandAllocator> commandAllocator)
{
	HRESULT result;

	result = commandList->Reset(commandAllocator.Get(), nullptr);
	assert(SUCCEEDED(result));
}