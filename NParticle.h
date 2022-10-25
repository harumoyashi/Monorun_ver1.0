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

	int impactTimer = 0; //パーティクル出す時間
	bool isImpact = false; //パーティクル出すかフラグ

	int directionX = -1;	//1:右向き -1:左向き

public:
	void Initialize(ComPtr<ID3D12Device> device);
	//壁伝いの時出るパーティクル
	void WallHit(bool isParticle, int isDirectionR, XMMATRIX matView, XMMATRIX matProjection, NObj3d* player);
	void Draw(NDX12* dx12, const ComPtr<ID3D12PipelineState> pipelineState, ComPtr<ID3D12RootSignature> rootSignature, UINT incrementSize);
};