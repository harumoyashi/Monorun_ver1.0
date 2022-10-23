#pragma once
#include <DirectXMath.h>
#include "NDX12.h"
#include <d3dx12.h>

#include <wrl.h>

//頂点データ構造体
struct Vertex
{
	XMFLOAT3 pos;		//xyz座標
	XMFLOAT3 normal;	//法線ベクトル
	XMFLOAT2 uv;		//uv座標
};

enum ModelNum
{
	CUBE,		//立方体
	CRYSTAL,	//八面体
};

class NCube
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	//頂点まわり//
	UINT singleSizeVB;					//頂点バッファ1個当たりのサイズ
	UINT sizeVB;						//頂点バッファ全体のサイズ
	static std::vector<Vertex> vertices;//頂点データ配列
	ComPtr<ID3D12Resource> vertBuff;	//頂点バッファ
	Vertex* vertMap = nullptr;			//マップ用

	//頂点インデックス//
	static std::vector<unsigned short>indices;//頂点インデックス配列
	UINT sizeIB;						//インデックスバッファ全体のサイズ
	ComPtr<ID3D12Resource> indexBuff;	//インデックスバッファ
	uint16_t* indexMap = nullptr;		//マップ用

public:
	D3D12_VERTEX_BUFFER_VIEW vbView{};	//頂点バッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView{};	//インデックスバッファビュー
	UINT numIB;							//インデックスバッファの数

public:
	//3Dオブジェクト
	void Initialize(ComPtr<ID3D12Device> device, int modelNum = CUBE);

private:
#pragma region 頂点まわり
	//頂点データ設定
	void SetVertCube();
	//頂点データ設定
	void SetVertCrystal();
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
	void SetIndexCube();
	//頂点インデックス設定
	void SetIndexCrystal();
	//バッファ作成
	void CreateIndexBuff(ComPtr<ID3D12Device> device);
	//マッピング
	void IndexMaping();
	//インデックスバッファビュー作成
	void CreateIndexBuffView();
#pragma endregion
};

