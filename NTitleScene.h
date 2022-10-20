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

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

class NTitleScene
{
private:
	//オブジェクト
	NMaterial material;				//マテリアル
	static const int maxObj = 1;	//オブジェクト数
	NObj3d obj3d[maxObj];			//オブジェクト(定数バッファ)

	////背景スプライト
	//static const int maxBackSprite = 3;	//背景スプライト数
	//NSprite* backSprite[maxBackSprite];	//背景スプライト

	//前景スプライト
	static const int maxForeSprite = 2;	//前景スプライト数
	NSprite* foreSprite[maxForeSprite];	//前景スプライト

	//カメラ
	XMMATRIX matProjection;
	XMMATRIX matView;

	//カメラ
	XMFLOAT3 eye = { 0, 0, -100 };	//視点座標
	XMFLOAT3 target = { 0, 0, 0 };	//注視点座標
	XMFLOAT3 up = { 0, 1, 0 };		//上方向ベクトル
	float angle = 0.0f;				//カメラの回転角

public:
	//インスタンス取得
	NTitleScene* GetInstance();

	void Initialize(NDX12* dx12);
	void Update();
	void Draw(NDX12* dx12);
	void Finalize();
};