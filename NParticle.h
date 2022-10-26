#pragma once
#include "NObj3d.h"
#include "NMaterial.h"
#include "NCube.h"

class NParticle
{
private:
	//オブジェクト
	NMaterial material;				//マテリアル
	static const int maxObj = 50;	//オブジェクト数(これを使いまわす)
	std::unique_ptr<NObj3d> obj3d[maxObj];			//オブジェクト(定数バッファ)
	std::unique_ptr<NCube> cube;	//立方体情報(頂点、インデックス)

	bool isActive = false;	//これ立ってる限り処理続ける
	const int maxTimer = 20;
	int timer = 0;

	float scale = 0.0f;
	float rot = 0.0f;
	float speedX = 0.0f;
	float speedY = 0.0f;
	float speedZ = 0.0f;

public:
	NParticle();
	~NParticle();
	//白くてアルファ値ちょい下げの汎用的な奴
	void Initialize(ComPtr<ID3D12Device> device);
	//マテリアルとモデルを指定できる
	void Initialize(ComPtr<ID3D12Device> device, int modelNum);
	//即死ブロック破壊する際の初期化
	void BlockBreakInitialize(ComPtr<ID3D12Device> device, int modelNum, XMFLOAT2 pos);
	//壁伝いの時出るパーティクル
	void WallHit(bool isParticle, int isDirectionR, XMMATRIX matView, XMMATRIX matProjection, NObj3d* player);
	//即死ブロック破壊時のパーティクル
	void BlockBreak(int atOnce, XMMATRIX matView, XMMATRIX matProjection);
	void PlayerBreak(bool isParticle, int atOnce, XMMATRIX matView, XMMATRIX matProjection,NObj3d* obj,float maxTimer = 50);
	//描画
	void Draw(NDX12* dx12);

	//マテリアルをセット
	void SetMaterial(NMaterial material) { this->material = material; }

	// --パーティクルが有効か取得-- //
	bool GetIsActive() { return isActive; }
};