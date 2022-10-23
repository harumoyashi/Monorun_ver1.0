#include "NStageSelectScene.h"
#include "NSceneManager.h"
#include "NInput.h"
#include "Util.h"
#include <chrono>

// --イージング-- //
float EaseOutCubic(float start, float end, float t) {
	float time = 1 - pow(1 - t, 3);
	return start * (1.0f - time) + end * time;
	//return start * (1.0f - pow(1.0f - t, 3)) + end * t;
}

NStageSelectScene* NStageSelectScene::GetInstance()
{
	static NStageSelectScene instance;
	return &instance;
}

void NStageSelectScene::Reset() {
	// --時間計測に必要なデータ変数-- //
	nowCount_ = 0;
	startCount_ = 0;

	// --スクロールしてからの経過時間-- //
	nowScrollTime_ = 0.0f;

	// --現在選んでいるステージ-- //
	selectStage_ = 1;

	for (size_t i = 0; i < maxForeSprite; i++) {
		foreSprite[i]->position.x = 300.0f;
		foreSprite[i]->position.y = 400.0f + (i * 250.0f);
		foreSprite[i]->UpdateMatrix();

		// --イージング用変数初期化-- //
		easeStartPos_[i] = { 300.0f, 400.0f + (i * 250.0f), 0.0f };
		easeEndPos_[i] = { 300.0f, 400.0f + (i * 250.0f), 0.0f };
	}
}

void NStageSelectScene::Initialize(NDX12* dx12)
{
#pragma region 描画初期化処理
	////マテリアル(定数バッファ)
	//material.Initialize(dx12->GetDevice());

	////オブジェクト(定数バッファ)
	//for (size_t i = 0; i < maxObj; i++)
	//{
	//	obj3d[i].Initialize(dx12->GetDevice());
	//	obj3d[i].texNum = 1;
	//}

	//obj3d[1].position.x = -20.0f;
	//obj3d[2].position.x = 20.0f;

	////背景スプライト生成
	//for (size_t i = 0; i < maxBackSprite; i++)
	//{
	//	backSprite[i] = new NSprite();
	//	backSprite[i]->texNum = static_cast<int>(i);
	//	backSprite[i]->CreateSprite(dx12->GetDevice(), NSceneManager::GetTex()[backSprite[i]->texNum].texBuff);
	//	//sprite[i]->CreateClipSprite(dx12->GetDevice(),tex[sprite[i]->texNum].texBuff,{100.0f,0},{50.0f,100.0f});	//一部切り取って生成
	//	backSprite[i]->position.x = i * 300.0f + 400.0f;
	//	backSprite[i]->position.y = 400.0f;
	//	backSprite[i]->UpdateMatrix();
	//	backSprite[i]->TransferMatrix();
	//}

	stage_ = stage_->GetInstance();
	stage_->Initialize(dx12);

	//前景スプライト生成
	for (size_t i = 0; i < maxForeSprite; i++) {
		foreSprite[i] = new NSprite();
		foreSprite[i]->texNum = static_cast<int>(STAGESELECTIMAGE);
		foreSprite[i]->CreateClipSprite(dx12->GetDevice(), NSceneManager::GetTex()[foreSprite[i]->texNum].texBuff, {i * 200.0f, 0.0f}, {200.0f, 200.0f});
		//sprite[i]->CreateClipSprite(dx12->GetDevice(),tex[sprite[i]->texNum].texBuff,{100.0f,0},{50.0f,100.0f});	//一部切り取って生成
		foreSprite[i]->SetColor(1, 1, 1, 1);
		foreSprite[i]->size = { 200.0f,200.0f };
		foreSprite[i]->TransferVertex();
		foreSprite[i]->position.x = 300.0f;
		foreSprite[i]->position.y = 400.0f + (i * 250.0f);
		foreSprite[i]->UpdateMatrix();

		// --イージング用変数初期化-- //
		easeStartPos_[i] = { 300.0f, 400.0f + (i * 250.0f), 0.0f};
		easeEndPos_[i] = { 300.0f, 400.0f + (i * 250.0f), 0.0f};
	}

	// --時間計測に必要なデータ変数-- //
	nowCount_ = 0;
	startCount_ = 0;

	// --スクロールしてからの経過時間-- //
	nowScrollTime_ = 0.0f;

	// --現在選んでいるステージ-- //
	selectStage_ = 1;

#pragma endregion
#pragma region	カメラ初期化
	//射影投影変換//
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0f),		//上下画角45度
		(float)NWindows::win_width / NWindows::win_height,	//アスペクト比(画面横幅/画面縦幅)
		0.1f, 2000.0f					//前端、奥端
	);

	//ここでビュー変換行列計算
	matView;
	eye = { -100, 100, -1000 };	//視点座標
	target = { 0, 0, 0 };	//注視点座標
	up = { 0, 1, 0 };		//上方向ベクトル
	//ビュー変換行列作成
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

	angle = 0.0f;	//カメラの回転角
#pragma endregion
}

void NStageSelectScene::Update()
{
	// --[SPACE]を押したら-- //
	if (NInput::IsKeyTrigger(DIK_SPACE))
	{
		stage_->SetCSV(selectStage_);
		if (!NSceneManager::GetPlayEffect()) {
			NSceneManager::SetScene(GAMESCENE);
		}
	}

	// --[↑]を押したら-- //
	if (NInput::IsKeyTrigger(DIK_UP)) {
		// --選んでいるステージの値が1より大きいなら-- //
		if (selectStage_ > 1) {
			// --現在のカウントを取得-- //
			startCount_ = static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());

			for (size_t i = 0; i < 10; i++) {
				easeStartPos_[i] = foreSprite[i]->position;
				easeEndPos_[i].y += 250.0f;
			}

			// --現在選んでいるステージの値を1減らす-- //
			selectStage_--;
		}
	}

	// --[↓]を押したら-- //
	if (NInput::IsKeyTrigger(DIK_DOWN)) {
		// --選んでいるステージの値が10より小さいなら-- //
		if (selectStage_ < 10) {
			// --現在のカウントを取得-- //
			startCount_ = static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());

			for (size_t i = 0; i < 10; i++) {
				easeStartPos_[i] = foreSprite[i]->position;
				easeEndPos_[i].y -= 250.0f;
			}

			// --現在選んでいるステージの値を1減らす-- //
			selectStage_++;
		}
	}

	// --経過時間timeRate[s]の計算
	//nowCount_ = GetNowCount();
	nowCount_ = static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());
	nowScrollTime_ = nowCount_ - startCount_;
	nowScrollTime_ = nowScrollTime_ / 1000.0f;

	float timeRate = Util::Clamp(nowScrollTime_ / maxScrollTime_, 1.0f, 0.0f);

	float len[10];
	for (size_t i = 0; i < 10; i++) {
		foreSprite[i]->position.y = EaseOutCubic(easeStartPos_[i].y, easeEndPos_[i].y, timeRate);
		len[i] = abs(400.0f - foreSprite[i]->position.y);
		len[i] = Util::Clamp(len[i] / 600.0f, 1.0f, 0.0f);
		foreSprite[i]->size.x = 200 * (1.0f - len[i]);
		foreSprite[i]->size.y = 200 * (1.0f - len[i]);
		foreSprite[i]->TransferVertex();
		foreSprite[i]->UpdateMatrix();
	}


#pragma region 行列の計算
	//ビュー変換行列再作成
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

	//obj3d[0].MoveKey();
	//for (size_t i = 0; i < maxObj; i++)
	//{
	//	obj3d[i].UpdateMatrix();
	//	obj3d[i].TransferMatrix(matView, matProjection);
	//}
#pragma endregion
}

void NStageSelectScene::Draw(NDX12* dx12)
{
#pragma region グラフィックスコマンド
	// 4.描画コマンドここから
	////背景スプライト
	//for (size_t i = 0; i < maxBackSprite; i++)
	//{
	//	backSprite[i]->CommonBeginDraw(dx12->GetCommandList(), NSceneManager::GetPipelineSprite()->pipelineSet.pipelineState, NSceneManager::GetPipelineSprite()->pipelineSet.rootSig.entity, dx12->GetSRVHeap());
	//	backSprite[i]->Draw(dx12->GetSRVHeap(), NSceneManager::GetTex()[0].incrementSize, dx12->GetCommandList());
	//}

	////3Dオブジェクト
	//for (size_t i = 0; i < maxObj; i++)
	//{
	//	obj3d[i].CommonBeginDraw(dx12->GetCommandList(), NSceneManager::GetPipeline3d()->pipelineSet.pipelineState, NSceneManager::GetPipeline3d()->pipelineSet.rootSig.entity, dx12->GetSRVHeap());
	//	obj3d[i].Draw(dx12->GetCommandList(), material, dx12->GetSRVHeap(), NSceneManager::GetPipeline3d()->vbView, NSceneManager::GetPipeline3d()->ibView, NSceneManager::GetPipeline3d()->numIB, NSceneManager::GetTex()[0].incrementSize);
	//}

	//前景スプライト
	for (size_t i = 0; i < maxForeSprite; i++)
	{
		foreSprite[i]->CommonBeginDraw(dx12->GetCommandList(), NSceneManager::GetPipelineSprite()->pipelineSet.pipelineState, NSceneManager::GetPipelineSprite()->pipelineSet.rootSig.entity, dx12->GetSRVHeap());
		foreSprite[i]->Draw(dx12->GetSRVHeap(), NSceneManager::GetTex()[0].incrementSize, dx12->GetCommandList());
	}
	// 4.描画コマンドここまで
#pragma endregion
}

void NStageSelectScene::Finalize()
{
	for (size_t i = 0; i < maxForeSprite; i++)
	{
		delete foreSprite[i];
	}

	//for (size_t i = 0; i < maxBackSprite; i++)
	//{
	//	delete backSprite[i];
	//}
}