#include "NSceneManager.h"
#pragma region staticメンバ変数初期化
NGPipeline* NSceneManager::gPipe3d = nullptr;
NGPipeline* NSceneManager::gPipeSprite = nullptr;
//シーンの初期化
int NSceneManager::scene = TITLESCENE;
//シーン変更フラグの初期化
bool NSceneManager::isSceneChange = false;
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
#pragma region グラフィックスパイプライン
	gPipe3d = new NGPipeline();
	gPipe3d->Initialize3d(dx12->GetDevice());
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
		gameScene->Update();
	}

	// --リザルトシーンの更新処理-- //
	else if (scene == RESULTSCENE) {
		//resultScene->Update();
	}

	// --シーン変更がされたら-- //
	if (isSceneChange == true) {
		// --タイトルシーンだったら-- //
		if (scene == TITLESCENE) {
			//リセット
		}

		// --ステージセレクトシーンなら-- //
		else if (scene == STAGESELECTSCENE) {
			//リセット
		}

		// --ゲームシーンなら-- //
		else if (scene == GAMESCENE) {
			//リセット
		}

		// --シーン変更フラグOFFにする-- //
		isSceneChange = false;
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
	scene = selectScene;

	// --シーン変更フラグをONに-- //
	isSceneChange = true;
}

NSceneManager::NSceneManager()
{
	titleScene = titleScene->GetInstance();
	stageSelectScene = stageSelectScene->GetInstance();
	gameScene = gameScene->GetInstance();
}
