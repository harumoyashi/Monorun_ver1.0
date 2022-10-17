#pragma once
#include "NDX12.h"
#include "NVector3.h"
#include "NMatrix4.h"

#include <vector>
#include <wrl.h>

#include "NMaterial.h"
#include "NGPipeline.h"
#include "NRootParam.h"
#include "NRootSignature.h"
#include "NObj3d.h"
#include "NTexture.h"
#include "NSprite.h"
#include "NPreDraw.h"
#include "NPostDraw.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

class NGameScene
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	NRootParam rootParams;	//ルートパラメータ

	//オブジェクト
	NMaterial material;				//マテリアル
	static const int maxObj = 3;	//オブジェクト数
	NObj3d obj3d[maxObj];			//オブジェクト(定数バッファ)

	//カメラ
	XMMATRIX matProjection;
	XMMATRIX matView;
	XMFLOAT3 eye = { 0, 0, -100 };	//視点座標
	XMFLOAT3 target = { 0, 0, 0 };	//注視点座標
	XMFLOAT3 up = { 0, 1, 0 };		//上方向ベクトル
	float angle = 0.0f;				//カメラの回転角

	//テクスチャ
	static const int maxTex = 3;	//テクスチャ数
	NTexture tex[maxTex];			//テクスチャ

	//スプライト
	static const int maxSprite = 3;	//スプライト数
	NSprite* sprite[maxSprite];		//スプライト

	//グラフィックスパイプライン
	NGPipeline* gPipe3d;		//3Dオブジェクト用グラフィックスパイプライン
	NGPipeline* gPipeSprite;	//スプライト用グラフィックスパイプライン

	float increment = 0.0f;	//テスト用

public:

public:
	void Initialize(NDX12* dx12);
	void Update();
	void Draw(NDX12* dx12);

	void Finalize();

private:


};

