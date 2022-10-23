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
#include "StageManager.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

class NStageSelectScene
{
private:
	////オブジェクト
	//NMaterial material;				//マテリアル
	//static const int maxObj = 3;	//オブジェクト数
	//NObj3d obj3d[maxObj];			//オブジェクト(定数バッファ)

	////背景スプライト
	//static const int maxBackSprite = 3;	//背景スプライト数
	//NSprite* backSprite[maxBackSprite];	//背景スプライト

	StageManager* stage_;

	//前景スプライト
	static const int maxForeSprite = 10;	//前景スプライト数
	NSprite* foreSprite[maxForeSprite];	//前景スプライト

	// --イージング用-- //
	XMFLOAT3 easeStartPos_[10];
	XMFLOAT3 easeEndPos_[10];

	// --時間計測に執拗なデータ変数-- //
	int nowCount_;
	int startCount_;

	// --スクロールするのにかかる時間[s]-- //
	const float maxScrollTime_ = 0.5f;

	// --スクロール開始からの経過時間-- //
	float nowScrollTime_;

	// --現在選んでいるステージ-- //
	int selectStage_;

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
	NStageSelectScene* GetInstance();

	void Initialize(NDX12* dx12);
	void Update();
	void Draw(NDX12* dx12);
	void Finalize();
};

