#pragma once
#include "NDX12.h"

#include <wrl.h>

class NPostDraw
{
public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	//バリア解除
	void BarrierReset(D3D12_RESOURCE_BARRIER barrierDesc, ComPtr<ID3D12GraphicsCommandList> commandList);
	//命令のクローズ
	//もうコマンドリストに積むのおしまい
	void CmdListClose(ComPtr<ID3D12GraphicsCommandList> commandList);
	// コマンドリストの実行
	void ExecuteCmdList(ComPtr<ID3D12GraphicsCommandList> commandList, ComPtr<ID3D12CommandQueue> commandQueue);
	// 画面に表示するバッファをフリップ(裏表の入替え)
	void BufferSwap(ComPtr<IDXGISwapChain4> swapchain);
	// コマンドの実行完了を待つ
	void CommandWait(ComPtr<ID3D12CommandQueue> commandQueue, ComPtr<ID3D12Fence> fence, UINT64 fenceVal);
	// キューをクリア
	void ClearQueue(ComPtr<ID3D12CommandAllocator> commandAllocator);
	// 再びコマンドリストを貯める準備
	void CmdListReset(ComPtr<ID3D12GraphicsCommandList> commandList, ComPtr<ID3D12CommandAllocator> commandAllocator);
	void All(D3D12_RESOURCE_BARRIER barrierDesc, ComPtr<ID3D12GraphicsCommandList> commandList,ComPtr<ID3D12CommandQueue> commandQueue,
		ComPtr<IDXGISwapChain4> swapchain, ComPtr<ID3D12Fence> fence, UINT64 fenceVal,ComPtr<ID3D12CommandAllocator> commandAllocator);
};

