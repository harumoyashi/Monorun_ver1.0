#pragma once
#include "NDX12.h"

#include <wrl.h>

class NTexture
{
public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	TexMetadata metadata{};
	ScratchImage scratchImg{};

	D3D12_HEAP_PROPERTIES texHeapProp{};	//ヒープ
	D3D12_RESOURCE_DESC texResDesc{};		//リソース
	ComPtr<ID3D12Resource> texBuff;			//テクスチャバッファ
	ScratchImage mipChain{};				//ミップマップ

	
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle;		//シェーダーリソースビューハンドル
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};	//設定構造体

	UINT incrementSize;	//次のテクスチャ情報の場所に進むために使う

public:
	//WICテクスチャのロード
	void Load(const wchar_t* pictureName);
	//ミップマップ生成
	void CreateMipmap();

	//ヒープ設定
	void SetTBHeap();
	//リソース設定
	void SetTBResource();
	//テクスチャバッファの生成
	void CreateTexBuff(ComPtr<ID3D12Device> device);
	//ミップマップデータの転送
	void TexBuffMaping();
	//シェーダーリソースビュー設定
	void SetSRV();
	//シェーダーリソースビュー生成
	void CreateSRV(ComPtr<ID3D12Device> device, ComPtr<ID3D12DescriptorHeap> srvHeap, int texNum);
};

