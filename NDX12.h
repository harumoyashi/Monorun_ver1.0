#pragma once
#include "NWindows.h"
#include <DirectXTex.h>

#include <d3d12.h>
#include <dxgi1_6.h>	//Visual Studio Graphics Debuggerが誤動作起こしたら、1_4にすると解決するかも

#include <wrl.h>

#include <DirectXMath.h>
using namespace DirectX;

#include "NInput.h"

class NDX12
{
public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	//デバッグレイヤーをオンにするために使用されるインターフェイス
	ComPtr<ID3D12Debug> debugController;

	//基本オブジェクトの生成
	//受け皿になる変数生成
	ComPtr<ID3D12Device> device;						//色々なもの作成するための仮想アダプタ	
	ComPtr<IDXGIFactory6> dxgiFactory;					//アダプターの列挙状態の変化を検出できるようにするためのインターフェース	
	ComPtr<IDXGISwapChain4> swapchain;					//ダブルバッファリングのために画面切り替え用のバッファー管理するやつ	
	ComPtr<ID3D12CommandAllocator> commandAllocator;	//コマンドリストに格納する命令の為のメモリを管理するオブジェクト	
	ComPtr<ID3D12GraphicsCommandList> commandList;		//命令を一時的に格納しとくやつ
	ComPtr<ID3D12CommandQueue> commandQueue;			//GPUが実行するべき命令のリストを,指定した順序でGPUに転送するためのインターフェース

	ComPtr<ID3D12DescriptorHeap> rtvHeap;

	std::vector< ComPtr<IDXGIAdapter4>> adapters;	//アダプターの列挙用
	ComPtr<IDXGIAdapter4> tmpAdapter;				//ここに特定の名前を持つアダプターオブジェクトが入る

	D3D_FEATURE_LEVEL featureLevel;

	//コマンドキューの設定
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};

	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc{};

	ComPtr<ID3D12DescriptorHeap> srvHeap;

	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};

	std::vector<ComPtr<ID3D12Resource>> backBuffers;

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;		//レンダーターゲットビューハンドル
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};	//レンダーターゲットビューの設定
	
	D3D12_RESOURCE_DESC depthResourceDesc{};	//深度バッファリソース
	D3D12_HEAP_PROPERTIES depthHeapProp{};		//ヒーププロパティ
	D3D12_CLEAR_VALUE depthClearValue{};
	ComPtr<ID3D12Resource> depthBuff;			//深度バッファ
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};	//デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> dsvHeap;
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};

	ComPtr<ID3D12Fence> fence;	//CPUとGPUの同期に使われるやつ
	UINT64 fenceVal = 0;

	//DirectX初期化
	void Init(NWindows win);

	//ゲッター//
	//デバイス取得
	ID3D12Device* GetDevice() { return device.Get(); }
	//コマンドアロケーター取得
	ID3D12CommandAllocator* GetCommandAllocator() { return commandAllocator.Get(); }
	//コマンドリスト取得
	ID3D12GraphicsCommandList* GetCommandList() { return commandList.Get(); }
	//フェンス取得
	ID3D12Fence* GetFence() { return fence.Get(); }

private:
	//アダプター選択
	void ChoiceAdapters();
	//デバイスの生成
	void CreateDevice();
	//コマンドアロケータ、リスト、キューの生成
	void CreateCommandGroup();
	// スワップチェーンの生成
	void CreateSwapChain(NWindows win);
	//シェーダリソースビューのデスクリプタヒープ生成
	void CreateSRVHeapDesc();
	//レンダーターゲットビューのデスクリプタヒープ生成
	void CreateRTVHeapDesc();
	//レンダーターゲットビューの生成
	void CreateRTV();
	//深度バッファのリソース設定
	void SetDepthRes();
	//深度バッファ生成
	void CreateDepthBuff();
	//深度ビュー用デスクリプタヒープ生成
	void CreateDescHeap();
	//深度ビュー作成
	void CreateDSV();
	//フェンスの生成
	void CreateFence();
};

