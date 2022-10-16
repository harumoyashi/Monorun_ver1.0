#include "NPostDraw.h"

void NPostDraw::BarrierReset(D3D12_RESOURCE_BARRIER barrierDesc, ComPtr<ID3D12GraphicsCommandList> commandList)
{
	// 5.リソースバリアを戻す
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;	//描画状態から
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;			//表示状態へ
	commandList->ResourceBarrier(1, &barrierDesc);
}

void NPostDraw::CmdListClose(ComPtr<ID3D12GraphicsCommandList> commandList)
{
	HRESULT result;

	result = commandList->Close();
	assert(SUCCEEDED(result));
}

void NPostDraw::ExecuteCmdList(ComPtr<ID3D12GraphicsCommandList> commandList, ComPtr<ID3D12CommandQueue> commandQueue)
{
	ID3D12CommandList* commandLists[] = { commandList.Get()};
	commandQueue->ExecuteCommandLists(1, commandLists);
}

void NPostDraw::BufferSwap(ComPtr<IDXGISwapChain4> swapchain)
{
	HRESULT result;

	result = swapchain->Present(1, 0);
	assert(SUCCEEDED(result));
}

void NPostDraw::CommandWait(ComPtr<ID3D12CommandQueue> commandQueue, ComPtr<ID3D12Fence> fence, UINT64 fenceVal)
{
	commandQueue->Signal(fence.Get(), ++fenceVal);
	if (fence->GetCompletedValue() != fenceVal) {
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}
}

void NPostDraw::ClearQueue(ComPtr<ID3D12CommandAllocator> commandAllocator)
{
	HRESULT result;

	result = commandAllocator->Reset();
	assert(SUCCEEDED(result));
}

void NPostDraw::CmdListReset(ComPtr<ID3D12GraphicsCommandList> commandList, ComPtr<ID3D12CommandAllocator> commandAllocator)
{
	HRESULT result;

	result = commandList->Reset(commandAllocator.Get(), nullptr);
	assert(SUCCEEDED(result));
}

void NPostDraw::All(D3D12_RESOURCE_BARRIER barrierDesc, ComPtr<ID3D12GraphicsCommandList> commandList, ComPtr<ID3D12CommandQueue> commandQueue, ComPtr<IDXGISwapChain4> swapchain, ComPtr<ID3D12Fence> fence, UINT64 fenceVal, ComPtr<ID3D12CommandAllocator> commandAllocator)
{
	// 5.リソースバリアを戻す
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;	//描画状態から
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;			//表示状態へ
	commandList->ResourceBarrier(1, &barrierDesc);

	HRESULT result;

	result = commandList->Close();
	assert(SUCCEEDED(result));

	ID3D12CommandList* commandLists[] = { commandList.Get() };
	commandQueue->ExecuteCommandLists(1, commandLists);

	result = swapchain->Present(1, 0);
	assert(SUCCEEDED(result));

	commandQueue->Signal(fence.Get(), ++fenceVal);
	if (fence->GetCompletedValue() != fenceVal) {
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	result = commandAllocator->Reset();
	assert(SUCCEEDED(result));

	result = commandList->Reset(commandAllocator.Get(), nullptr);
	assert(SUCCEEDED(result));
}
