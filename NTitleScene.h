#pragma once
#include "NDX12.h"

#include <vector>
#include <wrl.h>

#include "NRootParam.h"
#include "NMaterial.h"
#include "NGPipeline.h"
#include "NRootSignature.h"
#include "NObj3d.h"
#include "NTexture.h"
#include "NSprite.h"
#include "NPreDraw.h"
#include "NCube.h"
#include "Particle.h"
#include "Effect.h"
#include "NAudio.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

class NTitleScene
{
private:
	//オブジェクト
	NMaterial material;				//マテリアル
	std::unique_ptr<NObj3d> player;			//オブジェクト(定数バッファ)

	std::unique_ptr<NCube> cube;	//立方体情報(頂点、インデックス)

	////背景スプライト
	//static const int maxBackSprite = 3;	//背景スプライト数
	//NSprite* backSprite[maxBackSprite];	//背景スプライト

	//前景スプライト
	//static const int maxForeSprite = 2;	//前景スプライト数
	std::unique_ptr<NSprite> titleSprite;	//タイトルロゴ
	std::unique_ptr<NSprite> startSprite;	//スタートUI

	//カメラ
	XMMATRIX matProjection;
	XMMATRIX matView;

	//カメラ
	XMFLOAT3 eye = { 0, 0, -100 };	//視点座標
	XMFLOAT3 target = { 0, 0, 0 };	//注視点座標
	XMFLOAT3 up = { 0, 1, 0 };		//上方向ベクトル
	float angle = 0.0f;				//カメラの回転角

	NAudio* audio = nullptr;
	static const int maxSoundData = 3;
	uint32_t soundData[maxSoundData] = {};

public:
	//インスタンス取得
	static NTitleScene* GetInstance();

	//初期化
	void Initialize(NDX12* dx12);
	//更新
	void Update();
	//描画
	void Draw(NDX12* dx12);
	//リセット処理
	void Reset();
	//終了処理
	void Finalize();
};