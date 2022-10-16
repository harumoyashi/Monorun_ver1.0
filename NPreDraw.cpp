#include "NPreDraw.h"

void NPreDraw::SetResBarrier(ComPtr<IDXGISwapChain4> swapchain,
	std::vector<ComPtr<ID3D12Resource>> backBuffers, ComPtr<ID3D12GraphicsCommandList> commandList)
{
	// バックバッファの番号を取得(2つなので0番か1番)
	bbIndex = swapchain->GetCurrentBackBufferIndex();
	// 1.リソースバリアで書き込み可能に変更
	barrierDesc.Transition.pResource = backBuffers[bbIndex].Get(); // バックバッファを指定
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT; // 表示状態から
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET; // 描画状態へ
	commandList->ResourceBarrier(1, &barrierDesc);
}

void NPreDraw::SetRenderTarget(ComPtr<ID3D12DescriptorHeap> rtvHeap, ComPtr<ID3D12Device> device,
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc, ComPtr<ID3D12DescriptorHeap> dsvHeap, ComPtr<ID3D12GraphicsCommandList> commandList)
{
	// 2.描画先の変更
	// レンダーターゲットビューのハンドルを取得
	rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
	rtvHandle.ptr += bbIndex * device->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);
	dsvHandle = dsvHeap->GetCPUDescriptorHandleForHeapStart();
	commandList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);
}

void NPreDraw::ClearScreen(ComPtr<ID3D12GraphicsCommandList> commandList)
{
	// 3.画面クリア R G B A
	FLOAT clearColor[] = { 0.1f,0.25f,0.5f,0.0f }; // 青っぽい色
	commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
	commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void NPreDraw::SetViewport(ComPtr<ID3D12GraphicsCommandList> commandList)
{
	viewport.Width = win_width;
	viewport.Height = win_height;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;	//最小震度
	viewport.MaxDepth = 1.0f;	//最大深度
	// ビューポート設定コマンドを、コマンドリストに積む
	commandList->RSSetViewports(1, &viewport);
}

void NPreDraw::SetScissorRect(ComPtr<ID3D12GraphicsCommandList> commandList)
{
	scissorRect.left = 0; // 切り抜き座標左
	scissorRect.right = scissorRect.left + win_width; // 切り抜き座標右
	scissorRect.top = 0; // 切り抜き座標上
	scissorRect.bottom = scissorRect.top + win_height; // 切り抜き座標下
	// シザー矩形設定コマンドを、コマンドリストに積む
	commandList->RSSetScissorRects(1, &scissorRect);
}

void NPreDraw::BarrierReset(D3D12_RESOURCE_BARRIER barrierDesc, ComPtr<ID3D12GraphicsCommandList> commandList)
{
	// 5.リソースバリアを戻す
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;	//描画状態から
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;			//表示状態へ
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