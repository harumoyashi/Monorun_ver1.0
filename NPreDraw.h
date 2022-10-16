#pragma once
#include "NDX12.h"

#include <wrl.h>

class NPreDraw
{
public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	UINT bbIndex;
	D3D12_RESOURCE_BARRIER barrierDesc{};		//リソースバリア
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle{};	//レンダーターゲットビューハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle{};	//デスクリプタハンドル

	D3D12_VIEWPORT viewport{};	//ビューポート
	D3D12_RECT scissorRect{};	//シザー矩形

public:
	//リソースバリアで書き込み可能に変更
	void SetResBarrier(ComPtr<IDXGISwapChain4> swapchain,
		std::vector<ComPtr<ID3D12Resource>> backBuffers, ComPtr<ID3D12GraphicsCommandList> commandList);
	//描画先の変更
	void SetRenderTarget(ComPtr<ID3D12DescriptorHeap> rtvHeap, ComPtr<ID3D12Device> device,
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc, ComPtr<ID3D12DescriptorHeap> dsvHeap, ComPtr<ID3D12GraphicsCommandList> commandList);
	//画面クリアRGBA
	void ClearScreen(ComPtr<ID3D12GraphicsCommandList> commandList);
	//ビューポート設定
	void SetViewport(ComPtr<ID3D12GraphicsCommandList> commandList);
	//シザー矩形設定
	void SetScissorRect(ComPtr<ID3D12GraphicsCommandList> commandList);

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
};

