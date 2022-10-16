#pragma once
#include "NDX12.h"

#include<memory>
#include <wrl.h>
template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

//定数バッファ用構造体
struct ConstBufferDataMaterial
{
	XMFLOAT4 color;	//色(RGBA)
};

class NMaterial
{
private:
	D3D12_HEAP_PROPERTIES heapProp{};	//ヒープ
	D3D12_RESOURCE_DESC resDesc{};		//リソース

public:
	ComPtr<ID3D12Resource> constBuffMaterial;
	//std::unique_ptr<ConstBufferDataMaterial> constMapMaterial;
	ConstBufferDataMaterial* constMapMaterial;		//色(RGBA)

public:
	~NMaterial();
#pragma region 初期化まわり
	//初期化
	void Initialize(ComPtr<ID3D12Device> device);
	//ヒープ設定
	void SetHeap();
	//リソース設定
	void SetResource();
	//定数バッファの生成
	void CreateCB(ComPtr<ID3D12Device> device);
	//定数バッファのマッピング
	void MappingCB();
	//色情報転送
	void TransferColor();
#pragma endregion
#pragma region 描画まわり
	//定数バッファビュー(CBV)の設定コマンド
	void SetCBV(ComPtr<ID3D12GraphicsCommandList> commandList);
#pragma endregion
};

