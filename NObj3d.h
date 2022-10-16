#pragma once
#include "NDX12.h"
#include "NMaterial.h"

#include<memory>
#include <wrl.h>

//定数バッファ用データ構造体（3D変換行列）
struct  ConstBufferDataTransform
{
	XMMATRIX mat;	//3D変換行列
};

class NObj3d
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	D3D12_HEAP_PROPERTIES heapProp{};	//ヒープ
	D3D12_RESOURCE_DESC resDesc{};		//リソース

	ConstBufferDataTransform* constMapTransform;	//3D変換行列
	ComPtr<ID3D12Resource> constBuffTransform;		//定数バッファのGPUリソースのポインタ

	XMMATRIX matWorld{};	//3D変換行列
public:
	XMFLOAT3 scale = { 1.0f,1.0f,1.0f };	//スケーリング倍率
	XMFLOAT3 rotation = { 0.0f,0.0f,0.0f };	//回転角
	XMFLOAT3 position = { 0.0f,0.0f,0.0f };	//座標

	NObj3d* parent = nullptr;	//親のポインタ

	int texNum = 0;	//テクスチャ指定用

public:
#pragma region 初期化まわり
	//初期化
	void Initialize(ComPtr<ID3D12Device> device);

	//ヒープ設定
	void SetCBHeap();
	//リソース設定
	void SetCBResource();
	//定数バッファの生成
	void CreateCB(ComPtr<ID3D12Device> device);
	//定数バッファのマッピング
	void MappingCB();
#pragma endregion
#pragma region 更新まわり
	//キーボード操作
	void MoveKey();
	//ワールド行列の合成
	void UpdateMatrix();
	//定数バッファへ送信
	void TransferMatrix(XMMATRIX matView, XMMATRIX matProjection);
#pragma endregion
#pragma region 描画まわり
	//共通グラフィックスコマンド
	void CommonBeginDraw(ComPtr<ID3D12GraphicsCommandList> commandList,const ComPtr<ID3D12PipelineState> pipelineState,
		ComPtr<ID3D12RootSignature> rootSignature, ComPtr<ID3D12DescriptorHeap> srvHeap);
	//描画
	void Draw(ComPtr<ID3D12GraphicsCommandList> commandList, NMaterial material, ComPtr<ID3D12DescriptorHeap> srvHeap,
		D3D12_VERTEX_BUFFER_VIEW vbView, D3D12_INDEX_BUFFER_VIEW ibView, UINT indexSize, UINT incrementSize);
	//定数バッファビュー(CRV)の設定コマンド(マテリアル)
	void SetMaterialCBV(ComPtr<ID3D12GraphicsCommandList> commandList, NMaterial material);
	void SetSRVHeap(ComPtr<ID3D12DescriptorHeap> srvHeap, ComPtr<ID3D12GraphicsCommandList> commandList, UINT incrementSize);
	//頂点バッファビューの設定コマンド
	void SetVB(ComPtr<ID3D12GraphicsCommandList> commandList, D3D12_VERTEX_BUFFER_VIEW vbView);
	//インデックスバッファビューの設定コマンド
	void SetIB(ComPtr<ID3D12GraphicsCommandList> commandList, D3D12_INDEX_BUFFER_VIEW ibView);
	//定数バッファビュー(CRV)の設定コマンド(3D変換行列)
	void SetMatCBV(ComPtr<ID3D12GraphicsCommandList> commandList);
	//描画コマンド
	void DrawCommand(ComPtr<ID3D12GraphicsCommandList> commandList, UINT indexSize);
#pragma endregion
};