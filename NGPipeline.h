#pragma once
#include <DirectXMath.h>
#include "NDX12.h"
#include "NTexture.h"
#include "NRootSignature.h"

#include <wrl.h>

//シェーダー使うのに必要な方々
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

class NGPipeline
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	struct PipelineSet
	{
		ComPtr<ID3D12PipelineState> pipelineState;
		NRootSignature rootSig;
	};

	//シェーダーリソースビュー//
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};	//設定構造体

	//シェーダーまわり(頂点も含む)//
	ComPtr<ID3DBlob> vsBlob;	// 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob;	// ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	//頂点レイアウト//
	D3D12_INPUT_ELEMENT_DESC vertLayout3d[3]{};		//必要な分だけ用意する
	D3D12_INPUT_ELEMENT_DESC vertLayoutSprite[2]{};	//必要な分だけ用意する

	//パイプラインステート//
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc = {};

	//テクスチャサンプラー//
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};

public:
	//参照するものたち
	PipelineSet pipelineSet;			//パイプラインステートとルートシグネチャまとめたやつ

	D3D12_VERTEX_BUFFER_VIEW vbView{};	//頂点バッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView{};	//インデックスバッファビュー
	UINT sizeIB;						//インデックスバッファ全体のサイズ
	UINT numIB;							//インデックスバッファの数

public:
	//パイプラインの初期化(生成)
#pragma region シェーダーまわり
	//3Dオブジェクト用頂点シェーダーの読み込みとコンパイル
	void LoadVertShader3d();
	//3Dオブジェクト用ピクセルシェーダの読み込みとコンパイル
	void LoadPixelShader3d();
	//スプライト用頂点シェーダーの読み込みとコンパイル
	void LoadVertShaderSprite();
	//スプライト用ピクセルシェーダの読み込みとコンパイル
	void LoadPixelShaderSprite();
#pragma endregion
#pragma region 頂点レイアウトまわり
	//3Dオブジェクト用頂点レイアウト設定
	void SetVertLayout3d();
	//スプライト用頂点レイアウト設定
	void SetVertLayoutSprite();
#pragma endregion
#pragma region パイプラインステートまわり
	//シェーダーの設定(適用)
	void SetShader();
	//ラスタライザー設定
	void SetRasterizer(bool isCull);
	//ブレンド設定
	void SetBlend(bool AlphaToCoverageEnable);
	//入力レイアウトの設定
	void SetInputLayout(bool is3d);
	//図形の形状設定
	void SetTopology();
	//デプスステンシル(深度)設定
	void SetDepth(bool isDepth);
	//レンダーターゲット設定
	void SetRenderTarget();
	//アンチエイリアシングのためのサンプル数設定
	void SetAntiAliasing();
	//パイプラインにルートシグネチャをセット
	void SetRootSignature(ComPtr<ID3D12Device> device, std::vector<D3D12_ROOT_PARAMETER> rootParams);
	//グラフィックスパイプラインステートオブジェクトの生成
	void CreatePS(ComPtr<ID3D12Device> device);
#pragma endregion
#pragma region テクスチャサンプラー
	//テクスチャサンプラー設定
	void SetTexSampler();
#pragma endregion
#pragma region パイプライン生成
	//3Dオブジェクト用パイプライン生成
	PipelineSet CreatePipeline3d(ComPtr<ID3D12Device> device, std::vector<D3D12_ROOT_PARAMETER> rootParams);
	//スプライト用パイプライン生成
	PipelineSet CreatePipelineSprite(ComPtr<ID3D12Device> device, std::vector<D3D12_ROOT_PARAMETER> rootParams);
#pragma endregion

private:

};