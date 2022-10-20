#pragma once
#include "NTitleScene.h"
#include "NStageSelectScene.h"
#include "NGameScene.h"

enum Scene {
	TITLESCENE,
	STAGESELECTSCENE,
	GAMESCENE,
	RESULTSCENE
};

class NSceneManager
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	NRootParam rootParams;	//ルートパラメータ

	//テクスチャ
	static const int maxTex = 4;	//テクスチャ数
	static NTexture tex[maxTex];			//テクスチャ

	//グラフィックスパイプライン
	static NGPipeline* gPipe3d;		//3Dオブジェクト用グラフィックスパイプライン
	static NGPipeline* gPipeSprite;	//スプライト用グラフィックスパイプライン

	NTitleScene* titleScene;
	NStageSelectScene* stageSelectScene;
	NGameScene* gameScene;	//ゲームシーン

	static int scene;		//現在のシーン
	static bool isSceneChange;	//シーンの初期化フラグ

public:
	

public:
	//インスタンス取得
	NSceneManager* GetInstance();

	//初期化
	void Initialize(NDX12* dx12);
	//更新
	void Update(NDX12* dx12);
	//描画
	void Draw(NDX12* dx12);
	//終了処理
	void Finalize();
	//シーンの変更
	static void SetScene(int selectScene);

	NSceneManager();

#pragma region ゲッター
	static NGPipeline* GetPipeline3d() { return gPipe3d; }
	static NGPipeline* GetPipelineSprite() { return gPipeSprite; }
	static NTexture* GetTex() { return tex; }
#pragma endregion
private:


};