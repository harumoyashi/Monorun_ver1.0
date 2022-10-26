#include "NSceneManager.h"
#pragma region staticメンバ変数初期化
NTexture NSceneManager::tex[];
NGPipeline* NSceneManager::gPipe3d = nullptr;
NGPipeline* NSceneManager::gPipeSprite = nullptr;
//シーンの初期化
int NSceneManager::nextScene_ = 0;
int NSceneManager::scene = TITLESCENE;
//シーン変更フラグの初期化
bool NSceneManager::isSceneChange = false;
bool NSceneManager::isActiveEffect_ = false;
bool NSceneManager::isPlayEffect_ = false;
#pragma region
NSceneManager* NSceneManager::GetInstance()
{
	static NSceneManager instance;
	return &instance;
}

void NSceneManager::Initialize(NDX12* dx12)
{
	//ルートパラメータの設定
	rootParams.SetDescRange();
	rootParams.SetRootParam();

	//テクスチャ生成
	tex[0].Load(L"Resources/droppin_title_logo.png");
	tex[1].Load(L"Resources/droppin_start.png");
	tex[2].Load(L"Resources/player_tex.png");
	tex[3].Load(L"Resources/block_tex.png");
	tex[4].Load(L"Resources/dorppin_number.png");
	tex[5].Load(L"Resources/white64x64.png");
	tex[6].Load(L"Resources/droppin_stageselect.png");
	tex[7].Load(L"Resources/droppin_result.png");
	tex[8].Load(L"Resources/droppin_retry.png");
	tex[9].Load(L"Resources/dorppin_next.png");
	tex[10].Load(L"Resources/droppin_stage_frame.png");
	tex[11].Load(L"Resources/droppin_time.png");
	tex[12].Load(L"Resources/dorppin_big_number.png");
	tex[13].Load(L"Resources/droppin_go.png");
	tex[14].Load(L"Resources/droppin_minus.png");
	tex[15].Load(L"Resources/km_h.png");
	tex[16].Load(L"Resources/dorppin_slash.png");
	tex[17].Load(L"Resources/doroppin_crystal.png");
	tex[18].Load(L"Resources/bound1.png");
	tex[19].Load(L"Resources/bound2.png");
	tex[20].Load(L"Resources/wall_tex.png");

	for (int i = 0; i < maxTex; i++)
	{
		tex[i].CreateMipmap();
		//テクスチャバッファ設定
		tex[i].SetTBHeap();
		tex[i].SetTBResource();
		tex[i].CreateTexBuff(dx12->GetDevice());
		tex[i].TexBuffMaping();
		//シェーダーリソースビュー設定
		tex[i].SetSRV();
		tex[i].CreateSRV(dx12->GetDevice(), dx12->GetSRVHeap(), i);
	}
#pragma region グラフィックスパイプライン
	gPipe3d = new NGPipeline();
	gPipe3d->pipelineSet = gPipe3d->CreatePipeline3d(dx12->GetDevice(), rootParams.entity);

	gPipeSprite = new NGPipeline();
	gPipeSprite->pipelineSet = gPipeSprite->CreatePipelineSprite(dx12->GetDevice(), rootParams.entity);
#pragma endregion
	titleScene->Initialize(dx12);
	stageSelectScene->Initialize(dx12);
	gameScene->Initialize(dx12);
}

void NSceneManager::Update(NDX12* dx12)
{
	// --シーン変更がされたら-- //
	if (isActiveEffect_) {
		effect_.Activate();
		isActiveEffect_ = false;
	}

	if (effect_.GetAllowChangeScene()) {
		scene = nextScene_;
		isSceneChange = true;
	}

	// エフェクトUpdate
	effect_.ExpandSquareUpdate();

	isPlayEffect_ = effect_.GetEffectPlay();

	if (isSceneChange) {
		// --タイトルシーンだったら-- //
		if (scene == TITLESCENE) {
			//リセット
			titleScene->Reset();
		}

		// --ステージセレクトシーンなら-- //
		else if (scene == STAGESELECTSCENE) {
			//リセット
			stageSelectScene->Reset();
		}

		// --ゲームシーンなら-- //
		else if (scene == GAMESCENE) {
			//リセット
			gameScene->Reset(dx12);
		}

		// --シーン変更フラグOFFにする-- //
		//isSceneChange = false;

		isSceneChange = false;

	}

	// --タイトルシーンの更新処理-- //
	if (scene == TITLESCENE) {
		titleScene->Update();
	}

	// --ステージセレクトシーンの更新処理-- //
	else if (scene == STAGESELECTSCENE) {
		stageSelectScene->Update();
	}

	// --ゲームシーンの更新処理-- //
	else if (scene == GAMESCENE) {
		gameScene->Update(dx12);
	}
}

void NSceneManager::Draw(NDX12* dx12)
{
#pragma region 描画前処理
	NPreDraw* preDraw = nullptr;
	preDraw = new NPreDraw();

	preDraw->SetResBarrier(dx12->GetSwapchain(), dx12->backBuffers, dx12->GetCommandList());
	preDraw->SetRenderTarget(dx12->GetRTVHeap(), dx12->GetDevice(), dx12->GetRTVHeapDesc(), dx12->GetDSVHeap(), dx12->GetCommandList());
	preDraw->ClearScreen(dx12->GetCommandList());

	preDraw->SetViewport(dx12->GetCommandList());
	preDraw->SetScissorRect(dx12->GetCommandList());
#pragma endregion
	// --タイトルシーンの描画処理-- //
	if (scene == TITLESCENE) {
		titleScene->Draw(dx12);
	}

	// --ステージセレクトシーンの描画処理-- //
	else if (scene == STAGESELECTSCENE) {
		stageSelectScene->Draw(dx12);
	}

	// --ゲームシーンの描画処理-- //
	else if (scene == GAMESCENE) {
		gameScene->Draw(dx12);
	}
	effect_.ExpandSquareDraw();
	dx12->PostDraw(preDraw->barrierDesc);
}

void NSceneManager::Finalize()
{
	titleScene->Finalize();
	stageSelectScene->Finalize();
	gameScene->Finalize();

	delete gPipe3d;
	delete gPipeSprite;
}

void NSceneManager::SetScene(int selectScene)
{
	// --シーンを変更-- //
	nextScene_ = selectScene;

	// --シーン変更フラグをONに-- //
	//isSceneChange = true;

	isActiveEffect_ = true;
}

NSceneManager::NSceneManager()
{
	titleScene = NTitleScene::GetInstance();
	stageSelectScene = NStageSelectScene::GetInstance();
	gameScene = NGameScene::GetInstance();
}