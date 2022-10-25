#pragma once
#include "NDX12.h"

#include <vector>
#include <wrl.h>

#include "NRootParam.h"
#include "NMaterial.h"
#include "NCube.h"
#include "NGPipeline.h"
#include "NRootSignature.h"
#include "NObj3d.h"
#include "NTexture.h"
#include "NSprite.h"
#include "NPreDraw.h"
#include "Player.h"
#include "StageManager.h"
#include "Collision.h"
#include "NCamera.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

enum SelectText {
	RetryText = 1,
	NextText = 1,
	StageSelectText = 2
};

class NGameScene
{
private:
	Player* player_;

	StageManager* stage_;

	Collision* col_;

	// --オブジェクト-- //
	NMaterial material_;// -> マテリアル
	std::unique_ptr<NCube> cube;	//立方体情報(頂点、インデックス)

	std::unique_ptr<NCamera> camera;	//カメラ

	std::unique_ptr<NSprite> resultSprite;	//リザルトテキスト
	std::unique_ptr<NSprite> stageSelectSprite;	//ステージセレクトテキスト
	std::unique_ptr<NSprite> retrySprite;	//リトライテキスト
	std::unique_ptr<NSprite> nextSprite;	//ネクストテキスト
	std::unique_ptr<NSprite> timeSprite[5];//数字
	std::unique_ptr<NSprite> decimalPointSprite;

	int displayNum[5] = {0, 0, 0, 0, 0};

	int selectText = 1;

	float cosRota;

	int startCount_;

	float gameTime_;

	bool isDisplayTimeChange;

public:


public:
	//インスタンス取得
	static NGameScene* GetInstance();

	//初期化
	void Initialize(NDX12* dx12);
	//更新
	void Update(NDX12* dx12);
	//描画
	void Draw(NDX12* dx12);
	//リセット処理
	void Reset(NDX12* dx12);
	//終了処理
	void Finalize();

private:


};