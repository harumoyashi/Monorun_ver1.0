#pragma once
#include <DirectXMath.h>
#include "NDX12.h"
#include "NTexture.h"
#include "NRootSignature.h"

#include <wrl.h>

//シェーダー使うのに必要な方々
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

//頂点データ構造体
struct Vertex
{
	XMFLOAT3 pos;		//xyz座標
	XMFLOAT3 normal;	//法線ベクトル
	XMFLOAT2 uv;		//uv座標
};

class NGPipeline
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	struct PipelineSet
	{
		ComPtr<ID3D12PipelineState> pipelineState;
		NRootSignature rootSig;
	};

	//頂点まわり//
	UINT singleSizeVB;					//頂点バッファ1個当たりのサイズ
	UINT sizeVB;						//頂点バッファ全体のサイズ
	Vertex vertices[1000]{};			//頂点代入用
	D3D12_HEAP_PROPERTIES heapProp{};	//ヒープ
	D3D12_RESOURCE_DESC resDesc{};		//リソース
	ComPtr<ID3D12Resource> vertBuff;	//頂点バッファ
	Vertex* vertMap = nullptr;			//マップ用

	//頂点インデックス//
	unsigned short indices[1000]{};		//インデックス代入用
	
	ComPtr<ID3D12Resource> indexBuff;	//インデックスバッファ
	uint16_t* indexMap = nullptr;		//マップ用

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

public:
	//パイプラインの初期化(生成)
	//3Dオブジェクト
	void Initialize3d(ComPtr<ID3D12Device> device);
#pragma region 頂点まわり
	//頂点データ設定
	void SetVert();
	//ヒープ設定
	void SetVertHeap();
	//リソース設定
	void SetVertResource();
	//法線の計算
	void SetNormal();
	//バッファ作成
	void CreateVertBuff(ComPtr<ID3D12Device> device);
	//マッピング
	void VertMaping();
	//頂点バッファビュー作成
	void CreateVertBuffView();
#pragma endregion
#pragma region 頂点インデックスまわり
	//頂点インデックス設定
	void SetIndex();
	//リソース設定
	void SetIndexResource();
	//バッファ作成
	void CreateIndexBuff(ComPtr<ID3D12Device> device);
	//マッピング
	void IndexMaping();
	//インデックスバッファビュー作成
	void CreateIndexBuffView();
#pragma endregion
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
	void SetBlend();
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

